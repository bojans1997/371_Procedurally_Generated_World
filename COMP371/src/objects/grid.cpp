#include "grid.h"

Grid::Grid(int size) : size(size), VAO(0), VBO(0)
{
	float vertices[] = {
		// position           // color
		-1.0f,  -0.01f,  1.0f,  1.0f, 1.0f, 0.0f,   // top left
		 1.0f,  -0.01f,  1.0f,  1.0f, 1.0f, 0.0f,   // top right
		 1.0f,  -0.01f, -1.0f,  1.0f, 1.0f, 0.0f,   // bottom right
		-1.0f,  -0.01f, -1.0f,  1.0f, 1.0f, 0.0f    // bottom left
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = -size/2; i <= size/2; i++) {
		for (int j = size/2; j >= -size/2; j--) {

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i, 0, j));
			shader->setMat4("model", model);

			glDrawArrays(GL_QUADS, 0, 4);
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}