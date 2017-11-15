#pragma once
#include"Shader.hpp"
#include <glew.h>
#include "CFrameBuffer.h"
class ShaderProgram{
public:
	ShaderProgram(Shader* s);
	ShaderProgram();
	void setShader(Shader* s);
	void setShader(GLuint vert,GLuint frag);
	void setShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use() {
		glUseProgram(this->programHandle);
	}
	GLuint program() { return programHandle; }
	~ShaderProgram() {
		glDeleteProgram(programHandle);
	}
private:
	void linkShader(GLuint newVertHandle, GLuint newFragHandle);
	GLint checkShaderLinkStatus();
	void createShaderPgm() {
		// Create the shader program
		programHandle = glCreateProgram();
		if (0 == programHandle) {
			std::cerr << "Error create shader program" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	
	GLuint programHandle;

};