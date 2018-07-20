/*
Shader class - v1
- loading Shader source code, Shader Program creation
implementazione classe per caricamento codice shader e creazione Program Shader

N.B. ) adaptation of https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/shader.h

author: Davide Gadia

Real-time Graphics Programming - a.a. 2017/2018
Master degree in Computer Science
Universita' degli Studi di Milano
*/

#pragma once

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utils/gl_error.h>

// GL Includes
#include <glad/glad.h> // Contains all the necessery OpenGL includes

/////////////////// SHADER class ///////////////////////
class Shader
{
public:
    GLuint Program;

    //////////////////////////////////////////

    //constructor
	Shader() {

	}

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        // Step 1: we retrieve shaders source code from provided filepaths
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        // converto le stringhe in puntatori a char
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar * fShaderCode = fragmentCode.c_str();

        // Step 2: we compile the shaders
        GLuint vertex, fragment;

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
		glCheckError();
        glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCheckError();
        glCompileShader(vertex);
		glCheckError();
        // check compilation errors
        checkCompileErrors(vertex, "VERTEX");
		glCheckError();

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glCheckError();
        glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCheckError();
        glCompileShader(fragment);
		glCheckError();
        // check compilation errors
        checkCompileErrors(fragment, "FRAGMENT");
		glCheckError();

        // Step 3: Shader Program creation
        this->Program = glCreateProgram();
		glCheckError();
        glAttachShader(this->Program, vertex);
		glCheckError();
        glAttachShader(this->Program, fragment);
		glCheckError();
        glLinkProgram(this->Program);
		glCheckError();
        // check linking errors
        checkCompileErrors(this->Program, "PROGRAM");
		glCheckError();

        // Step 4: we delete the shaders because they are linked to the Shader Program, and we do not need them anymore
        glDeleteShader(vertex);
		glCheckError();
        glDeleteShader(fragment);
		glCheckError();
    }

    //////////////////////////////////////////

    // We activate the Shader Program as part of the current rendering process
    void Use() { glUseProgram(this->Program); }

    // We delete the Shader Program when application closes
    void Delete() {    glDeleteProgram(this->Program); }

private:
    //////////////////////////////////////////

    // Check compilation and linking errors
    void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if(type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::::PROGRAM-LINKING-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
			}
		}
	}
};
