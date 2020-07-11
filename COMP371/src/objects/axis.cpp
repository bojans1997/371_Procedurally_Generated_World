#include "axis.h"

Axis::Axis(int size) : size(size), VAO(0), VBO(0)
{
	float vertices[] = {
		// position           // color
		// x-axis
		 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

		// y-axis
		 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		// z-axis
		 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
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

Axis::~Axis()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Axis::draw(Shader *shader)
{
	shader->use();
	glBindVertexArray(VAO);

	glLineWidth(5);

	for (int i = 0; i < size; i++) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(i, 0, 0));
		shader->setMat4("model", model);

		glDrawArrays(GL_LINES, 0, 2);
	}

	for (int i = 0; i < size; i++) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, i, 0));
		shader->setMat4("model", model);

		glDrawArrays(GL_LINES, 2, 3);
	}

	for (int i = 0; i < size; i++) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, i));
		shader->setMat4("model", model);

		glDrawArrays(GL_LINES, 4, 6);
	}

	glLineWidth(1);
}