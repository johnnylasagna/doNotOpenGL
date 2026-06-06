#version 330 core

in vec3 ourNormal;
in vec2 ourTexCoords;
in vec3 fragPos;
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D texture0;

void main()
{
    vec3 norm = normalize(ourNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec4 texColor = texture(texture0, ourTexCoords);
    FragColor = texColor * vec4((ambient + diffuse) * objectColor, 1.0);
}