#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform sampler2D bodyTexture;
uniform bool hasBodyTexture;

uniform bool isSkybox;
uniform bool isLightSource;
uniform bool hasMaterial;
uniform vec3 lightColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular; 
    float shininess;

    bool hasDiffuseMap;
    bool hasSpecularMap;
}; 
uniform Material material;

struct Light {
    int type; // 0 = Directional, 1 = Point, 2 = Spot
    vec3 position;
    vec3 direction;
    
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float intensity;
    float constant;
    float linear;
    float quadratic;
    
    float innerCutoff;
    float outerCutoff;
};

#define MAX_LIGHTS 16
uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

void main()
{
    vec4 baseTexColor = vec4(1.0);
    if (hasBodyTexture) {
        baseTexColor = texture(bodyTexture, TexCoords);
    }

    if (isSkybox) {
        FragColor = baseTexColor;
        return;
    }

    if (isLightSource) {
        FragColor = baseTexColor * vec4(lightColor, 1.0);
        return;
    }

    if (hasMaterial) {
        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos);
        
        vec3 finalLighting = vec3(0.0);
        vec3 diffuseTex = vec3(1.0);
        
        if (material.hasDiffuseMap) {
            diffuseTex = vec3(texture(material.diffuse, TexCoords));
        }
        
        vec3 specularTex = vec3(0.0); 
        if (material.hasSpecularMap) {
            specularTex = vec3(texture(material.specular, TexCoords));
        }

        vec3 baseColor = baseTexColor.rgb;

        for(int i = 0; i < lightCount; i++) {
            vec3 lightDir;
            float attenuation = 1.0;
            float spotIntensity = 1.0;

            if (lights[i].type == 0) {
                lightDir = normalize(-lights[i].direction);
            } else {
                lightDir = normalize(lights[i].position - FragPos);
                
                float distance = length(lights[i].position - FragPos);
                attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));

                if (lights[i].type == 2) {
                    float theta = dot(lightDir, normalize(-lights[i].direction));
                    float epsilon = lights[i].innerCutoff - lights[i].outerCutoff;
                    spotIntensity = clamp((theta - lights[i].outerCutoff) / epsilon, 0.0, 1.0);
                }
            }

            vec3 ambient = lights[i].color * lights[i].ambient * diffuseTex * baseColor * attenuation * lights[i].intensity;;
            
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = lights[i].color * diff * lights[i].diffuse * diffuseTex * baseColor * attenuation * spotIntensity * lights[i].intensity;;
            
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3 specular = lights[i].color * lights[i].specular * spec * specularTex * attenuation * spotIntensity * lights[i].intensity;;
            
            finalLighting += (ambient + diffuse + specular);
        }
        
        FragColor = vec4(finalLighting, 1.0);
    } else {
        FragColor = baseTexColor;
    }
}