# Snowman-Fight
Simple game developed using OpenGL and GLUT.
This is a class project developed in Syracuse University in a introductory Computer Graphics course.

This project was written in C++ using OpenGL, GLUT and GLU.
This software was tested under Windows OS, and developed using CodeBlocks IDE.

Before compiling, make sure to include "opengl32", "glut32" and "glu32" to the link list.

A binary release can be found for Windows users.

The project proposal can be seen in the file "Project Proposal.pdf".

# How to Play
After starting a new game, inputs can be customized by acessing the in-game menu by pressing Return Key (Enter).

Inputs can be changed on the second tab of the menu.

The default inputs are:

	w, a, s, d: move forward, backward, to the left, to the right.

	arrow keys: change look direction of camera.

	space bar: charges power to throw a snowball.

	tab: changes camera mode.

	1: changes snowman character face style.

To fast exit the game, the Escape key can be pressed anytime.

To freeze the game anytime, the "`" key can be pressed.

# Known Issues
When pushing down a keyboard letter button with Capital Lock On, the software will be notified that the uppercase letter was pressed, but when the key is released, the software will be notified that the lowercase letter was released.
The trouble that it causes is that the uppercase letter will be treated as if it was never released.
