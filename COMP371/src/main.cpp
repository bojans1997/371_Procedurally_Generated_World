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
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float distance = 20.0f;
float camX = 0;
float camZ = 0;
float camY = 0;

void mouse_callback_horizontal(GLFWwindow* window, double xpos, double ypos);
void mouse_callback_vertical(GLFWwindow* window, double xpos, double ypos);
void mouse_callback_zoom(GLFWwindow* window, double xpos, double ypos);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = WINDOW_LENGTH/ 2.0;
float lastY = WINDOW_WIDTH / 2.0;
float fov = 45.0f;


void mouse_callback_horizontal(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	lastX = xpos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;

	yaw += xoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw));
	direction.y = cameraFront.y;
	direction.z = sin(glm::radians(yaw));
	cameraFront.x += direction.x;
	cameraFront.z += direction.z;
	
}

void mouse_callback_vertical(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront.y += direction.y;
}

void mouse_callback_zoom(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float yoffset = lastY - ypos;
	lastY = ypos;

	float sensitivity = 0.1f;
	yoffset *= sensitivity;

	fov -= yoffset;

	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

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
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
		cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
		fov = 45.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraPos = glm::vec3(sin(camX)*distance, cameraPos.y, cos(camZ)*distance);
		camX += 0.1f;
		camZ += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraPos = glm::vec3(sin(camX)*distance, cameraPos.y, cos(camZ)*distance);
		camX -= 0.1f;
		camZ -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos = glm::vec3(cameraPos.x, abs(sin(camY)*distance), cos(camZ)*distance);
		camY += 0.1f;
		camZ += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos = glm::vec3(cameraPos.x, abs(sin(camY)*distance), cos(camZ)*distance);
		camY -= 0.1f;
		camZ -= 0.1f;
	}
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
		glfwSetCursorPosCallback(window, NULL);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
		glfwSetCursorPosCallback(window, NULL);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
		glfwSetCursorPosCallback(window, NULL);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPosCallback(window, mouse_callback_horizontal);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPosCallback(window, mouse_callback_vertical);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPosCallback(window, mouse_callback_zoom);
	}
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
	//Letter E and digit 0 for Alexis Laurens-Renner
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
	//Letter J and digit 5 for Bojan Srbinoski
	Cube *J5Cubes[] = {
		// Draw J
		new Cube(34, 5, -45),
		new Cube(33, 5, -45),
		new Cube(32, 5, -45),
		new Cube(33, 4, -45),
		new Cube(33, 3, -45),
		new Cube(33, 2, -45),
		new Cube(33, 1, -45),
		new Cube(33, 0, -45),
		new Cube(32, 0, -45),
		new Cube(31, 0, -45),
		new Cube(31, 1, -45),
		// Draw 5
		new Cube(41, 5, -45),
		new Cube(40, 5, -45),
		new Cube(39, 5, -45),
		new Cube(38, 5, -45),
		new Cube(38, 4, -45),
		new Cube(38, 3, -45),
		new Cube(39, 3, -45),
		new Cube(40, 3, -45),
		new Cube(41, 3, -45),
		new Cube(41, 2, -45),
		new Cube(41, 1, -45),
		new Cube(41, 0, -45),
		new Cube(40, 0, -45),
		new Cube(39, 0, -45),
		new Cube(38, 0, -45),
	};

	//Letter A annd digit 6 for Saad Ahmed
	Cube *A6Cubes[] = {
		// Draw A
		new Cube(31, 5, 45),
		new Cube(32, 5, 45),
		new Cube(33, 5, 45),
		new Cube(34, 5, 45),
		new Cube(31, 4, 45),
		new Cube(31, 3, 45),
		new Cube(31, 2, 45),
		new Cube(31, 1, 45),
		new Cube(31, 0, 45),
		new Cube(34, 4, 45),
		new Cube(34, 3, 45),
		new Cube(34, 2, 45),
		new Cube(34, 1, 45),
		new Cube(34, 0, 45),
		new Cube(32, 3, 45),
		new Cube(33, 3, 45),
		// Draw 6
		new Cube(37, 5, 45),
		new Cube(38, 5, 45),
		new Cube(39, 5, 45),
		new Cube(40, 5, 45),
		new Cube(37, 4, 45),
		new Cube(37, 3, 45),
		new Cube(37, 2, 45),
		new Cube(37, 1, 45),
		new Cube(37, 0, 45),
		new Cube(38, 0, 45),
		new Cube(39, 0, 45),
		new Cube(40, 0, 45),
		new Cube(40, 1, 45),
		new Cube(40, 2, 45),
		new Cube(39, 2, 45),
		new Cube(38, 2, 45),
	};

	glEnable(GL_DEPTH_TEST);

	
	
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_LENGTH / (float)WINDOW_WIDTH, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);

		cameraShader->setMat4("projection", projection);
		cameraShader->setMat4("view", view);
		

		grid->draw(cameraShader);
		axis->draw(cameraShader);
		
		for (int i = 0; i < sizeof(U4Cubes) / sizeof(U4Cubes[0]); i++) {
			U4Cubes[i]->draw(cameraShader, modelRenderMode);
		}

		for (int i = 0; i < sizeof(E0Cubes) / sizeof(E0Cubes[0]); i++) {
			E0Cubes[i]->draw(cameraShader, modelRenderMode);
		}

		for (int i = 0; i < sizeof(J5Cubes) / sizeof(J5Cubes[0]); i++) {
			J5Cubes[i]->draw(cameraShader, modelRenderMode);
		}

		for (int i = 0; i < sizeof(A6Cubes) / sizeof(A6Cubes[0]); i++) {
			A6Cubes[i]->draw(cameraShader, modelRenderMode);
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