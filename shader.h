#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using std::cout; using std::endl; using std::string;
extern int success; extern char infoLog[512];
struct Shader{ 
    uint vertexShader;
    uint fragmentShader;
    uint shaderProgram;
    Shader(const char* pathv, const char* pathf){
        const char *vertexShaderSource, *fragmentShaderSource;
        std::ifstream vshaderFile, fshaderFile; std::stringstream ssv, ssf; string t1, t2;
        vshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try{
            vshaderFile.open(pathv); fshaderFile.open(pathf);
            ssv<<vshaderFile.rdbuf(); t1 = ssv.str(); vertexShaderSource = t1.c_str();
            ssf<<fshaderFile.rdbuf(); t2 = ssf.str(); fragmentShaderSource = t2.c_str();
            vshaderFile.close(); fshaderFile.close();
        }catch(std::ifstream::failure e){
        cout<<"ERROR: SHADER: OPEN VFSHADER"<<endl;}
           
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success){glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); 
        cout<<"ERROR: VERTEX SHADER: "<<infoLog<<endl;}

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success){glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout<<"ERROR: FRAGMENT SHADER: "<<infoLog<<endl;}

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);    
        glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
        if(!success){glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog); 
        cout<<"ERROR: SHADER PROGRAM: "<<infoLog<<endl;}
    }
    ~Shader(){
        //glDeleteShader(vertexShader);
        //glDeleteShader(fragmentShader);
    }
    void use(){
        glUseProgram(shaderProgram);
    }
};


#endif 
