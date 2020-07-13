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
float lastX = WINDOW_LENGTH/ 2.0f;
float lastY = WINDOW_WIDTH / 2.0f;
float fov = 45.0f;

float angle = 0.0f;
float moveX, moveY = 0.0f;
float scale = 1.0f;

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

void mouse_callback_zoom(GLFWwindow* window, double xpos, double ypos)
{
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
	if (fov >= 90.0f)
		fov = 90.0f;
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

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && scale <= 3.0f)
		scale += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && scale >= 0.1f)
		scale -= 0.1f;

	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
		cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
		fov = 45.0f;
		angle, moveX, moveY = 0.0f;
		scale = 1.0f;
		angle = 0.0f;
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveY += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveY -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveX -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveX += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		angle += 5.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		angle -= 5.0f;

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

    GLuint VBO = 0, VAO = 0;

	Shader *shader = new Shader("src/shaders/shader.vs", "src/shaders/shader.fs");

	Grid *grid = new Grid(100);
	Axis *axis = new Axis(5);

	// Letter U and digit 4 for Giuseppe Campanelli
	Cube *U4Cubes[] = {
		// Draw U
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
		// Draw 4
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

	// Letter E and digit 5 for Alexis Laurens-Renner
	Cube *E5Cubes[] = {
		// Draw E
		new Cube(-5, 0, 0),
		new Cube(-5, 1, 0),
		new Cube(-5, 2, 0),
		new Cube(-5, 3, 0),
		new Cube(-5, 4, 0),
		new Cube(-4, 0, 0),
		new Cube(-3, 0, 0),
		new Cube(-2, 0, 0),
		new Cube(-4, 2, 0),
		new Cube(-3, 2, 0),
		new Cube(-2, 2, 0),
		new Cube(-4, 4, 0),
		new Cube(-3, 4, 0),
		new Cube(-2, 4, 0),
		// Draw 5
		new Cube(5, 0, 0),
		new Cube(5, 1, 0),
		new Cube(5, 2, 0),
		new Cube(5, 4, 0),
		new Cube(2, 0, 0),
		new Cube(2, 2, 0),
		new Cube(2, 3, 0),
		new Cube(2, 4, 0),
		new Cube(4, 4, 0),
		new Cube(3, 4, 0),
		new Cube(4, 0, 0),
		new Cube(3, 0, 0),
		new Cube(4, 2, 0),
		new Cube(3, 2, 0)
	};

	// Letter J and digit 5 for Bojan Srbinoski
	Cube *J5Cubes[] = {
		// Draw J
		new Cube(-2, 5, 0),
		new Cube(-3, 5, 0),
		new Cube(-4, 5, 0),
		new Cube(-3, 4, 0),
		new Cube(-3, 3, 0),
		new Cube(-3, 2, 0),
		new Cube(-3, 1, 0),
		new Cube(-3, 0, 0),
		new Cube(-4, 0, 0),
		new Cube(-5, 0, 0),
		new Cube(-5, 1, 0),
		// Draw 5
		new Cube(4, 5, 0),
		new Cube(3, 5, 0),
		new Cube(2, 5, 0),
		new Cube(1, 5, 0),
		new Cube(1, 4, 0),
		new Cube(1, 3, 0),
		new Cube(2, 3, 0),
		new Cube(3, 3, 0),
		new Cube(4, 3, 0),
		new Cube(4, 2, 0),
		new Cube(4, 1, 0),
		new Cube(4, 0, 0),
		new Cube(3, 0, 0),
		new Cube(2, 0, 0),
		new Cube(1, 0, 0)
	};

	// Letter A and digit 6 for Saad Ahmed
	Cube *A6Cubes[] = {
		// Draw A
		new Cube(-5, 5, 0),
		new Cube(-4, 5, 0),
		new Cube(-3, 5, 0),
		new Cube(-2, 5, 0),
		new Cube(-5, 4, 0),
		new Cube(-5, 3, 0),
		new Cube(-5, 2, 0),
		new Cube(-5, 1, 0),
		new Cube(-5, 0, 0),
		new Cube(-2, 4, 0),
		new Cube(-2, 3, 0),
		new Cube(-2, 2, 0),
		new Cube(-2, 1, 0),
		new Cube(-2, 0, 0),
		new Cube(-4, 3, 0),
		new Cube(-3, 3, 0),
		// Draw 6
		new Cube(1, 5, 0),
		new Cube(2, 5, 0),
		new Cube(3, 5, 0),
		new Cube(4, 5, 0),
		new Cube(1, 4, 0),
		new Cube(1, 3, 0),
		new Cube(1, 2, 0),
		new Cube(1, 1, 0),
		new Cube(1, 0, 0),
		new Cube(2, 0, 0),
		new Cube(3, 0, 0),
		new Cube(4, 0, 0),
		new Cube(4, 1, 0),
		new Cube(4, 2, 0),
		new Cube(3, 2, 0),
		new Cube(2, 2, 0)
	};

	// Letter N and digit 2 for Anna Kmieciak
	Cube *N2Cubes[] = {
		// Draw N
		new Cube(-5, 0, 0),
		new Cube(-5, 1, 0),
		new Cube(-5, 2, 0),
		new Cube(-5, 3, 0),
		new Cube(-5, 4, 0),
		new Cube(-4, 3, 0),
		new Cube(-3, 2, 0),
		new Cube(-2, 1, 0),
		new Cube(-1, 0, 0),
		new Cube(-1, 1, 0),
		new Cube(-1, 2, 0),
		new Cube(-1, 3, 0),
		new Cube(-1, 4, 0),
		// Draw 2
		new Cube(2,4,0),
		new Cube(3,4,0),
		new Cube(4,4,0),
		new Cube(5,4,0),
		new Cube(5,3,0),
		new Cube(2,2,0),
		new Cube(2,1,0),
		new Cube(3,2,0),
		new Cube(4,2,0),
		new Cube(5,2,0),
		new Cube(2,0,0),
		new Cube(3,0,0),
		new Cube(4,0,0),
		new Cube(5,0,0)
	};

	glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_LENGTH / (float)WINDOW_WIDTH, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);

		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		
		grid->draw(shader);
		axis->draw(shader);
		
		glm::mat4 modelU4 = glm::mat4(1.0f);
		modelU4 = glm::translate(modelU4, glm::vec3(moveX, moveY, 0.0f));
		modelU4 = glm::rotate(modelU4, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelU4 = glm::scale(modelU4, glm::vec3(scale, scale, scale));

		for (int i = 0; i < sizeof(U4Cubes) / sizeof(U4Cubes[0]); i++) {
			U4Cubes[i]->draw(shader, modelRenderMode, modelU4);
		}

		glm::mat4 modelE5 = glm::mat4(1.0f);
		modelE5 = glm::translate(modelE5, glm::vec3(moveX - 40, moveY, 0.0f - 45));
		modelE5 = glm::rotate(modelE5, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelE5 = glm::scale(modelE5, glm::vec3(scale, scale, scale));

		for (int i = 0; i < sizeof(E5Cubes) / sizeof(E5Cubes[0]); i++) {
			E5Cubes[i]->draw(shader, modelRenderMode, modelE5);
		}

		glm::mat4 modelJ5 = glm::mat4(1.0f);
		modelJ5 = glm::translate(modelJ5, glm::vec3(moveX + 40, moveY, 0.0f - 45));
		modelJ5 = glm::rotate(modelJ5, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelJ5 = glm::scale(modelJ5, glm::vec3(scale, scale, scale));

		for (int i = 0; i < sizeof(J5Cubes) / sizeof(J5Cubes[0]); i++) {
			J5Cubes[i]->draw(shader, modelRenderMode, modelJ5);
		}

		glm::mat4 modelA6 = glm::mat4(1.0f);
		modelA6 = glm::translate(modelA6, glm::vec3(moveX + 40, moveY, 0.0f + 45));
		modelA6 = glm::rotate(modelA6, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelA6 = glm::scale(modelA6, glm::vec3(scale, scale, scale));

		for (int i = 0; i < sizeof(A6Cubes) / sizeof(A6Cubes[0]); i++) {
			A6Cubes[i]->draw(shader, modelRenderMode, modelA6);
		}

		glm::mat4 modelN2 = glm::mat4(1.0f);
		modelN2 = glm::translate(modelN2, glm::vec3(moveX-40, moveY, 0.0f + 45));
		modelN2 = glm::rotate(modelN2, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelN2 = glm::scale(modelN2, glm::vec3(scale, scale, scale));

		for (int i = 0; i < sizeof(N2Cubes) / sizeof(N2Cubes[0]); i++) {
			N2Cubes[i]->draw(shader, modelRenderMode, modelN2);
		}

		if (angle == 360.0f)
			angle = 0.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	for (int i = 0; i < sizeof(U4Cubes) / sizeof(U4Cubes[0]); i++) {
		delete U4Cubes[i];
	}

	for (int i = 0; i < sizeof(E5Cubes) / sizeof(E5Cubes[0]); i++) {
		delete E5Cubes[i];
	}

	for (int i = 0; i < sizeof(J5Cubes) / sizeof(J5Cubes[0]); i++) {
		delete J5Cubes[i];
	}

	for (int i = 0; i < sizeof(A6Cubes) / sizeof(A6Cubes[0]); i++) {
		delete A6Cubes[i];
	}

	for (int i = 0; i < sizeof(N2Cubes) / sizeof(N2Cubes[0]); i++) {
		delete N2Cubes[i];
	}

	delete axis;
	delete grid;
	delete shader;

    glfwTerminate();

    return 0;
}