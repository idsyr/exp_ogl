#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
out vec4 Position;
void main(){
ourColor = aColor;
Position = vec4(-aPos.x+0.3f, -aPos.y, -aPos.z, 1.0);
gl_Position = vec4(-aPos.x+0.3f, -aPos.y, -aPos.z, 1.0);}
