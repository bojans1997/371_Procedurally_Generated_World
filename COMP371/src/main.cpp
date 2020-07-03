#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include "objects/shader.h"
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

	Shader *basicShader = new Shader("src/shaders/shader.vs", "src/shaders/shader.fs");
	Shader *cameraShader = new Shader("src/shaders/camera.vs", "src/shaders/shader.fs");

	Grid *grid = new Grid(100);
	Axis *axis = new Axis(5);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_LENGTH / (float)WINDOW_WIDTH, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		cameraShader->setMat4("projection", projection);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(0.0f, 5.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cameraShader->setMat4("view", view);

		grid->draw(cameraShader);
		axis->draw(cameraShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	delete grid;
	delete basicShader;
	delete cameraShader;
    glfwTerminate();
    return 0;
}