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
#include "objects/tree.h"
#include "objects/bush.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WINDOW_LENGTH = 1024;
const int WINDOW_WIDTH = 768;

int GRID_SIZE = 100;
int AXIS_SIZE = 5;

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
GLfloat fov = 60.0f;

GLfloat angle = 0.0f;
GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;

glm::mat4 shear(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

//glm::vec3 pairU4Pos = glm::vec3(0, 0, 0);

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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
		cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
		fov = 45.0f;
		angle = 0.0f;
		moveX = 0.0f;
		moveY = 0.0f;
		moveZ = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z - 0.2f);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z + 0.2f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos = glm::vec3(cameraPos.x - 0.2f, cameraPos.y, cameraPos.z);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos = glm::vec3(cameraPos.x + 0.2f, cameraPos.y, cameraPos.z);
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

    window = glfwCreateWindow(WINDOW_LENGTH, WINDOW_WIDTH, "COMP 371 Project - OpenGLHF", NULL, NULL);

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
	Shader *textureShader = new Shader("src/shaders/texture.vs", "src/shaders/texture.fs");
	Shader *lightShader = new Shader("src/shaders/lightCube.vs", "src/shaders/lightCube.fs");
	Shader *depthShader = new Shader("src/shaders/depthShader.vs", "src/shaders/depthShader.fs");
	Shader *gridShader = new Shader("src/shaders/gridShader.vs","src/shaders/gridShader.fs");

	Grid *grid = new Grid(GRID_SIZE);
	Axis *axis = new Axis(AXIS_SIZE);

	Cube *lightSource = new Cube(0, 30, 0);

	std::vector<Tree*> trees;

	srand(time(NULL));

	for (int i = 0; i < 50; i++) {
		int x = -(GRID_SIZE / 2) + (rand() % (GRID_SIZE - -(GRID_SIZE / 2) + 1));
		int z = -(GRID_SIZE / 2) + (rand() % (GRID_SIZE - -(GRID_SIZE / 2) + 1));
		int size = 3 + (rand() % (6 - 3 + 1));
		trees.push_back(new Tree(glm::vec3(x, 0, z), size));
	}

	std::vector<Bush*> bushes;

	for (int i = 0; i < 100; i++) {
		int x = -40 + (rand() % (80 - -40 + 1));
		int z = -40 + (rand() % (80 - -40 + 1));
		int xLen = 2 + (rand() % (4 - 2 + 1));
		int yLen = 2 + (rand() % (4 - 2 + 1));
		int zLen = 2 + (rand() % (4 - 2 + 1));
		bushes.push_back(new Bush(glm::vec3(x, 0, z), glm::vec3(xLen, yLen, zLen)));
	}
	
	glEnable(GL_DEPTH_TEST);

	// configure depth map FBO
	// -----------------------
	const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float bordercolor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// shader configuration
	gridShader->use();
	gridShader->setInt("shadowMap", 1);
	gridShader->setInt("diffuseTexture", 0);

	// lighting info
	glm::vec3 lightPosition(0.0f, 30.0f, -5.0f);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Procedurally grow terrain and creates objects
		if (cameraPos.x < -GRID_SIZE / 2 || cameraPos.x > GRID_SIZE / 2 || 
			cameraPos.z < -GRID_SIZE / 2 || cameraPos.z > GRID_SIZE / 2) {
			GRID_SIZE += 50;
			grid->setSize(GRID_SIZE);

			for (int i = 0; i < 25; i++) {
				int x = -(GRID_SIZE / 2) + (rand() % ((GRID_SIZE / 2) - -(GRID_SIZE / 2) + 1));
				int z = ((GRID_SIZE - 50) / 2) + (rand() % ((GRID_SIZE / 2) - ((GRID_SIZE - 50) / 2) + 1));
				int size = 3 + (rand() % (6 - 3 + 1));

				if ((rand() % 100) > 50) {
					z = -z;
				}

				trees.push_back(new Tree(glm::vec3(x, 0, z), size));
			}

			for (int i = 0; i < 25; i++) {
				int x = ((GRID_SIZE - 50) / 2) + (rand() % ((GRID_SIZE / 2) - ((GRID_SIZE - 50) / 2) + 1));
				int z = -(GRID_SIZE / 2) + (rand() % ((GRID_SIZE / 2) - -(GRID_SIZE / 2) + 1));
				int size = 3 + (rand() % (6 - 3 + 1));

				if ((rand() % 100) > 50) {
					x = -x;
				}

				trees.push_back(new Tree(glm::vec3(x, 0, z), size));
			}

			for (int i = 0; i < 50; i++) {
				int x = -(GRID_SIZE / 2) + (rand() % ((GRID_SIZE / 2) - -(GRID_SIZE / 2) + 1));
				int z = ((GRID_SIZE - 50) / 2) + (rand() % ((GRID_SIZE / 2) - ((GRID_SIZE - 50) / 2) + 1));
				int xLen = 2 + (rand() % (4 - 2 + 1));
				int yLen = 2 + (rand() % (4 - 2 + 1));
				int zLen = 2 + (rand() % (4 - 2 + 1));

				if (rand() % 100 >= 50) {
					z = -z;
				}

				bushes.push_back(new Bush(glm::vec3(x, 0, z), glm::vec3(xLen, yLen, zLen)));
			}

			for (int i = 0; i < 50; i++) {
				int x = ((GRID_SIZE - 50) / 2) + (rand() % ((GRID_SIZE / 2) - ((GRID_SIZE - 50) / 2) + 1));
				int z = -(GRID_SIZE / 2) + (rand() % ((GRID_SIZE / 2) - -(GRID_SIZE / 2) + 1));
				int xLen = 2 + (rand() % (4 - 2 + 1));
				int yLen = 2 + (rand() % (4 - 2 + 1));
				int zLen = 2 + (rand() % (4 - 2 + 1));

				if (rand() % 100 >= 50) {
					x = -x;
				}

				bushes.push_back(new Bush(glm::vec3(x, 0, z), glm::vec3(xLen, yLen, zLen)));
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glm::mat4 modelU4 = glm::mat4(1.0f);
		modelU4 = glm::translate(modelU4, glm::vec3(pairU4Pos.x + moveX, pairU4Pos.y + moveY, pairU4Pos.z + moveZ));
		modelU4 = glm::rotate(modelU4, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		modelU4 = glm::scale(modelU4, glm::vec3(scale, scale, scale));
		modelU4 = modelU4 * shear;*/

		//Shadow Pass 1 - Shadow Map
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 30.5f;
		lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		depthShader->use();
		depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		grid->draw(depthShader);

		for (std::vector<Tree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
			(*it)->draw(depthShader);
		}

		for (std::vector<Bush*>::iterator it = bushes.begin(); it != bushes.end(); ++it) {
			(*it)->draw(depthShader);
		}

		// reset viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_LENGTH, WINDOW_WIDTH);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Shadow Pass 2 - Normal Render
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_LENGTH / (float)WINDOW_WIDTH, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);

		//Draw axis
		basicShader->use();
		basicShader->setMat4("projection", projection);
		basicShader->setMat4("view", view);
		axis->draw(basicShader);

		//Draw light cube
		glm::mat4 lightModel = lightSource->getModel();
		lightShader->use();
		lightShader->setMat4("projection", projection);
		lightShader->setMat4("view", view);
		lightSource->draw(lightShader, lightModel);

		//Light For Texture
		textureShader->use();
		textureShader->setMat4("projection", projection);
		textureShader->setMat4("view", view);
		// set light uniforms
		textureShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		textureShader->setVec3("lightPos", lightPosition);
		textureShader->setVec3("viewPos", cameraPos);
		textureShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		textureShader->setVec3("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		textureShader->setVec3("material.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		textureShader->setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
		textureShader->setFloat("material.shininess", 0.3f);
		
		gridShader->use();
		gridShader->setMat4("projection", projection);
		gridShader->setMat4("view", view);
		// set light uniforms
		gridShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		gridShader->setVec3("lightPos", lightPosition);
		gridShader->setVec3("viewPos", cameraPos);
		gridShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//Draw Grid
		grid->draw(gridShader, depthMap);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		//Draw Models with Texture
		for (std::vector<Tree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
			(*it)->draw(textureShader);
		}

		for (std::vector<Bush*>::iterator it = bushes.begin(); it != bushes.end(); ++it) {
			(*it)->draw(textureShader);
		}

		if (angle == 360.0f)
			angle = 0.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	for (std::vector<Tree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
		delete *it;
	}

	for (std::vector<Bush*>::iterator it = bushes.begin(); it != bushes.end(); ++it) {
		delete *it;
	}

	delete axis;
	delete grid;
	delete gridShader;
	delete depthShader;
	delete textureShader;
	delete lightShader;
	delete basicShader;

    glfwTerminate();

    return 0;
}