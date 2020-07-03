#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include "objects/grid.h"
#include "objects/axis.h"

const int WINDOW_LENGTH = 1024;
const int WINDOW_WIDTH = 768;

GLFWwindow* window;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WINDOW_LENGTH, WINDOW_WIDTH, "COMP 371 Assignment 1 - OpenGLHF", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int VBO = 0, VAO = 0;

	Grid *grid = new Grid(100);
	Axis *axis = new Axis(5);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		grid->draw();
		axis->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	delete grid;
    glfwTerminate();
    return 0;
}