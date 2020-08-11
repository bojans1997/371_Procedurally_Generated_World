#include "Stack.h"

Stack::Stack(float x, float y, float z, float sA1, float sA2, float radius, int sectorCount) : stackAngle1(sA1), stackAngle2(sA2), radius(radius), sectorCount(sectorCount)
{	
	float sectorStep = 2 * M_PI / sectorCount;
	float sectorAngle;
	for (int i = 0; i < sectorCount; i++) {
		sectorAngle = i * sectorStep;
		sectorAngle2 = (i + 1)*sectorStep;

		sectors.push_back(new Sector(x, y, z, sA1, sA2, radius, sectorAngle, sectorAngle2));
	}
}

void Stack::draw(Shader *shader, glm::mat4 matrix)
{
	for (std::vector<Sector*>::iterator it = sectors.begin(); it != sectors.end(); ++it) {
		(*it)->draw(shader, matrix);
	}
}

Stack::~Stack()
{
	for (std::vector<Sector*>::iterator it = sectors.begin(); it != sectors.end(); ++it) {
		delete *it;
	}
}