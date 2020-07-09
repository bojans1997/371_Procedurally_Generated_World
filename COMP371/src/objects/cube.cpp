#include "cube.h"

Cube::Cube(int x, int y, int z) : x(x), y(y), z(z), VAO(0), VBO(0)
{
	float vertices[] = {
		0.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f,
		1.0f,  1.0f, 0.0f,  0.5f, 0.5f, 0.5f,
		1.0f,  1.0f, 0.0f,  0.5f, 0.5f, 0.5f,
		0.0f,  1.0f, 0.0f,  0.5f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f,

		0.0f, 0.0f, 1.0f,  0.5f, 0.5f, 0.5f,
		1.0f, 0.0f,	1.0f,  0.5f, 0.5f, 0.5f,
		1.0f,  1.0f,  1.0f,  0.5f, 0.5f, 0.5f,
		1.0f,  1.0f,  1.0f,  0.5f, 0.5f, 0.5f,
		0.0f,  1.0f,  1.0f,  0.5f, 0.5f, 0.5f,
		0.0f, 0.0f,  1.0f,  0.5f, 0.5f, 0.5f,

		0.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f,
		1.0f, 0.0f,  1.0f,  0.5f, 0.5f, 0.5f,
		1.0f, 0.0f,  1.0f,  0.5f, 0.5f, 0.5f,
		0.0f, 0.0f,  1.0f,  0.5f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f,

		0.0f,  1.0f,  1.0f,  0.75f, 0.75f, 0.75f,
		0.0f,  1.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		0.0f, 0.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		0.0f, 0.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		0.0f, 0.0f,  1.0f,  0.75f, 0.75f, 0.75f,
		0.0f,  1.0f,  1.0f,  0.75f, 0.75f, 0.75f,

		1.0f,  1.0f,  1.0f,  0.75f, 0.75f, 0.75f,
		1.0f,  1.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		1.0f, 0.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		1.0f, 0.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		1.0f, 0.0f,  1.0f,  0.75f, 0.75f, 0.75f,
		1.0f,  1.0f,  1.0f,  0.75f, 0.75f, 0.75f,

		0.0f,  1.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		1.0f,  1.0f, 0.0f,  0.75f, 0.75f, 0.75f,
		1.0f,  1.0f,  1.0f,  0.75f, 0.75f, 0.75f,
		1.0f,  1.0f,  1.0f,  0.75f, 0.75f, 0.75f,
		0.0f,  1.0f,  1.0f,  0.75f, 0.75f, 0.75f,
		0.0f,  1.0f, 0.0f,  0.75f, 0.75f, 0.75f

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

Cube::~Cube()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Cube::draw(Shader* shader, unsigned int modelRenderMode, glm::mat4 matrix) {
	shader->use();
	glBindVertexArray(VAO);
	
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	shader->setMat4("model", matrix);

	glDrawArrays(modelRenderMode, 0, 36);
}