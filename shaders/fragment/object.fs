#version 330 core

in vec3 ourNormal;
in vec2 ourTexCoords;
in vec3 fragPos;
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D texture0;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

void main()
{
    vec3 ambient = material.ambient * light.ambient;

    vec3 norm = normalize(ourNormal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;  

    vec4 texColor = texture(texture0, ourTexCoords);
    FragColor = texColor * vec4((ambient + diffuse + specular) * objectColor, 1.0);
}