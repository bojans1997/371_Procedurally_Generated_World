#include "Sector.h"

Sector::Sector(float x, float y, float z, float sA1, float sA2, float r, float s1, float s2) : x(x), y(y), z(z), stackAngle1(sA1), stackAngle2(sA2), radius(r), sectorAngle1(s1), sectorAngle2(s2)
{
	float x1 = (radius*cos(stackAngle1))*sin(sectorAngle1);
	float y1 = (radius*sin(stackAngle1));
	float z1 = (radius*cos(stackAngle1))*cos(sectorAngle1);

	float x2 = (radius*cos(stackAngle2))*sin(sectorAngle1);
	float y2 = (radius*sin(stackAngle2));
	float z2 = (radius*cos(stackAngle2))*cos(sectorAngle1);

	float x3 = (radius*cos(stackAngle1))*sin(sectorAngle2);
	float y3 = (radius*sin(stackAngle1));
	float z3 = (radius*cos(stackAngle1))*cos(sectorAngle2);

	float x4 = (radius*cos(stackAngle2))*sin(sectorAngle2);
	float y4 = (radius*sin(stackAngle2));
	float z4 = (radius*cos(stackAngle2))*cos(sectorAngle2);

	glm::vec3 v1(x2-x1, y2-y1, z2-z1);
	glm::vec3 v2(x3 - x1, y3 - y1, z3 - z1);

	glm::vec3 nv = glm::cross(v1, v2);
	normal = glm::normalize(nv);

	float vertices[]{
		x1, y1, z1, normal.x, normal.y, normal.z,
		x2, y2, z2, normal.x, normal.y, normal.z,
		x3, y3, z3, normal.x, normal.y, normal.z,
		x2, y2, z2, normal.x, normal.y, normal.z,
		x3, y3, z3, normal.x, normal.y, normal.z,
		x4, y4, z4, normal.x, normal.y, normal.z
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

Sector::~Sector()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Sector::draw(Shader *shader, glm::mat4 matrix)
{
	shader->use();
	glBindVertexArray(VAO);
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	shader->setMat4("model", matrix);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}