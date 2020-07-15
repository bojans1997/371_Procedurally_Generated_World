#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "objects/shader.h"
#include "objects/grid.h"
#include "objects/axis.h"
#include "objects/cube.h"
#include "objects/texture.h"
#include "objects/characters/character.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WINDOW_LENGTH = 1024;
const int WINDOW_WIDTH = 768;

GLFWwindow* window;

GLuint modelRenderMode = GL_TRIANGLES;
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat distance = 20.0f;
GLfloat camX = 0;
GLfloat camZ = 0;
GLfloat camY = 0;

void mouse_callback_horizontal(GLFWwindow* window, double xpos, double ypos);
void mouse_callback_vertical(GLFWwindow* window, double xpos, double ypos);
void mouse_callback_zoom(GLFWwindow* window, double xpos, double ypos);

bool firstMouse = true;
GLfloat yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WINDOW_LENGTH/ 2.0f;
GLfloat lastY = WINDOW_WIDTH / 2.0f;
GLfloat fov = 45.0f;

GLfloat angle = 0.0f;
GLfloat moveX, moveY = 0.0f;
GLfloat scale = 1.0f;

bool textures = true;

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

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		textures = !textures;

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
	Shader *textureShader = new Shader("src/shaders/texture.vs", "src/shaders/texture.fs");

	Texture *tileTexture = new Texture("src/textures/tile.jpg");
	Texture *woodTexture = new Texture("src/textures/wood.jpg");
	Texture *goldTexture = new Texture("src/textures/gold.jpg");

	Grid grid = Grid(100);
	Axis axis = Axis(5);

	// Letter U and digit 4 for Giuseppe Campanelli
	std::vector<Cube> cubesU = {
		Cube(-5, 0, 0),
		Cube(-5, 1, 0),
		Cube(-5, 2, 0),
		Cube(-5, 3, 0),
		Cube(-5, 4, 0),
		Cube(-4, 0, 0),
		Cube(-3, 0, 0),
		Cube(-2, 0, 0),
		Cube(-2, 1, 0),
		Cube(-2, 2, 0),
		Cube(-2, 3, 0),
		Cube(-2, 4, 0)
	};
	std::vector<Cube> cubes4 = {
		Cube(2, 2, 0),
		Cube(2, 3, 0),
		Cube(2, 4, 0),
		Cube(3, 2, 0),
		Cube(4, 2, 0),
		Cube(5, 0, 0),
		Cube(5, 1, 0),
		Cube(5, 2, 0),
		Cube(5, 3, 0),
		Cube(5, 4, 0)
	};

	Character letterU = Character(cubesU);
	Character digit4 = Character(cubes4);

	glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_LENGTH / (float)WINDOW_WIDTH, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);

		shader->use();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		axis.draw(shader);

		textureShader->use();
		textureShader->setMat4("projection", projection);
		textureShader->setMat4("view", view);
		grid.draw(textureShader, tileTexture);
		
		glm::mat4 modelU4 = glm::mat4(1.0f);
		modelU4 = glm::translate(modelU4, glm::vec3(moveX, moveY, 0.0f));
		modelU4 = glm::rotate(modelU4, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelU4 = glm::scale(modelU4, glm::vec3(scale, scale, scale));

		if (textures) {
			letterU.draw(textureShader, modelRenderMode, modelU4, woodTexture);
			digit4.draw(textureShader, modelRenderMode, modelU4, goldTexture);
		} else {
			letterU.draw(shader, modelRenderMode, modelU4);
			digit4.draw(shader, modelRenderMode, modelU4);
		}

		if (angle == 360.0f)
			angle = 0.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	delete shader;
	delete textureShader;
	delete tileTexture;
	delete woodTexture;
	delete goldTexture;

    glfwTerminate();

    return 0;
}