#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <irrKlang.h>
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
#include "objects/tree.h"
#include "objects/bush.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WINDOW_LENGTH = 1920;
const int WINDOW_HEIGHT = 1080;

int user_win_length = 1600;
int user_win_height = 900;
int user_win_xpos = 150;
int user_win_ypos = 100;

int GRID_SIZE = 100;
int AXIS_SIZE = 5;

int MODEL_SCALE = 2.0f;

GLFWwindow* window;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool is_fs = false;


bool is_jumping = false;
float jumpValue = 1.0f;
bool Jumpdown = false;
const float JUMPSPEED = 0.5f;
const float JUMPMAXHEIGHT = 2.5f;
GLfloat distance = 20.0f;
GLfloat camX = 0;
GLfloat camZ = 0;
GLfloat camY = 0;


bool firstMouse = true;
GLfloat yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLdouble lastX = WINDOW_LENGTH / 2.0;
GLdouble lastY = WINDOW_HEIGHT / 2.0;
GLfloat fov = 50.0f;

glm::vec3 pairU4Pos = glm::vec3(0, 0, 0);
glm::vec3 pairE5Pos = glm::vec3(-40, 0, -45);
glm::vec3 pairJ5Pos = glm::vec3(40, 0, -45);
glm::vec3 pairA6Pos = glm::vec3(40, 0, 45);
glm::vec3 pairN2Pos = glm::vec3(-40, 0, 45);

Pair *pairU4;
Pair *pairE5;
Pair *pairJ5;
Pair *pairA6;
Pair *pairN2;

// Initial procedural creation of objects on terrain
std::vector<Tree*> trees;
std::vector<Bush*> bushes;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float cameraSpeed = 10.0 * deltaTime;

/*
	Using irrKlang for music and sound effects
	Downloaded from https://www.ambiera.com/irrklang/downloads.html
	Code reference: https://learnopengl.com/In-Practice/2D-Game/Audio
	Music: https://www.youtube.com/watch?v=Lmi2kLH9aL8
	Footstep: https://www.fesliyanstudios.com/play-mp3/6985
*/
irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();
irrklang::ISound *music = SoundEngine->play2D("src/resources/music.mp3", true, true);
irrklang::ISound *footstep = SoundEngine->play2D("src/resources/footstep.mp3", true, true);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

