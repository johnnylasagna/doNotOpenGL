#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform bool hasHeightMap;
uniform sampler2D heightMap;

void main()
{
    TexCoords = aTexCoords;
    vec3 pos = aPos;
    vec3 norm = aNormal;

    if (hasHeightMap) {
        vec3 sphereNormal = normalize(aPos);
        float h1 = texture(heightMap, aTexCoords).r;
        float h2 = texture(heightMap, aTexCoords + vec2(0.1, 0.0)).r;
        float h3 = texture(heightMap, aTexCoords + vec2(-0.1, 0.0)).r;
        
        float rawHeight = (h1 + h2 + h3) / 3.0;
        float heightScale = 0.2;
        float height = rawHeight * heightScale;
        
        pos = aPos + sphereNormal * height;
        norm = normalize(pos);
    }

    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = normalize(normalMatrix * norm);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}