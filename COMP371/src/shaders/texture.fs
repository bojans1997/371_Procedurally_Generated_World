#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec2 textCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D ourTexture;
  
void main()
{
    //Ambient
	
    vec3 ambient = lightColor * material.ambient;

	// diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

	// specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular); 

    vec3 result = (ambient + diffuse + specular) * vec3(texture(ourTexture, textCoord));
    FragColor = vec4(result, 1.0);
	
	//FragColor = texture(ourTexture, textCoord);
}