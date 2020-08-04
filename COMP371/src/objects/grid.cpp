#include "grid.h"

Grid::Grid(float size)
{
	this->setSize(size);
}

void Grid::setSize(float size)
{
	float vertices[] = {
		// position            // normal           // texture
		-size / 2, -0.01f,  size / 2,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,     // top left
		 size / 2, -0.01f,  size / 2,  0.0f, 1.0f, 0.0f,  size / 2, 0.0f,   // top right
		 size / 2, -0.01f, -size / 2,  0.0f, 1.0f, 0.0f,  size / 2, size / 2, // bottom right
		-size / 2, -0.01f, -size / 2,  0.0f, 1.0f, 0.0f,  0.0f, size / 2    // bottom left
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Grid::~Grid()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Grid::draw(Shader *shader)
{
	shader->use();
	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 4);
}

void Grid::draw(Shader *shader, GLuint depthMap)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture->getTextureId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	this->draw(shader);

	glBindTexture(GL_TEXTURE_2D, 0);
}