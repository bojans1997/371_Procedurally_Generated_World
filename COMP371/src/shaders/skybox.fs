#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform float light;
uniform vec3 lightColor;

void main()
{    
	vec3 ambiant = 0.2 * lightColor;
	vec3 result = (ambiant + (1.0f * light)) * vec3(texture(skybox, TexCoords));
    FragColor = vec4(result, 1.0f);
}