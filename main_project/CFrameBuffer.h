#pragma once
#include <glew.h>  
#include <glut.h>  
#include<iostream>
#include<vector>
//#include <cv.h>  
//#include <highgui.h>  
//this class automaticly generate a framebuffer with one render buffer and a depth buffer
#define GL_ERROR() checkForOpenGLError(__FILE__, __LINE__)
static int checkForOpenGLError(const char* file, int line) {
	// return 1 if an OpenGL error occured, 0 otherwise.
	GLenum glErr;
	int retCode = 0;

	glErr = glGetError();
	//string s(gluErrorString());
	while (glErr != GL_NO_ERROR)
	{
		std::cout << "glError in file " << file
			<< "@line " << line << gluErrorString(glErr) << std::endl;
		retCode = 1;
 		exit(EXIT_FAILURE);
	}
	return retCode;
}
class CFrameBuffer
{
public:
	CFrameBuffer();
public:
	~CFrameBuffer();
private:
	GLuint m_FboID;
	GLuint m_RboID;
	std::vector<GLuint> m_tex;
	bool     m_bIsBegined;
	int      m_curbuff;

public:
	int     m_width;
	int     m_height;

public:
	void init(int width, int height);
	bool begin();
	bool end();
	void saveFrameBuff(const char* fileName);
	GLuint getsize() { return m_height*m_width; }
	GLuint getFb() { return m_FboID; }
	GLuint getDepTez() { return m_RboID; }
	GLuint getTex(int x) { return x<m_tex.size()?m_tex[x]:-1; }
	int gettexsize() { return m_tex.size(); }
	void AttachTextures();
};