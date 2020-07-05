#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include "objects/shader.h"
#include "objects/grid.h"
#include "objects/axis.h"
#include "objects/cube.h"

const int WINDOW_LENGTH = 1024;
const int WINDOW_WIDTH = 768;

GLFWwindow* window;

unsigned int modelRenderMode = GL_TRIANGLES;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		modelRenderMode = GL_POINTS;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		modelRenderMode = GL_TRIANGLES;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		modelRenderMode = GL_LINES;
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

	Shader *basicShader = new Shader("src/shaders/shader.vs", "src/shaders/shader.fs");
	Shader *cameraShader = new Shader("src/shaders/camera.vs", "src/shaders/shader.fs");

	Grid *grid = new Grid(100);
	Axis *axis = new Axis(5);

	// Ryze/themilanfan - U 4
	// To replicate in another corner copy the same process but offset
	// the x & z to place in corner
	Cube *U4Cubes[] = {
		new Cube(-5, 0, 0),
		new Cube(-5, 1, 0),
		new Cube(-5, 2, 0),
		new Cube(-5, 3, 0),
		new Cube(-5, 4, 0),
		new Cube(-4, 0, 0),
		new Cube(-3, 0, 0),
		new Cube(-2, 0, 0),
		new Cube(-2, 1, 0),
		new Cube(-2, 2, 0),
		new Cube(-2, 3, 0),
		new Cube(-2, 4, 0),
		new Cube(2, 2, 0),
		new Cube(2, 3, 0),
		new Cube(2, 4, 0),
		new Cube(3, 2, 0),
		new Cube(4, 2, 0),
		new Cube(5, 0, 0),
		new Cube(5, 1, 0),
		new Cube(5, 2, 0),
		new Cube(5, 3, 0),
		new Cube(5, 4, 0)
	};

	Cube *E0Cubes[] = {
		// Draw E
		new Cube(-45, 0, -45),
		new Cube(-45, 1, -45),
		new Cube(-45, 2, -45),
		new Cube(-45, 3, -45),
		new Cube(-45, 4, -45),
		new Cube(-44, 0, -45),
		new Cube(-43, 0, -45),
		new Cube(-42, 0, -45),
		new Cube(-44, 2, -45),
		new Cube(-43, 2, -45),
		new Cube(-42, 2, -45),
		new Cube(-44, 4, -45),
		new Cube(-43, 4, -45),
		new Cube(-42, 4, -45),
		// Draw 0
		new Cube(-35, 0, -45),
		new Cube(-35, 1, -45),
		new Cube(-35, 2, -45),
		new Cube(-35, 3, -45),
		new Cube(-35, 4, -45),
		new Cube(-38, 0, -45),
		new Cube(-38, 1, -45),
		new Cube(-38, 2, -45),
		new Cube(-38, 3, -45),
		new Cube(-38, 4, -45),
		new Cube(-36, 4, -45),
		new Cube(-37, 4, -45),
		new Cube(-36, 0, -45),
		new Cube(-37, 0, -45)
	};

	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_LENGTH / (float)WINDOW_WIDTH, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cameraShader->setMat4("projection", projection);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(0.0f, 5.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cameraShader->setMat4("view", view);

		grid->draw(cameraShader);
		axis->draw(cameraShader);
		
		for (int i = 0; i < 22; i++) {
			U4Cubes[i]->draw(cameraShader, modelRenderMode);
		}

		for (int i = 0; i < 28; i++) {
			E0Cubes[i]->draw(cameraShader, modelRenderMode);
		}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	delete grid;
	delete basicShader;
	delete cameraShader;
	
	for (int i = 0; i < 22; i++) {
		delete U4Cubes[i];
	}

    glfwTerminate();
    return 0;
}