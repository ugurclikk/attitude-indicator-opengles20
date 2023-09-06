# OpenGL ES 2.0 Attitude Indicator

This is a simple OpenGL ES 2.0 program simulating an attitude indicator. It uses GLFW for window management and rendering, featuring inner and outer circles, a stick, and arrow textures representing different components of the attitude indicator.



https://github.com/ugurclikk/attitude-indicator-opengles20/assets/75498822/2f51a472-4aa6-4053-a222-c0f92fb70131




## Prerequisites

Ensure the following are installed on your system before running this program:

- [GLFW](https://www.glfw.org/): Used for window management.
- [OpenGL ES 2.0](https://www.khronos.org/opengles/2_X/): The OpenGL ES version used for rendering.

## Building and Running

To build and run the program, follow these steps:

1. Clone the repository or download the source code.

2. Open a terminal and navigate to the project directory.

3. Compile the program using the following command:

g++ -o attitude main.cpp -lglfw -lGLESv2 -lEGL

This command generates an executable named `attitude`.

4. Run the program:

./attitude


This will launch the OpenGL ES 2.0 Attitude Indicator window.

## Controls

- Press 'R' to randomize the attitude indicator's position.
- Press 'C' to reset the attitude indicator to its default position.
- Use the arrow keys to adjust the rotation of the outer circle.
- Use 'W' and 'S' to adjust the vertical position of the inner circle.
- Use 'A' and 'D' to adjust the rotation of the inner circle.

## Additional Information

- The program uses two different shaders, one for the circles and one for the stick.
- Textures are loaded for the inner and outer circles.
- Alpha blending is enabled to draw lines on top of the texture.
.

## Acknowledgments

- The OpenGL ES 2.0 Attitude Indicator is based on [OpenGL](https://www.opengl.org/) and [GLFW](https://www.glfw.org/).
- Some code for handling random numbers is included in the program.