int randomInt(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

void generateObjects(int min1, int max1, int min2, int max2) {
	for (int i = 0; i < 25; i++) {
		int x = randomInt(min1, max1);
		int z = randomInt(min2, max2);
		int size = randomInt(3, 6);

		if ((rand() % 100) > 50) {
			z = -z;
		}

		trees.push_back(new Tree(glm::vec3(x, 0, z), size));
	}

	for (int i = 0; i < 25; i++) {
		int x = randomInt(min2, max2);
		int z = randomInt(min1, max1);
		int size = randomInt(3, 6);

		if ((rand() % 100) > 50) {
			x = -x;
		}

		trees.push_back(new Tree(glm::vec3(x, 0, z), size));
	}

	for (int i = 0; i < 50; i++) {
		int x = randomInt(min1, max1);
		int z = randomInt(min2, max2);
		int xLen = randomInt(2, 4);
		int yLen = randomInt(2, 4);
		int zLen = randomInt(2, 4);

		if (rand() % 100 >= 50) {
			z = -z;
		}

		bushes.push_back(new Bush(glm::vec3(x, 0, z), glm::vec3(xLen, yLen, zLen)));
	}

	for (int i = 0; i < 50; i++) {
		int x = randomInt(min2, max2);
		int z = randomInt(min1, max1);
		int xLen = randomInt(2, 4);
		int yLen = randomInt(2, 4);
		int zLen = randomInt(2, 4);

		if (rand() % 100 >= 50) {
			x = -x;
		}

		bushes.push_back(new Bush(glm::vec3(x, 0, z), glm::vec3(xLen, yLen, zLen)));
	}
}

bool checkCharacterCollision(Character *character, glm::vec3 pairPos, glm::vec3 newCameraPos)
{
	std::vector<Cube*> charCubes = character->getCubes();

	for (std::vector<Cube*>::iterator it = charCubes.begin(); it != charCubes.end(); ++it) {
		glm::vec3 pos = (*it)->position;

		if (pos.y <= 1) {
			bool collisionX = newCameraPos.x + 0.1f >= pos.x * MODEL_SCALE + pairPos.x
				&& newCameraPos.x - 0.1f <= pos.x * MODEL_SCALE + +pairPos.x + MODEL_SCALE;
			bool collisionZ = newCameraPos.z + 0.1f >= pos.z * MODEL_SCALE + pairPos.z
				&& newCameraPos.z - 0.1f <= pos.z * MODEL_SCALE + pairPos.z + MODEL_SCALE;

			if (collisionX && collisionZ) {
				return true;
			}
		}
	}

	return false;
}

bool checkPairCollision(Pair *pair, glm::vec3 pairPos, glm::vec3 newCameraPos)
{
	if (pair != NULL) {
		return checkCharacterCollision(pair->getLetter(), pairPos, newCameraPos)
			|| checkCharacterCollision(pair->getDigit(), pairPos, newCameraPos);
	}

	return false;
}

bool checkCollision(glm::vec3 newCameraPos)
{
	for (std::vector<Tree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
		glm::vec3 cubeCenter = glm::vec3((*it)->position.x + 0.5f, (*it)->position.y, (*it)->position.z + 0.5f);
		if (glm::distance(cubeCenter, newCameraPos) <= 2.2f) {
			return true;
		}
	}

	for (std::vector<Bush*>::iterator it = bushes.begin(); it != bushes.end(); ++it) {
		glm::vec3 pos = (*it)->position;
		glm::vec3 size = (*it)->size;

		bool collisionX = newCameraPos.x + 0.1f >= pos.x && newCameraPos.x - 0.1f <= pos.x + size.x;
		bool collisionZ = newCameraPos.z + 0.1f >= pos.z && newCameraPos.z - 0.1f <= pos.z + size.z;

		if (collisionX && collisionZ) {
			return true;
		}
	}

	bool modelCollision = checkPairCollision(pairU4, pairU4Pos, newCameraPos)
		|| checkPairCollision(pairE5, pairE5Pos, newCameraPos) || checkPairCollision(pairJ5, pairJ5Pos, newCameraPos)
		|| checkPairCollision(pairA6, pairA6Pos, newCameraPos) || checkPairCollision(pairN2, pairN2Pos, newCameraPos);

	return modelCollision;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		cameraSpeed = 10.0 * deltaTime;
		footstep->setPlaybackSpeed(1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraSpeed = 20.0 * deltaTime;
		footstep->setPlaybackSpeed(1.2f);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		cameraPos = glm::vec3(0.0f, 2.0f, 20.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		fov = 50.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos + cameraSpeed * cameraFront;
		newCameraPos.y = cameraPos.y;
		if (!checkCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos - cameraSpeed * cameraFront;
		newCameraPos.y = cameraPos.y;
		if (!checkCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos - glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (!checkCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (!checkCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) {
		footstep->setIsPaused(true);
	}

	//Jump
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			is_jumping = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
		is_fs = !is_fs;
		glfwSetWindowMonitor(window, is_fs ? glfwGetPrimaryMonitor() : NULL, is_fs ? 0 : user_win_xpos, is_fs ? 0 : user_win_ypos, is_fs ? WINDOW_LENGTH : user_win_length, is_fs ? WINDOW_HEIGHT : user_win_height, GLFW_DONT_CARE);
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
	//glfwGetPrimaryMonitor() for fullscreen
	window = glfwCreateWindow(user_win_length, user_win_height, "COMP 371 Project - OpenGLHF", NULL, NULL);
	glfwSetWindowPos(window, user_win_xpos, user_win_ypos);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	music->setIsPaused(false);
	music->setVolume(0.6f);
	footstep->setVolume(1.0f);

	GLuint VBO = 0, VAO = 0;

	Shader *basicShader = new Shader("src/shaders/shader.vs", "src/shaders/shader.fs");
	Shader *textureShader = new Shader("src/shaders/texture.vs", "src/shaders/texture.fs");
	Shader *lightShader = new Shader("src/shaders/lightCube.vs", "src/shaders/lightCube.fs");
	Shader *depthShader = new Shader("src/shaders/depthShader.vs", "src/shaders/depthShader.fs");
	Shader *sphereShader = new Shader("src/shaders/SphereShader.vs", "src/shaders/SphereShader.fs");
	Shader *gridShader = new Shader("src/shaders/gridShader.vs", "src/shaders/gridShader.fs");
	Shader *skyBoxShader = new Shader("src/shaders/skybox.vs", "src/shaders/skybox.fs");

	// Referenced from https://freestocktextures.com/texture/wall-moss-brick,621.html
	Texture *ruinTexture = new Texture("src/textures/ruin.jpg");

	Grid *grid = new Grid(GRID_SIZE);
	
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
		new Cube(1, 2, 0),
		new Cube(1, 3, 0),
		new Cube(1, 4, 0),
		new Cube(2, 2, 0),
		new Cube(3, 2, 0),
		new Cube(4, 0, 0),
		new Cube(4, 1, 0),
		new Cube(4, 2, 0),
		new Cube(4, 3, 0),
		new Cube(4, 4, 0)
	};
	pairU4 = new Pair(new Character(cubesU), new Character(cubes4), new Sphere(0, 6, 0, 5, 10, 10));

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
		new Cube(4, 0, 0),
		new Cube(4, 1, 0),
		new Cube(4, 2, 0),
		new Cube(4, 4, 0),
		new Cube(1, 0, 0),
		new Cube(1, 2, 0),
		new Cube(1, 3, 0),
		new Cube(1, 4, 0),
		new Cube(3, 4, 0),
		new Cube(2, 4, 0),
		new Cube(3, 0, 0),
		new Cube(2, 0, 0),
		new Cube(3, 2, 0),
		new Cube(2, 2, 0)
	};
	pairE5 = new Pair(new Character(cubesE), new Character(cubes5_1), new Sphere(0, 6, 0, 5, 10, 10));

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
		new Cube(4, 0, 0),
		new Cube(4, 1, 0),
		new Cube(4, 2, 0),
		new Cube(4, 4, 0),
		new Cube(1, 0, 0),
		new Cube(1, 2, 0),
		new Cube(1, 3, 0),
		new Cube(1, 4, 0),
		new Cube(3, 4, 0),
		new Cube(2, 4, 0),
		new Cube(3, 0, 0),
		new Cube(2, 0, 0),
		new Cube(3, 2, 0),
		new Cube(2, 2, 0)
	};
	pairJ5 = new Pair(new Character(cubesJ), new Character(cubes5_2), new Sphere(0, 6, 0, 5, 10, 10));

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
	pairA6 = new Pair(new Character(cubesA), new Character(cubes6), new Sphere(0, 6, 0, 5, 10, 10));

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
	pairN2 = new Pair(new Character(cubesN), new Character(cubes2), new Sphere(0, 6, 0, 5, 10, 10));

	srand(time(NULL));

	generateObjects(-(GRID_SIZE - 10) / 2, (GRID_SIZE - 10) / 2, 10, GRID_SIZE / 2);

	glEnable(GL_DEPTH_TEST);

	// configure depth map FBO
	// -----------------------
	const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1920;
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
	float bordercolor[] = { 1.0, 1.0, 1.0, 1.0 };
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

	textureShader->use();
	textureShader->setInt("shadowMap", 1);


	// lighting info
	glm::vec3 lightPosition(0.0f, 30.0f, -5.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);

	// skybox info
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<std::string> faces
	{
			"src/textures/right.jpg",
			"src/textures/left.jpg",
			"src/textures/top.jpg",
			"src/textures/bottom.jpg",
			"src/textures/front.jpg",
			"src/textures/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	skyBoxShader->use();
	skyBoxShader->setInt("skybox", 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//Jump Animation
		if (is_jumping) {
			glm::vec3 newCameraPos = cameraPos + cameraSpeed * glm::vec3(0.0f, jumpValue, 0.0f);
			if (!checkCollision(newCameraPos)) {
				cameraPos = newCameraPos;
			}
			if (jumpValue >= JUMPMAXHEIGHT) {
				Jumpdown = true;
			}

			if (Jumpdown) {
				jumpValue-= JUMPSPEED;
			}
			else {
				jumpValue+= JUMPSPEED;
			}

			if (cameraPos.y <= 2.0f && Jumpdown) {
				cameraPos.y = 2.0f;
				is_jumping = false;
				Jumpdown = false;
				jumpValue = 0.0f;
			}
		}

		//Get User Window Size and Postion
		if (!is_fs) {
			glfwGetWindowSize(window, &user_win_length, &user_win_height);
			glfwGetWindowPos(window, &user_win_xpos, &user_win_ypos);
		}

		// Procedurally grow terrain and creates objects
		if (cameraPos.x < -GRID_SIZE / 2 || cameraPos.x > GRID_SIZE / 2 ||
			cameraPos.z < -GRID_SIZE / 2 || cameraPos.z > GRID_SIZE / 2) {
			GRID_SIZE += 50;
			grid->setSize(GRID_SIZE);

			generateObjects(-GRID_SIZE / 2, GRID_SIZE / 2, (GRID_SIZE - 50) / 2, GRID_SIZE / 2);
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 modelU4 = glm::mat4(1.0f);
		modelU4 = glm::translate(modelU4, glm::vec3(pairU4Pos.x, pairU4Pos.y, pairU4Pos.z));
		modelU4 = glm::scale(modelU4, glm::vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));

		glm::mat4 modelE5 = glm::mat4(1.0f);
		modelE5 = glm::translate(modelE5, glm::vec3(pairE5Pos.x, pairE5Pos.y, pairE5Pos.z));
		modelE5 = glm::scale(modelE5, glm::vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));

		glm::mat4 modelJ5 = glm::mat4(1.0f);
		modelJ5 = glm::translate(modelJ5, glm::vec3(pairJ5Pos.x, pairJ5Pos.y, pairJ5Pos.z));
		modelJ5 = glm::scale(modelJ5, glm::vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));

		glm::mat4 modelA6 = glm::mat4(1.0f);
		modelA6 = glm::translate(modelA6, glm::vec3(pairA6Pos.x, pairA6Pos.y, pairA6Pos.z));
		modelA6 = glm::scale(modelA6, glm::vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));

		glm::mat4 modelN2 = glm::mat4(1.0f);
		modelN2 = glm::translate(modelN2, glm::vec3(pairN2Pos.x, pairN2Pos.y, pairN2Pos.z));
		modelN2 = glm::scale(modelN2, glm::vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));

		//Shadow Pass 1 - Shadow Map
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 40.5f;
		lightProjection = glm::ortho(-GRID_SIZE/(float)2.0, GRID_SIZE/(float)2.0, -GRID_SIZE/(float)2.0, GRID_SIZE/(float)2.0, near_plane, far_plane);
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

		pairU4->draw(depthShader, sphereShader, modelU4);
		pairE5->draw(depthShader, sphereShader, modelE5);
		pairJ5->draw(depthShader, sphereShader, modelJ5);
		pairA6->draw(depthShader, sphereShader, modelA6);
		pairN2->draw(depthShader, sphereShader, modelN2);

		// reset viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_LENGTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Shadow Pass 2 - Normal Render
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_LENGTH / (float)WINDOW_HEIGHT, 0.1f, 500.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//Generate Sphere
		sphereShader->use();
		sphereShader->setMat4("projection", projection);
		sphereShader->setMat4("view", view);
		sphereShader->setVec3("lightColor", lightColor);
		sphereShader->setVec3("lightPos", lightPosition);
		sphereShader->setVec3("viewPos", cameraPos);

		//Light For Texture
		textureShader->use();
		textureShader->setMat4("projection", projection);
		textureShader->setMat4("view", view);
		// set light uniforms
		textureShader->setVec3("lightColor", lightColor);
		textureShader->setVec3("lightPos", lightPosition);
		textureShader->setVec3("viewPos", cameraPos);
		textureShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		gridShader->use();
		gridShader->setMat4("projection", projection);
		gridShader->setMat4("view", view);
		// set light uniforms
		gridShader->setVec3("lightColor", lightColor);
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

		pairU4->draw(textureShader, sphereShader, modelU4, ruinTexture);
		pairE5->draw(textureShader, sphereShader, modelE5, ruinTexture);
		pairJ5->draw(textureShader, sphereShader, modelJ5, ruinTexture);
		pairA6->draw(textureShader, sphereShader, modelA6, ruinTexture);
		pairN2->draw(textureShader, sphereShader, modelN2, ruinTexture);

		// draw skybox as last
		glEnable(GL_DEPTH_CLAMP);
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyBoxShader->use();
		view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp))); // remove translation from the view matrix
		skyBoxShader->setMat4("view", view);
		skyBoxShader->setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (std::vector<Tree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
		delete *it;
	}

	for (std::vector<Bush*>::iterator it = bushes.begin(); it != bushes.end(); ++it) {
		delete *it;
	}

	delete pairU4;
	delete pairE5;
	delete pairJ5;
	delete pairA6;
	delete pairN2;
	delete grid;
	delete ruinTexture;
	delete gridShader;
	delete depthShader;
	delete textureShader;
	delete sphereShader;
	delete lightShader;
	delete basicShader;

	glfwTerminate();

	return 0;
}