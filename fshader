#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TC;

out vec4 FragColor;

struct Material{
       sampler2D diffuse;
       sampler2D specular;
       float shininess;
};
struct Light{
       vec3 position;
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){
vec3 ambient = light.ambient * vec3(texture(material.diffuse, TC));

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(light.position-FragPos);
float diff = max(dot(norm, lightDir), 0.0f);
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TC));

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
vec3 specular = light.specular * spec * vec3(texture(material.specular, TC));

vec3 result = (ambient + diffuse + specular);
FragColor = vec4(result, 1.0f);
}

