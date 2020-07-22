#include "Sphere.h"

Sphere::Sphere(float x, float y, float z, float radius, int sC, int rC) : x(x), y(y), z(z), radius(radius), stackCount(sC), sectorCount(rC)
{
	float stackStep = M_PI / stackCount;

	float stackAngle, stackAngle2;
	for (int i = 0; i < stackCount; i++) {
		stackAngle = M_PI_2 - i * stackStep;
		stackAngle2 = M_PI_2 - (i+1) * stackStep;

		stacks.push_back(new Stack(x, y, z, stackAngle, stackAngle2, radius, sectorCount));
	}
}

void Sphere::draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (std::vector<Stack*>::iterator it = stacks.begin(); it != stacks.end(); ++it) {
		(*it)->draw(shader, modelRenderMode, matrix);
	}

	glDisable(GL_BLEND);
}

Sphere::~Sphere()
{
	for (std::vector<Stack*>::iterator it = stacks.begin(); it != stacks.end(); ++it) {
		delete *it;
	}
}