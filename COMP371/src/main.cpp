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
#include "objects/character.h"
#include "objects/pair.h"
#include "objects/Sphere.h"
#include "objects/Stack.h"
#include "objects/Sector.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WINDOW_LENGTH = 1024;
const int WINDOW_WIDTH = 768;

const int GRID_SIZE = 100;
const int AXIS_SIZE = 5;

GLFWwindow* window;

glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat distance = 20.0f;
GLfloat camX = 0;
GLfloat camZ = 0;
GLfloat camY = 0;

bool firstMouse = true;
GLfloat yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLdouble lastX = WINDOW_LENGTH/ 2.0;
GLdouble lastY = WINDOW_WIDTH / 2.0;
GLfloat fov = 45.0f;

GLuint modelRenderMode = GL_TRIANGLES;

GLfloat angle = 0.0f;
GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;
GLfloat scale = 1.0f;

bool textures = true;

glm::vec3 pairU4Pos = glm::vec3(0, 0, 0);
glm::vec3 pairE5Pos = glm::vec3(-40, 0, -45);
glm::vec3 pairJ5Pos = glm::vec3(40, 0, -45);
glm::vec3 pairA6Pos = glm::vec3(40, 0, 45);
glm::vec3 pairN2Pos = glm::vec3(-40, 0, 45);

