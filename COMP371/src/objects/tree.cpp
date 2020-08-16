#include "tree.h"

Tree::Tree(glm::vec3 position, int size) : position(position), size(size)
{
	bark = new Cube(position.x, position.y, position.z);
	leaves = new Cube(position.x, position.y, position.z);
}

Tree::~Tree()
{
	delete bark;
	delete leaves;
	delete barkTexture;
	delete leavesTexture;
}

void Tree::draw(Shader *shader)
{
	glm::mat4 barkMatrix = bark->getModel();

	shader->setVec3("material.ambient", glm::vec3(0.49f, 0.15f, 0.0f));
	shader->setVec3("material.diffuse", glm::vec3(0.1f, 0.17f, 0.12f));
	shader->setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
	shader->setFloat("material.shininess", 10.0f);

	barkMatrix = glm::scale(barkMatrix, glm::vec3(1, size, 1));
	bark->draw(shader, barkMatrix, barkTexture);

	glm::mat4 leavesMatrix = leaves->getModel();

	shader->setVec3("material.ambient", glm::vec3(0.11f, 0.53f, 0.15f));
	shader->setVec3("material.diffuse", glm::vec3(0.03f, 0.17f, 0.12f));
	shader->setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
	shader->setFloat("material.shininess", 5.0f);

	leavesMatrix = glm::translate(leavesMatrix, glm::vec3(-size/2, size, -size/2));
	leavesMatrix = glm::scale(leavesMatrix, glm::vec3(size, size, size));
	leaves->draw(shader, leavesMatrix, leavesTexture);
}