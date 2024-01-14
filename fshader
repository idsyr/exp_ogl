#version 330 core

struct Material{
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
       float shininess;
};
uniform Material material;
struct Light{
       vec3 position;
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
};
uniform Light light;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
//uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main(){
//vec4 textColor = mix(texture(texture1, TC), texture(texture2, TC), 0.1);
//vec4(lightColor* objectColor, 1.0f);
vec3 ambient = material.ambient * light.ambient;

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos-FragPos);

float diff = max(dot(norm, lightDir), 0.0f);
vec3 diffuse = (diff*material.diffuse) * light.diffuse;

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
vec3 specular = material.specular * spec * light.specular;

vec3 result = (ambient + diffuse + specular) * objectColor;
FragColor = vec4(result, 1.0f);
}

