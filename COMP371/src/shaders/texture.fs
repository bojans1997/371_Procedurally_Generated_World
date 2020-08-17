#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec2 textCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;

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


uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if (projCoords.z > 1.0){
        projCoords.z = 1.0;
    }
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias= 0.05;
    float shadow = (currentDepth-bias) > closestDepth  ? 1.0 : 0.0;

    return shadow;
}
  
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
    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular); 

	float shadow = ShadowCalculation(fragPosLightSpace);

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular))* vec3(texture(ourTexture, textCoord)) * texture(diffuseTexture, textCoord).rgb;
    FragColor = vec4(result, 1.0);

    //FragColor = texture(ourTexture, textCoord);
}