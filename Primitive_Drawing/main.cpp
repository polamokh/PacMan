#include <iostream>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/glfw3.h>
#include "Renderer.h"

int KeyPressed = -1;
double MouseXPos = -1.0;
double MouseYPos = -1.0;

bool InitGraphics();
void SpecialKeyPressed(GLFWwindow*, int, int, int, int);
void MouseClicked(GLFWwindow*, int, int, int);

int main(void)
{
	InitGraphics();
	return 0;
}

/**
Creates the main window and initializes OpenGL stuff.
*/
bool InitGraphics()
{
	Renderer *renderer = new Renderer(); //All drawing code goes here
	GLFWwindow* window;

	// ******************** Initialise GLFW ******************** //
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL version 3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 600, "Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// ******************** Initialize GLEW ******************** //
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, &SpecialKeyPressed);
	glfwSetMouseButtonCallback(window, &MouseClicked);


	// ******************** Initialize OpenGL ******************** //
	renderer->Initialize();

	do { // Loop

		renderer->Draw();
		renderer->Update();

		if (KeyPressed != -1) {
			renderer->HandleKeyboardInput(KeyPressed);
			KeyPressed = -1;
		}

		if (MouseXPos != -1 && MouseYPos!=-1) {
			renderer->HandleMouseClick(MouseXPos, MouseYPos);
			MouseXPos = MouseYPos = -1;
		}

		// Swap buffers
		glfwSwapBuffers(window); //Displaying our finished scene
		glfwPollEvents(); //try to comment it

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	// Deleting renderer object will deallocate it's memory and call it's destructor (which calls the renderer's Cleanup method)
	delete renderer;
	return true;

}

void SpecialKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS || GLFW_REPEAT)
		KeyPressed = key;
}

void MouseClicked(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_MOUSE_BUTTON_LEFT)
		glfwGetCursorPos(window, &MouseXPos, &MouseYPos);
}