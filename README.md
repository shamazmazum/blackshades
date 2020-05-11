# Black Shades
[![Build Status](https://travis-ci.com/shamazmazum/blackshades.svg?branch=master)](https://travis-ci.com/shamazmazum/blackshades)

By David Rosen (SDL2 port — Vasily Postnicov)

## Controls:

* `WASD` walk
* `shift` run
* `mouse` look
* `control` crouch/zoom
* `click` = fire (while aiming) or smash (while running) or pick up gun
  (while crouching over a body and not aiming) or disarm (while not
  aiming).
* `q` aim or un-aim (important for picking up guns)
* `r` reload
* `e` psychic aim
* `z` toggle soul release
* `space` dive (while running forwards)
* `tab` toggle third person

Thats it

## Instructions:
Try to keep your VIP (the guy in white) alive as long as possible. The
assassins will all try to shoot or stab him to death. You must do all
you can to prevent this. Your reputation has preceded you, so the VIP
has absolute confidence in your abilities and will completely ignore
all the assassins. When an assassin is aiming at your VIP with a gun,
you will psychicly see a line of sight extending from him to your
VIP. This line will narrow and redden until it disappears and the
assassin fires. Depending on the situation it may be best just to
shoot the assassin(s), or to dive and tackle the VIP to the ground to
avoid the bullet. Unfortunately your psychic powers do not show the
line of sight of knife-wielding assassins.

If you are feeling overwhelmed you can use psychic aiming to
temporarily speed up your thought processes and aim your shots
better. If there are no visible enemies you may want to release your
soul and look for nearby enemies. When your soul is released your VIP
pulsates between blue and red, civilians between black and red, and
assassins are solid red.

If you die or your VIP is killed.. the level restarts. If you are are
interested in the theory behind this: you are a psychic bodyguard, so
you can see small distances into the future. The failure only occured
in a possible future which you are now going to try and avoid. 

The scoring system consists of: 
* 150 points for a successful disarm
* 300 points for destroying a zombie (by blowing its head off)
* 100+50x points for completing a mission where x is the mission
  number (i.e. 450 points for completing mission 3).
* 75 points for incapacitating an assassin
* +50 if he had a knife
* -300 points for hurting a civilian
* -200 points for allowing the VIP to die. The penalty for failing to
  protect the VIP is halved if you kill the assassin. 

You can edit levels by setting "Custom levels" to 1 in the config and
editing the customlevels.txt file in the data folder.

## Weapons:

* Bare Hands: Smack people with them. Or if you want to be nice, walk
  or stand (don't run) near somebody with a gun and take it away.
* Knife: Like bare hands, but deadly and with longer range. Look out
  for knife-wielding assassins, they are the most dangerous.
* Handgun: One shot with this should be enough to incapacitate any
  human target, but they may remain conscious for a second (this can
  be bad if they are about to stab your VIP, aim for the head).
* Magnum:  Not as much ammo as the vanilla handgun, but one shot is an
  instantenous kill.
* Assault Rifle: This weapon has quite a kick and is bigger and more
  unwieldy than the handguns, but it has a large magazine and can fire
  quickly if necessary.
* Sniper Rifle: A bit more powerful than the magnum, with a
  scope. Very difficult to aim unless you are zoomed in (zoom by
  holding the control key).
* Shotgun: Aim and shoot. Very powerful but somewhat inaccurate.
* Grenade: Hold down the mouse button to take out the pin, release the
  button to throw it. Crouch to change your mind and put the pin back
  in. You can knock people out if you hit them in the head, or  of
  course you can just blow them to pieces.

Main menu music is included with permission from musician John Graham,
Copyright 2002. Due to legal difficulties all other music is composed
by me at the last minute based on some nice loops made by Carlos
Camacho, hence the 'programmer music'. Also many thanks to my other
friends and beta testers, and to artist David Drew for modelling the
assault rifle, sniper rifle, handgun, shotgun and grenade.

## Black Shades Elite

This is the new-ish version "Black Shades", which uses the original
code, but will add yet to be determined features while still keeping
the original functionality and gameplay.

Currently the source is being cleaned up, and slowly redesigned so
extra features are possible.  The number 1 feature will be additional
network play.

Other more minor features are in the works or done:

* Help screen (shows keys used to play the game) (done).
* The ability to change the resolution, (done)
* Fullscreen option in the config. (done)
* Mouse grab for window mode. (done)
* Fix framerate limiting in loop. (done - needs testing)
* Sound issus (relate to fps limiting?).
* Saving (just a file with a mission number?)
* A model/anim viewer
* Button widget (partial - drawing still separate)
* Input widget
* Mission class
* Civilian class that inherits Person (for all AI stuff in
  GameTick.cpp)
* Weapon class (partially done)
* Net framework

Thanks to David Rosen.
 * GPL Licensed with permission from David Rosen
 * David Rosen reserves the right to Black Shades 2
