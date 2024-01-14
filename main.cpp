#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
#include "data.h"
#include "camera.h"
#include "texture.h"
using std::cout; using std::endl; using std::sin;
int success; char infoLog[512];
uint scr_w = 1360, scr_h = 768;
float delta_time = 0.0f, lastFrame = 0.0f;
float lastX = 0, lastY = 0;

Camera cam1(scr_w, scr_h);
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam1.action_w(delta_time);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam1.action_s(delta_time);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam1.action_a(delta_time);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam1.action_d(delta_time); 

	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    cam1.action_on_scroll(yoffset);}
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; 
    lastY = ypos;
    cam1.action_on_mouse(xoffset, yoffset);}
void framebuffer_size_callback(GLFWwindow* window, int w, int h){glViewport(0,0,w,h);}

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(scr_w, scr_h, "experimental", NULL, NULL);
	if(window == NULL){cout<<"window == NULL"<<endl; glfwTerminate(); return -1;}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<<"gladLoadGLLoader"<<endl; return -1;}

	glViewport(0,0,scr_w,scr_h);
    
    uint VBO; glGenBuffers(1, &VBO);      glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint VAO; glGenVertexArrays(1, &VAO); glBindVertexArray(VAO); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    uint lightVAO; glGenVertexArrays(1, &lightVAO); glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    Texture texture1("/home/ids/pro/exp/rsr/ras0.jpg");
    Texture texture2("/home/ids/pro/exp/rsr/t9.png"); 

    Shader shaderProgram("/home/ids/pro/exp/vshader", "/home/ids/pro/exp/fshader");
    shaderProgram.setInt("texture1.ID", 0); shaderProgram.setInt("texture2.ID", 1);

    Shader shaderProgramL("/home/ids/pro/exp/vshader", "/home/ids/pro/exp/flshader");

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//(0.5f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture1.ID);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texture2.ID);
        
        glm::mat4 projection = glm::perspective(glm::radians(cam1.get_fov()), (float)scr_w/(float)scr_h, 0.1f, 100.0f);
        glm::mat4 view = cam1.get_view();
        shaderProgram.use();
        shaderProgram.setVec3f("objectColor", 1.0f, 0.9f, 0.9f);
        shaderProgram.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        shaderProgram.setVec3f("lightPos", 1.2f, 1.0f, 2.0f);
        shaderProgram.setVec3f("viewPos", cam1.position);

        shaderProgram.setVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
        shaderProgram.setVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        shaderProgram.setVec3f("material.specular", 0.5f, 0.5f, 0.5f);
        shaderProgram.setFloat("material.shininess", 32.0f);
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime()*2.0f);
        lightColor.y = sin(glfwGetTime()*0.7f);
        lightColor.z = sin(glfwGetTime()*1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        
        
        shaderProgram.setVec3f("light.ambient", ambientColor);
        shaderProgram.setVec3f("light.diffuse", diffuseColor);
        shaderProgram.setVec3f("light.specular", 1.0f, 1.0f, 1.0f);
 
        shaderProgram.setMat4f(view);
        shaderProgram.setMat4f(projection);
        glBindVertexArray(VAO);
        for(uint i = 0; i<10; ++i){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime()*glm::radians(-50.0f),
                                glm::vec3(0.5f, 1.0, 0.0f));
            shaderProgram.setMat4f(model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        shaderProgramL.use();
        shaderProgramL.setMat4f(view);
        shaderProgramL.setMat4f(projection);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shaderProgram.setMat4f(model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
