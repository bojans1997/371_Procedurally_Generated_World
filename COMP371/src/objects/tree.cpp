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
	barkMatrix = glm::scale(barkMatrix, glm::vec3(1, size, 1));
	bark->draw(shader, barkMatrix, barkTexture);

	glm::mat4 leavesMatrix = leaves->getModel();
	leavesMatrix = glm::translate(leavesMatrix, glm::vec3(-size/2, size, -size/2));
	leavesMatrix = glm::scale(leavesMatrix, glm::vec3(size, size, size));
	leaves->draw(shader, leavesMatrix, leavesTexture);
}