#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <irrKlang.h>
#include <iostream>
#include <map>
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
#include "objects/apple.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WINDOW_LENGTH = 1920;
const int WINDOW_HEIGHT = 1080;

int GRID_SIZE = 100;
int AXIS_SIZE = 5;

int MODEL_SCALE = 2.0f;

GLFWwindow* window;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool is_fs = true;
bool create_apple = true;
bool near_apple = false;
int apple_collected = 0;

bool mute = false;
bool is_jumping = false;
float jumpValue = 2.0f;
bool Jumpdown = false;
const float JUMPSPEED = 10.0f;
const float JUMPMAXHEIGHT = 5.0f;
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
std::vector<Apple*> apples;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float cameraSpeed = 10.0 * deltaTime;

float dayspeed = 20.0f;
float lightDistance = 30.0f;
float light = 0.0f;

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

bool checkAppleCollision(glm::vec3 newCameraPos) {
	for (std::vector<Apple*>::iterator it = apples.begin(); it != apples.end(); ++it) {
		glm::vec3 pos = (*it)->position;
		glm::vec3 size = (*it)->size;

		bool collisionX = newCameraPos.x + 0.7f >= pos.x && newCameraPos.x - 0.7f <= pos.x + size.x;
		bool collisionZ = newCameraPos.z + 0.7f >= pos.z && newCameraPos.z - 0.7f <= pos.z + size.z;

		if (collisionX && collisionZ) {
			return true;
		}
	}
	return false;
}

