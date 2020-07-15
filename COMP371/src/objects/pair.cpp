#include "pair.h"

Pair::Pair(Character *letter, Character *digit) : letter(letter), digit(digit) {}

Pair::~Pair() {
	delete letter;
	delete digit;
}

void Pair::draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix)
{
	letter->draw(shader, modelRenderMode, matrix);
	digit->draw(shader, modelRenderMode, matrix);
}

void Pair::draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix, Texture *letterTexture, Texture *digitTexture)
{
	letter->draw(shader, modelRenderMode, matrix, letterTexture);
	digit->draw(shader, modelRenderMode, matrix, digitTexture);
}