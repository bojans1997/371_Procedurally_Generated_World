#include "grid.h"

Grid::Grid(int size) : size(size)
{
	float vertices[] = {
		// position             // color           // texture
		-1.0f,  -0.01f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // top left
		 1.0f,  -0.01f,  1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // top right
		 1.0f,  -0.01f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
		-1.0f,  -0.01f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f    // bottom left
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Grid::~Grid()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Grid::draw(Shader *shader, Texture *texture)
{
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	shader->use();
	shader->setVec3("material.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader->setVec3("material.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader->setVec3("material.specular", glm::vec3(0.7f, 0.7f, 0.7f));
	shader->setFloat("material.shininess", 10.0f);
	glBindVertexArray(VAO);

	for (int i = -size / 2; i <= size / 2; i++) {
		for (int j = size / 2; j >= -size / 2; j--) {

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i, 0, j));
			shader->setMat4("model", model);

			glDrawArrays(GL_QUADS, 0, 4);
		}
	}

	if (texture) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}