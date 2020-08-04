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
	bushMatrix = glm::scale(bushMatrix, glm::vec3(size.x, size.y, size.z));
	bush->draw(shader, bushMatrix, bushTexture);
}