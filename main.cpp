#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <cmath>
using std::cout; using std::endl; using std::sin;
int success; char infoLog[512];

void framebuffer_size_callback(GLFWwindow* window, int w, int h){
    glViewport(0,0,w,h);}

void processInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);}

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "experimental", NULL, NULL);
	if(window == NULL){cout<<"window == NULL"<<endl; glfwTerminate(); return -1;}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<<"gladLoadGLLoader"<<endl; return -1;}

	glViewport(0,0,800,600);
    
    float vertices[] = {
        -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f
    };
    uint indices[] = {
        0, 1, 2
    };

    uint VAO; glGenVertexArrays(1, &VAO); glBindVertexArray(VAO);
    uint VBO; glGenBuffers(1, &VBO);      glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //uint EBO; glGenBuffers(1, &EBO);      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shaderProgram("../vshader", "../fshader");
	
    while(!glfwWindowShouldClose(window)){
		processInput(window);
		glClearColor(0.5f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        //glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 
        //            0.1f, sin(glfwGetTime()/2.0f+0.5f), 0.3f, 1.0f);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
