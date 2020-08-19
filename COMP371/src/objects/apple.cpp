#include "apple.h"

Apple::Apple(glm::vec3 position, glm::vec3 size) : position(position), size(size)
{
	apple = new Cube(position.x, position.y, position.z);
}

Apple::~Apple()
{
	delete apple;
	delete appleTexture;
}

void Apple::draw(Shader *shader)
{
	glm::mat4 appleMatrix = apple->getModel();

	shader->setVec3("material.ambient", glm::vec3(0.5f, 0.64f, 0.5f));
	shader->setVec3("material.diffuse", glm::vec3(0.22f, 0.24f, 0.22f));
	shader->setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
	shader->setFloat("material.shininess", 5.0f);

	appleMatrix = glm::scale(appleMatrix, glm::vec3(size.x, size.y, size.z));
	apple->draw(shader, appleMatrix, appleTexture);
}

void Apple::moveApple(Shader *shader, glm::vec3 positionVector)
{
	glm::mat4 appleMatrix = apple->getModel();

	appleMatrix = glm::translate(appleMatrix, glm::vec3(positionVector.x, positionVector.y, positionVector.z));
	apple->draw(shader, appleMatrix, appleTexture);
}