#include "bush.h"

Bush::Bush(glm::vec3 position, glm::vec3 size) : position(position), size(size)
{
	bush = new Cube(position.x, position.y, position.z);
}

Bush::~Bush()
{
	delete bush;
	delete bushTexture;
}

void Bush::draw(Shader *shader)
{
	glm::mat4 bushMatrix = bush->getModel();

	shader->setVec3("material.ambient", glm::vec3(0.11f, 0.64f, 0.15f));
	shader->setVec3("material.diffuse", glm::vec3(0.03f, 0.24f, 0.12f));
	shader->setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
	shader->setFloat("material.shininess", 5.0f);

	bushMatrix = glm::scale(bushMatrix, glm::vec3(size.x, size.y, size.z));
	bush->draw(shader, bushMatrix, bushTexture);
}