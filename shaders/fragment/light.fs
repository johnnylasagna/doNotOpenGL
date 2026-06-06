#version 330 core
out vec4 FragColor;

in vec2 ourTexCoords;
uniform vec3 lightColor;
uniform sampler2D texture0;

void main()
{
    vec4 texColor = texture(texture0, ourTexCoords);
    FragColor = texColor * vec4(lightColor, 1.0);
}