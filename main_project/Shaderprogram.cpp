#include "Shaderprogram.hpp"

ShaderProgram::ShaderProgram(Shader * s)
{
	createShaderPgm();
	setShader(s);
}

ShaderProgram::ShaderProgram()
{
	createShaderPgm();
}

void ShaderProgram::setShader(Shader * s)
{
	linkShader(s->vertex, s->fragment);
}

void ShaderProgram::setShader(GLuint vert, GLuint frag)
{
	linkShader(vert, frag);
}

void ShaderProgram::setShader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	Shader* s=new Shader(vertexPath, fragmentPath);
	
	if (s) {
		setShader(s);
		delete s;
	}
	
}

void ShaderProgram::linkShader(GLuint newVertHandle, GLuint newFragHandle)
{
	const GLsizei maxCount = 2;
	GLsizei count;
	GLuint shaders[maxCount];
	glGetAttachedShaders(programHandle, maxCount, &count, shaders);
	// cout << "get VertHandle: " << shaders[0] << endl;
	// cout << "get FragHandle: " << shaders[1] << endl;
	GL_ERROR();
	for (int i = 0; i < count; i++) {
		glDetachShader(programHandle, shaders[i]);
	}
	// Bind index 0 to the shader input variable "VerPos"
	glBindAttribLocation(programHandle, 0, "VerPos");
	// Bind index 1 to the shader input variable "VerClr"
	glBindAttribLocation(programHandle, 1, "VerClr");
	glBindAttribLocation(programHandle, 2, "Normal");
	GL_ERROR();
	glAttachShader(programHandle, newVertHandle);
	glAttachShader(programHandle, newFragHandle);
	GL_ERROR();
	glLinkProgram(programHandle);
	if (GL_FALSE == checkShaderLinkStatus())
	{
		std::cerr << "Failed to relink shader program!" << std::endl;
		exit(EXIT_FAILURE);
	}
	GL_ERROR();
}

GLint ShaderProgram::checkShaderLinkStatus()
{
	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			GLchar * log = (GLchar *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			std::cerr << "Program log: " << log << std::endl;
		}
	}
	return status;
}
