#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoord;
layout (location = 3) in vec3 aNormal;
  
out vec3 ourColor;
out vec3 normal;
out vec2 textCoord;
out vec3 fragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	fragPos = vec3(model* vec4(aPos, 1.0f));
    gl_Position = projection * view * vec4(fragPos, 1.0);
    ourColor = aColor;
	textCoord = aTextCoord;
	normal = transpose(inverse(mat3(model))) * aNormal;
	FragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
} 