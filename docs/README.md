# Swap game
Swap game is a puzzle-action-platformer where you must overcome a series of challenges using your intelligence and reflexes!
This game was made as part of the development subject by 2 students in CITM (UPC). We've programmed the game from scratch in C++ and used free artwork.

## Credits
- Programming: Jose Antonio Prieto
> _Github profile: https://github.com/peterMcP_

- Programming: Jaume Montagut
> _Github profile: https://github.com/JaumeMontagut_

## Innovations:
- Get background animations directly edited in Tiled Map Editor.
- Mechanic that lets you change position with other objects.
- Shoot with mouse.
- Get music from the level from Tiled Map Editor.
- Camera debugger
- Optimization: Things out of the screen aren't rendered
- Optimization: Each object has a different updateCycle which is used to avoid that all objects' TimedUpdate() are called at the same time.
- Controller support and specific gameplay mechanic for this type of input.
- Walking enemy: Kills you if you touch it from the sides. You can stand on top of it to solve puzzles.
- Flying enemy: Moves in a random direction if it does't find a path. Stops if you mark it and lets you stand above it.
- You can swap position with enemies too!
- Get all entities directly from each map xml
- When you swap with an object you also exchange velocities.
- 13 levels!
- Added gui element templates on an xml to easlily create similar elements.
- Loaded elements gui elements from Tiled.

## Release
[Download game](https://github.com/JaumeMontagut/CITM_2_Dev_Platformer/releases/tag/v1.5)


## Video
[![Watch the video](https://www.youtube.com/watch?v=QE5P8YfVCxM)](https://www.youtube.com/watch?v=QE5P8YfVCxM)

<iframe width="560" height="315" src="https://www.youtube.com/embed/QE5P8YfVCxM" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Member contributions
Jose Antonio Prieto
- Flying enemy
- A*
- Save and load state of all entities
- Brofiler
- Added module particles
- Added gamepad hotplug functionality
- Implemented trigger import(from tiled map editor)/ functionality
- Added new object trigger events to entity system
- Added new object door
- Get objects from Tiled xml

Jaume Montagut Guix
- Walking enemy
- A*
- Added module entities (j1Object)
- Incorporation of dt to stabilize movement & animations
- Implemented gamepad and - exclusive gameplay!" -
- new material to play with it, glass
- implement camera draw limits and its debug functionality
- designed some new cool levels!
- multitude of fixes/improvements
- Added GUI slider

## How to play:
### Controller:
- Left joystick: Move
- A: Jump
- B: Shoot a projectile which marks objects
- Left Joystick (holding B): Aim
- X: Swap positions with marked object

### Keyboard & Mouse:
- A: Move left
- D: Move right
- Space: Jump
- Mouse left button: Shoot a projectile which marks objects
- Mouse right button: Swap positions with marked object

## License:
	GNU GPLv3
	-Swap Game- (Provisional name)
    Copyright (C) 2018  Jaume Montagut / Jose Antonio Prieto

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
	
	See the license here: https://github.com/JaumeMontagut/CITM_2_Dev_Platformer/blob/master/LICENSE

## Find this project's repository in:
https://github.com/JaumeMontagut/CITM_2_Dev_Platformer
