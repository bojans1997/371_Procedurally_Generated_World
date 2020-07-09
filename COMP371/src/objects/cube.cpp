#include "cube.h"

Cube::Cube(int x, int y, int z) : x(x), y(y), z(z), VAO(0), VBO(0)
{
	float vertices[] = {
		//need to adjust vertices for 0-1 range, not -0.5 to 0.5
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

void Cube::draw(Shader *shader, unsigned int modelRenderMode)
{
	shader->use();
	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, z));
	//model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
	
	shader->setMat4("model", model);

	glDrawArrays(modelRenderMode, 0, 36);
}

void Cube::draw(Shader* shader, unsigned int modelRenderMode, glm::mat4 matrix) {
	shader->use();
	glBindVertexArray(VAO);
	
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	shader->setMat4("model", matrix);
	glDrawArrays(modelRenderMode, 0, 36);
}