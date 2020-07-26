#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;
  
out vec3 ourColor;
out vec2 TexCoords;

out VS_OUT {
	out vec3 Normal;
	out vec3 FragPos;
	out vec4 FragPosLightSpace;
	out vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{	
	ourColor = aColor;
	
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
} 