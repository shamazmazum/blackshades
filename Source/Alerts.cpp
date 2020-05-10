/*
 * Alerts.cpp
 * Copyright (C) 2007 by Bryan Duff <duff0097@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "Alerts.h"

/********************> SelectResolution() <*****/
int SelectResolution(void)
{
  DialogPtr dialog;
  Boolean dialogDone = false;
  short itemHit, itemType;
  Handle okItem;
  Handle resolutionItem;
  Rect itemRect;
  int selectionNum;

  // Load the dialog
  dialog = GetNewDialog(kResID_DLOG_SelectResolution, nil, kMoveToFront);

  // Display the dialog
  ShowWindow(dialog);
  SetPort(dialog);

  // Load dialog items
  SetDialogDefaultItem(dialog, iOK);
  SetDialogTracksCursor(dialog, true);
  GetDialogItem(dialog, iOK, &itemType, &okItem, &itemRect);
  GetDialogItem(dialog, iResolutionPopUp, &itemType, &resolutionItem,
                &itemRect);

  // Set item values
  SetControlValue((ControlHandle) resolutionItem, i640x480);

  while(!dialogDone) {

    ModalDialog(nil, &itemHit);

    switch (itemHit) {
      case iOK:
        dialogDone = true;
        // Get the item number selected int the popup
        selectionNum = GetControlValue((ControlHandle) resolutionItem);
        break;
      case iResolutionPopUp:
        // We don't actually need to do anything here
        break;
    }

  }

  DisposeDialog(dialog);

  // Return the item selected in the popup menu
  return selectionNum;
}

/********************> MessageAlert() <*****/
void MessageAlert(unsigned char *theMessage)
{
  // Set parameter ^0 to our message (I could set up to three, but for simplicity's sake I won't)
  ParamText((unsigned char *)theMessage, NULL, NULL, NULL);

  // Do the Alert
  NoteAlert(kResID_ALRT_MessageAlert, nil);
}

/********************> FatalErrorAlert() <*****/
void FatalErrorAlert(UInt16 errorNum, OSErr osError)
{
  Str15 errNumStr;
  Str255 mainMessage;

  // Convert the OSErr to a string
  NumToString(osError, errNumStr);

  // Get the error description (inErrorDesc) from the STR# resource
  GetIndString(mainMessage, kResID_STRn_ErrorStrings, errorNum);

  // Set the parameters (^0 and ^1) in the ALRT to our error messages
  ParamText(mainMessage, errNumStr, NULL, NULL);

  // Do the alert (which now has our messages in it)
  StopAlert(kResID_ALRT_ErrorAlert, NULL);

  // Quit
  exit(EXIT_SUCCESS);
}
