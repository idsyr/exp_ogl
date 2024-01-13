#version 330 core
in vec2 TC;
out vec4 FragColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 objectColor;
uniform vec3 lightColor;
void main(){
FragColor = //mix(texture(texture1, TC), texture(texture2, TC), 0.1);
vec4(lightColor* objectColor, 1.0f);
}

