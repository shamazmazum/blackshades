/*
 * Alerts.h
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

#ifndef _MYALERTS_H_
#define _MYALERTS_H_

#include <string.h>
#include <stdlib.h>

#define	kMoveToFront		kFirstWindowOfClass
// Alerts
#define	kResID_ALRT_MessageAlert		128
#define	kResID_ALRT_ErrorAlert			129
// Dialogs
#define	kResID_DLOG_SelectResolution	130
#define	iOK								1
#define	iResolutionPopUp				2
// Menus
#define	mResolution						128
#define	i640x480						1
#define	i800x600						2
#define	i1024x768						3
// String resources
#define	kResID_STRn_ErrorStrings		128
// Misc
#define kInsignificantConstant7454955	0

// Error numbers        
#define	kErr_DSpFindBestContextFailed	1
#define	kErr_DSpContext_ReserveFailed	2
#define	kErr_ActivateContextFailed		3
#define	kErr_DSpStartupFailed			4
#define	kErr_DSpFadeFailed				5
#define	kErr_AGLContext_CreationFailed	6

int SelectResolution(void);
void MessageAlert(unsigned char *theMessage);
void FatalErrorAlert(UInt16 errorNum, OSErr osError);

#endif
