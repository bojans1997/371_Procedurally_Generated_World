=========================
FEATURES
=========================

>>> Assignment 1 <<<
- The application opens a 1024x768 window which contains a 100x100 square grid.
- There are 5 models in the grid, each containing the third letter of our first names and the fourth digit of our student IDs.
- The X, Y, and Z axes are marked with red, green, and blue lines in the middle of the grid.
- The models can be rotated, scaled up and down, and moved along the XY plane.
- The world orientation can be rotated and moved up and down.
- The camera can pan and tilt as well as zoom in and out.
- The rendering mode can be swapped between triangles, points, and lines.
- Pressing the spacebar makes the models move at a random place on the grid.

>>> Assignment 2 <<<
- Each letter / digit pair is enclosed in a sphere.
- There is a light source in the middle of the scene, 30 units in the air.
- The grid (floor) has a texture.
- The letters have a box texture.
- The digits have a golden / metallic texture.
- Textures can be toggled on / off using the 'x' key.
- The letter / digit pairs cast shadows relative to the light source.
- Shadows can be toggled on / off using the 'b' key.

========================
CONTROLS
========================

>>> Assignment 1 <<<
- Model manipulation
	- "u" -> scale up
	- "j" -> scale down
	- "a" -> move left
	- "d" -> move right
	- "s" -> move down
	- "q" -> move forward
	- "e" -> move backward
	- "w" -> move up
	- "k" -> rotate clockwise
	- "i" -> rotate counter-clockwise
	- "space" -> reposition models to random location on grid

- Rendering mode change
	- "t" -> render using triangles
	- "p" -> render using points
	- "l" -> render using lines (lowercase L)

- World orientation
	- "right arrow" -> rotate world clockwise
	- "left arrow"  -> rotate world counter-clockwise
	- "up arrow"    -> tilt world up
	- "down arrow"  -> tilt world down
	- "Home key"    -> reset world orientation

- Camera
	- "hold right mouse button"   -> move mouse along x axis to pan camera
	- "hold middle mouse button"  -> move mouse along y axis to tilt camera
	- "hold left mouse buttin"    -> move mouse along y axis to zoom in/out
	
>>> Assignment 2 <<<
- Rendering
	- "x" -> toggle textures on / off.
	- "b" -> toggle shadows on / off.

- Movements
	- 't' -> move model backwards
	- 'g' -> move model forward

>>> Assignment 3 <<<

	- "SHIFT" -> run
	- "ESCAPE" -> exit the window
	- "HOME" -> reset position
	- "w" -> walk forward
	- "a" -> walk left
	- "s" -> walk backward
	- "d" -> walk right
	- "e" -> pickup apple
 	- "SPACE" -> jump
	- "F11" -> Toggle fullscreen
	- "m" -> toggle mute music