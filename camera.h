#if !defined(CAMERA_H)
#define CAMERA_H 1
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
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
#endif
