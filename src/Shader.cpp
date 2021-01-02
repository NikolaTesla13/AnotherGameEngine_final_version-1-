#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <Debugger.h>

Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexFile);
        fShaderFile.open(fragmentFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
    glCall(glCompileShader(vertex));
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
    glCall(glCompileShader(fragment));
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glCall(glAttachShader(ID, vertex));
    glCall(glAttachShader(ID, fragment));
    glCall(glLinkProgram(ID));
    checkCompileErrors(ID, "PROGRAM");

    glCall(glDeleteShader(vertex));
    glCall(glDeleteShader(fragment));

    int texLoc = glGetUniformLocation(ID, "u_Textures");
    int viewLoc = glGetUniformLocation(ID, "ortho");

    int sampler[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
    glm::mat4 view = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);

    glCall(glUseProgram(ID));
    glCall(glUniform1iv(texLoc, 32, sampler));
    glCall(glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]));

}

void Shader::start()
{
    glCall(glUseProgram(ID));
}

void Shader::stop()
{
    glCall(glUseProgram(0));
}

Shader::~Shader()
{
    glCall(glDeleteProgram(ID));
}

unsigned int Shader::getID()
{
    return ID;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            glCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glCall(glGetProgramiv(shader, GL_LINK_STATUS, &success));
        if (!success)
        {
            glCall(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}