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
using std::cout; using std::endl; using std::sin;
int success; char infoLog[512];

uint scr_w = 1360;
uint scr_h = 768;


    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;


float yaw = -90.0f;
float pitch = -90.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float fov = 42.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos; lastY = ypos; firstMouse = false;}

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; lastY = ypos;
    
    const float sens = 0.01f;
    xoffset *= sens; yoffset *= sens;
    yaw += xoffset; pitch += yoffset;
    if(pitch>89.0f)  pitch = 89.0f;
    if(pitch<-89.0f) pitch = -89.0f;

    glm::vec3 dir; 
    dir.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));
    cameraFront = glm::normalize(dir);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -=(float)yoffset;
    if(fov<1.0f) fov = 1.0f;
    if(fov>45.0f) fov = 45.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h){
    glViewport(0,0,w,h);}

void processInput(GLFWwindow* window){
    const float cameraSpeed = 2.5f*deltaTime;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos+=cameraSpeed*cameraFront;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos-=cameraSpeed*cameraFront;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos-=glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos+=glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;

	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);}

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

	glViewport(0,0,800,600);
    
    //float vertices[] = {
    //     0.5f,  0.5f,  0.0f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
    //     0.5f, -0.5f,  0.0f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
    //    -0.5f, -0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
    //    -0.5f,  0.5f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f, 1.0f
    //};
    //uint indices[] = {
    //    0, 1, 3,
    //    1, 2, 3
    //};
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    uint VAO; glGenVertexArrays(1, &VAO); glBindVertexArray(VAO);
    uint VBO; glGenBuffers(1, &VBO);      glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //uint EBO; glGenBuffers(1, &EBO);      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    //glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    uint lightVAO; glGenVertexArrays(1, &lightVAO); glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    uint texture1; glGenTextures(1, &texture1); glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    int width, height, nrChannels; string path = "/home/ids/pro/exp/ras0.jpg";
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if(data){
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {cout<<"ERROR: LOAD TEXTURE1"<<endl;}
    stbi_image_free(data);


    uint texture2; glGenTextures(1, &texture2); glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    path = "/home/ids/pro/exp/t9.png";
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {cout<<"ERROR: LOAD TEXTURE"<<endl;}
    stbi_image_free(data);


    Shader shaderProgram("/home/ids/pro/exp/vshader", "/home/ids/pro/exp/fshader");
    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    Shader shaderProgramL("/home/ids/pro/exp/vshader", "/home/ids/pro/exp/flshader");

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.5f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        shaderProgram.use();
        shaderProgram.setVec3f("objectColor", 1.0f, 0.5f, 0.31f);
        shaderProgram.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), (float)scr_w / (float)scr_h, 0.1f, 100.0f);
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.shaderProgram, "view"),
                           1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.shaderProgram, "projection"),
                           1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(VAO);
        for(uint i = 0; i<10; ++i){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime()*glm::radians(-50.0f),
                                glm::vec3(0.5f, 1.0, 0.0f));
 
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram.shaderProgram, "model"),
                           1, GL_FALSE, glm::value_ptr(model));
        
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        shaderProgramL.use();
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramL.shaderProgram, "view"),
                           1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramL.shaderProgram, "projection"),
                           1, GL_FALSE, glm::value_ptr(projection));
        
       glm::mat4 model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
       model = glm::scale(model, glm::vec3(0.2f));
       
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramL.shaderProgram, "model"),
                           1, GL_FALSE, glm::value_ptr(model));
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