void mouse_callback_horizontal(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLdouble xoffset = xpos - lastX;
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

void mouse_callback_vertical(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLdouble xoffset = xpos - lastX;
	GLdouble yoffset = lastY - ypos;
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

void mouse_callback_zoom(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLdouble yoffset = lastY - ypos;
	lastY = ypos;

	float sensitivity = 0.1f;
	yoffset *= sensitivity;

	fov -= yoffset;

	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 90.0f)
		fov = 90.0f;
}

glm::vec3 generateRandomGridPosition() {
	int min = -GRID_SIZE/2, max = GRID_SIZE/2;
	int x = min + (rand() % static_cast<int>(max - min + 1));
	int z = min + (rand() % static_cast<int>(max - min + 1));
	return glm::vec3(x, 0, z);
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
		angle = 0.0f;
		moveX = 0.0f;
		moveY = 0.0f;
		moveZ = 0.0f;
		scale = 1.0f;
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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		pairU4Pos = generateRandomGridPosition();
		pairE5Pos = generateRandomGridPosition();
		pairJ5Pos = generateRandomGridPosition();
		pairA6Pos = generateRandomGridPosition();
		pairN2Pos = generateRandomGridPosition();
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveY += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveY -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveX -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveX += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		moveZ -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		moveZ += 1.0f;
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

	Shader *basicShader = new Shader("src/shaders/shader.vs", "src/shaders/shader.fs");
	Shader *shader = new Shader("src/shaders/light.vs", "src/shaders/light.fs");
	Shader *textureShader = new Shader("src/shaders/texture.vs", "src/shaders/texture.fs");
	Shader *lightShader = new Shader("src/shaders/lightCube.vs", "src/shaders/lightCube.fs");
	Texture *tileTexture = new Texture("src/textures/tile.jpg");
	Texture *woodTexture = new Texture("src/textures/wood.jpg");
	Texture *goldTexture = new Texture("src/textures/gold.jpg");

	Grid *grid = new Grid(GRID_SIZE);
	Axis *axis = new Axis(AXIS_SIZE);

	Cube *lightSource = new Cube(0, 30, 0);
	

	// Letter U and digit 4 for Giuseppe Campanelli
	std::vector<Cube*> cubesU = {
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
		new Cube(-2, 4, 0)
	};
	std::vector<Cube*> cubes4 = {
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
	Pair *pairU4 = new Pair(new Character(cubesU), new Character(cubes4));

	// Letter E and digit 5 for Alexis Laurens-Renner
	std::vector<Cube*> cubesE = {
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
		new Cube(-2, 4, 0)
	};
	std::vector<Cube*> cubes5_1 = {
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
	Pair *pairE5 = new Pair(new Character(cubesE), new Character(cubes5_1));

	// Letter J and digit 5 for Bojan Srbinoski
	std::vector<Cube*> cubesJ = {
		new Cube(-2, 4, 0),
		new Cube(-3, 4, 0),
		new Cube(-4, 4, 0),
		new Cube(-3, 3, 0),
		new Cube(-3, 2, 0),
		new Cube(-3, 1, 0),
		new Cube(-3, 0, 0),
		new Cube(-4, 0, 0),
		new Cube(-5, 0, 0),
		new Cube(-5, 1, 0)
	};
	std::vector<Cube*> cubes5_2 = {
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
	Pair *pairJ5 = new Pair(new Character(cubesJ), new Character(cubes5_2));

	// Letter A and digit 6 for Saad Ahmed
	std::vector<Cube*> cubesA = {
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
		new Cube(-3, 3, 0)
	};
	std::vector<Cube*> cubes6 = {
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
	Pair *pairA6 = new Pair(new Character(cubesA), new Character(cubes6));

	// Letter N and digit 2 for Anna Kmieciak
	std::vector<Cube*> cubesN = {
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
		new Cube(-1, 4, 0)
	};
	std::vector<Cube*> cubes2 = {
		new Cube(2, 4, 0),
		new Cube(3, 4, 0),
		new Cube(4, 4, 0),
		new Cube(5, 4, 0),
		new Cube(5, 3, 0),
		new Cube(2, 2, 0),
		new Cube(2, 1, 0),
		new Cube(3, 2, 0),
		new Cube(4, 2, 0),
		new Cube(5, 2, 0),
		new Cube(2, 0, 0),
		new Cube(3, 0, 0),
		new Cube(4, 0, 0),
		new Cube(5, 0, 0)
	};
	Pair *pairN2 = new Pair(new Character(cubesN), new Character(cubes2));

	glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_LENGTH / (float)WINDOW_WIDTH, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);

		basicShader->use();
		basicShader->setMat4("projection", projection);
		basicShader->setMat4("view", view);
		axis->draw(basicShader);

		glm::mat4 lightModel(1.0f);
		lightShader->use();
		lightShader->setMat4("projection", projection);
		lightShader->setMat4("view", view);
		lightSource->draw(lightShader, modelRenderMode, lightModel);

		textureShader->use();
		textureShader->setMat4("projection", projection);
		textureShader->setMat4("view", view);
		textureShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		textureShader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
		textureShader->setVec3("viewPos", cameraPos);
		grid->draw(textureShader, tileTexture);
		
		shader->use();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shader->setVec3("lightPos", glm::vec3(0.0f, 30.0f, 0.0f));
		shader->setVec3("viewPos", cameraPos);

		glm::mat4 modelU4 = glm::mat4(1.0f);
		modelU4 = glm::translate(modelU4, glm::vec3(pairU4Pos.x + moveX, pairU4Pos.y + moveY, pairU4Pos.z + moveZ));
		modelU4 = glm::rotate(modelU4, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelU4 = glm::scale(modelU4, glm::vec3(scale, scale, scale));

		if (textures) {
			pairU4->draw(textureShader, modelRenderMode, modelU4, woodTexture, goldTexture);
		} else {
			pairU4->draw(shader, modelRenderMode, modelU4);
		}

		glm::mat4 modelE5 = glm::mat4(1.0f);
		modelE5 = glm::translate(modelE5, glm::vec3(pairE5Pos.x + moveX, pairE5Pos.y + moveY, pairE5Pos.z + moveZ));
		modelE5 = glm::rotate(modelE5, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelE5 = glm::scale(modelE5, glm::vec3(scale, scale, scale));

		if (textures) {
			pairE5->draw(textureShader, modelRenderMode, modelE5, woodTexture, goldTexture);
		}
		else {
			pairE5->draw(shader, modelRenderMode, modelE5);
		}

		glm::mat4 modelJ5 = glm::mat4(1.0f);
		modelJ5 = glm::translate(modelJ5, glm::vec3(pairJ5Pos.x + moveX, pairJ5Pos.y + moveY, pairJ5Pos.z + moveZ));
		modelJ5 = glm::rotate(modelJ5, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelJ5 = glm::scale(modelJ5, glm::vec3(scale, scale, scale));

		if (textures) {
			pairJ5->draw(textureShader, modelRenderMode, modelJ5, woodTexture, goldTexture);
		}
		else {
			pairJ5->draw(shader, modelRenderMode, modelJ5);
		}

		glm::mat4 modelA6 = glm::mat4(1.0f);
		modelA6 = glm::translate(modelA6, glm::vec3(pairA6Pos.x + moveX, pairA6Pos.y + moveY, pairA6Pos.z + moveZ));
		modelA6 = glm::rotate(modelA6, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelA6 = glm::scale(modelA6, glm::vec3(scale, scale, scale));

		if (textures) {
			pairA6->draw(textureShader, modelRenderMode, modelA6, woodTexture, goldTexture);
		}
		else {
			pairA6->draw(shader, modelRenderMode, modelA6);
		}

		glm::mat4 modelN2 = glm::mat4(1.0f);
		modelN2 = glm::translate(modelN2, glm::vec3(pairN2Pos.x + moveX, pairN2Pos.y + moveY, pairN2Pos.z + moveZ));
		modelN2 = glm::rotate(modelN2, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelN2 = glm::scale(modelN2, glm::vec3(scale, scale, scale));

		if (textures) {
			pairN2->draw(textureShader, modelRenderMode, modelN2, woodTexture, goldTexture);
		}
		else {
			pairN2->draw(shader, modelRenderMode, modelN2);
		}

		if (angle == 360.0f)
			angle = 0.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	delete pairU4;
	delete pairE5;
	delete pairJ5;
	delete pairA6;
	delete pairN2;
	delete axis;
	delete grid;
	delete tileTexture;
	delete woodTexture;
	delete goldTexture;
	delete textureShader;
	delete shader;

    glfwTerminate();

    return 0;
}