void generateObjects(int min1, int max1, int min2, int max2) {
	for (int i = 0; i < 25; i++) {
		int x = randomInt(min1, max1);
		int z = randomInt(min2, max2);
		int size = randomInt(3, 6);

		// prevent objects from spawning inside the player spawn
		while (x < 5 && x > -5 && z < 25 && z > 15) {
			x = randomInt(min1, max1);
			z = randomInt(min2, max2);
		}

		if ((rand() % 100) > 50) {
			z = -z;
		}

		trees.push_back(new Tree(glm::vec3(x, 0, z), size));
	}

	for (int i = 0; i < 25; i++) {
		int x = randomInt(min2, max2);
		int z = randomInt(min1, max1);
		int size = randomInt(3, 6);

		// prevent objects from spawning inside the player spawn
		while (x < 5 && x > -5 && z < 25 && z > 15) {
			x = randomInt(min1, max1);
			z = randomInt(min2, max2);
		}

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

		// prevent objects from spawning inside the player spawn
		while (x < 5 && x > -5 && z < 25 && z > 15) {
			x = randomInt(min1, max1);
			z = randomInt(min2, max2);
		}

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

		// prevent objects from spawning inside the player spawn
		while (x < 5 && x > -5 && z < 25 && z > 15) {
			x = randomInt(min1, max1);
			z = randomInt(min2, max2);
		}

		if (rand() % 100 >= 50) {
			x = -x;
		}

		bushes.push_back(new Bush(glm::vec3(x, 0, z), glm::vec3(xLen, yLen, zLen)));
	}

	if (create_apple) {
		for (int i = 0; i < 5; i++) {
			int x = randomInt(min1, max1);
			int z = randomInt(min2, max2);

			while (checkCollision(glm::vec3(x, 2, z))) {
				x = randomInt(min1, max1);
				z = randomInt(min2, max2);
			}

			apples.push_back(new Apple(glm::vec3(x, 0, z), glm::vec3(0.25f, 0.25f, 0.25f)));
		}
		create_apple = false;
	}

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

// Holds all state information relevant to a text character as loaded using FreeType. References from https://learnopengl.com/In-Practice/Text-Rendering
struct Text {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Text> Texts;
unsigned int textVAO, textVBO;

//To render Text. References from https://learnopengl.com/In-Practice/Text-Rendering
void RenderText(Shader *shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader->use();
	shader->setVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Text ch = Texts[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
		if (!checkCollision(newCameraPos) && !checkAppleCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos - cameraSpeed * cameraFront;
		newCameraPos.y = cameraPos.y;
		if (!checkCollision(newCameraPos) && !checkAppleCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos - glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (!checkCollision(newCameraPos) && !checkAppleCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (!checkCollision(newCameraPos) && !checkAppleCollision(newCameraPos)) {
			cameraPos = newCameraPos;
			if (footstep->getIsPaused()) {
				footstep->setIsPaused(false);
			}
		}
		
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
		glm::vec3 newCameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		near_apple = false;
		for (std::vector<Apple*>::iterator it = apples.begin(); it != apples.end(); ++it) {
			glm::vec3 pos = (*it)->position;
			glm::vec3 size = (*it)->size;

			bool appleCollisionX = newCameraPos.x + 1.5f >= pos.x && newCameraPos.x - 1.5f <= pos.x + size.x;
			bool appleCollisionZ = newCameraPos.z + 1.5f >= pos.z && newCameraPos.z - 1.5f <= pos.z + size.z;

			if (appleCollisionX && appleCollisionZ) {
				near_apple = true;
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		glm::vec3 newCameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

		for (std::vector<Apple*>::iterator it = apples.begin(); it != apples.end(); ++it) {
			glm::vec3 pos = (*it)->position;
			glm::vec3 size = (*it)->size;

			bool appleCollisionX = newCameraPos.x + 1.5f >= pos.x && newCameraPos.x - 1.5f <= pos.x + size.x;
			bool appleCollisionZ = newCameraPos.z + 1.5f >= pos.z && newCameraPos.z - 1.5f <= pos.z + size.z;

			if (appleCollisionX && appleCollisionZ) {
				(*it)->position.x = 100000.0f;
				(*it)->position.y = -10.0f;
				(*it)->position.z = 100000.0f;
				(*it)->~Apple();
				apple_collected++;
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
		glfwSetWindowMonitor(window, is_fs ? glfwGetPrimaryMonitor() : NULL, 0, 0, WINDOW_LENGTH, WINDOW_HEIGHT, GLFW_DONT_CARE);
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		mute = !mute;
		mute ? music->setVolume(0.0f) : music->setVolume(0.6f);
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
	window = glfwCreateWindow(WINDOW_LENGTH, WINDOW_HEIGHT, "COMP 371 Project - OpenGLHF", glfwGetPrimaryMonitor(), NULL);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
	Shader *textShader = new Shader("src/shaders/text.vs", "src/shaders/text.fs");

	//Text Setup. References from https://learnopengl.com/In-Practice/Text-Rendering
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(WINDOW_LENGTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));
	textShader->use();
	textShader->setMat4("projection", textProjection);

	//FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "src/resources/manaspc.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else
	{
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Text text = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Texts.insert(std::pair<char, Text>(c, text));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// configure textVAO/textVBO for texture quads
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

	Cube *sun = new Cube(0, 0, 0);
	Cube *moon = new Cube(0, 0, 0);

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
	textureShader->setInt("diffuseTexture", 0);

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

	/*
		Skybox images taken from https://learnopengl.com/img/textures/skybox.zip
	*/
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

	glm::vec3 lightPosition(sin(glfwGetTime()/dayspeed) * lightDistance, cos(glfwGetTime()/dayspeed) * lightDistance, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);

	while (!glfwWindowShouldClose(window))
	{
		// lighting info
		lightPosition = glm::vec3(sin(glfwGetTime() / dayspeed) * lightDistance, cos(glfwGetTime() / dayspeed) * lightDistance, 0.0);
		
		if (lightPosition.y <= 0) {
			light = 0.0f;
			lightColor = glm::vec3(1.0f);
		}
		else {
			light = 1.0f;
			if (lightPosition.y <= 10) {
				lightColor = glm::vec3(1.0f, 0.5f + (0.5f/10) * lightPosition.y, 0.5 + (2 * (0.5f/10) * lightPosition.y));
			}
		}

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//Jump Animation
		if (is_jumping) {
			footstep->setIsPaused(true);
			glm::vec3 newCameraPos = cameraPos;
			newCameraPos.y = jumpValue;
			if (!checkCollision(newCameraPos)) {
				cameraPos = newCameraPos;
			}
			if (jumpValue >= JUMPMAXHEIGHT) {
				Jumpdown = true;
			}

			if (Jumpdown) {
				jumpValue -= JUMPSPEED * deltaTime;
			}
			else {
				jumpValue += JUMPSPEED * deltaTime;
			}

			if (cameraPos.y <= 2.0f && Jumpdown) {
				cameraPos.y = 2.0f;
				is_jumping = false;
				Jumpdown = false;
				jumpValue = 2.0f;
			}
		}

		// Procedurally grow terrain and creates objects
		if (cameraPos.x < -GRID_SIZE / 2 || cameraPos.x > GRID_SIZE / 2 ||
			cameraPos.z < -GRID_SIZE / 2 || cameraPos.z > GRID_SIZE / 2) {
			GRID_SIZE += 50;
			grid->setSize(GRID_SIZE);

			generateObjects(-GRID_SIZE / 2, GRID_SIZE / 2, (GRID_SIZE - 50) / 2, GRID_SIZE / 2);
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

		glm::mat4 modelSun = glm::mat4(1.0f);
		modelSun = glm::translate(modelSun, glm::vec3(sin(glfwGetTime() / dayspeed) * (lightDistance + 20) + cameraPos.x, cos(glfwGetTime() / dayspeed) * (lightDistance + 20) + cameraPos.y, cameraPos.z));
		modelSun = glm::scale(modelSun, glm::vec3(1, 1, 1));

		glm::mat4 modelMoon = glm::mat4(1.0f);
		modelMoon = glm::translate(modelMoon, glm::vec3((sin(glfwGetTime() / dayspeed + glm::radians(180.0f)) * (lightDistance + 20) + cameraPos.x), (cos(glfwGetTime() / dayspeed + glm::radians(180.0f)) * (lightDistance + 20) + cameraPos.y), cameraPos.z));
		modelMoon = glm::scale(modelMoon, glm::vec3(1, 1, 1));

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

		for (std::vector<Apple*>::iterator it = apples.begin(); it != apples.end(); ++it) {
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

		//Render Text. References from https://learnopengl.com/In-Practice/Text-Rendering
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (apple_collected == 5) {
			RenderText(textShader, "You have collected all the apples.", 25.0f, 75.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			RenderText(textShader, "You can continue to play or press ESC to exit the game.", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else {
			RenderText(textShader, "Apples Collected:" + std::to_string(apple_collected) + "/5", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		if (near_apple) {
			RenderText(textShader, "Press E to collect the apple", 550.0f, 800.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		glDisable(GL_BLEND);

		//Shadow Pass 2 - Normal Render
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_LENGTH / (float)WINDOW_HEIGHT, 0.1f, 500.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//Generate Sphere
		sphereShader->use();
		sphereShader->setMat4("projection", projection);
		sphereShader->setMat4("view", view);
		sphereShader->setFloat("light", light);
		sphereShader->setVec3("lightColor", lightColor);
		sphereShader->setVec3("lightPos", lightPosition);
		sphereShader->setVec3("viewPos", cameraPos);

		//Light For Texture
		textureShader->use();
		textureShader->setMat4("projection", projection);
		textureShader->setMat4("view", view);
		// set light uniforms
		textureShader->setFloat("light", light);
		textureShader->setVec3("lightColor", lightColor);
		textureShader->setVec3("lightPos", lightPosition);
		textureShader->setVec3("viewPos", cameraPos);
		textureShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		gridShader->use();
		gridShader->setMat4("projection", projection);
		gridShader->setMat4("view", view);
		// set light uniforms
		gridShader->setFloat("light", light);
		gridShader->setVec3("lightColor", lightColor);
		gridShader->setVec3("lightPos", lightPosition);
		gridShader->setVec3("viewPos", cameraPos);
		gridShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//Sun
		lightShader->use();
		lightShader->setMat4("projection", projection);
		lightShader->setMat4("view", view);
		lightShader->setVec3("color", glm::vec3(1.0f, 0.9f, 0.0f));
		
		sun->draw(lightShader, modelSun);

		lightShader->setVec3("color", glm::vec3(0.6f, 0.9f, 1.0f));

		moon->draw(lightShader, modelMoon);


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

		for (std::vector<Apple*>::iterator it = apples.begin(); it != apples.end(); ++it) {
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
		skyBoxShader->setFloat("light", light);
		skyBoxShader->setVec3("lightColor", lightColor);
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