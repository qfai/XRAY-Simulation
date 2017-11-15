//#pragma once
//#include "Shader.hpp"
//#include "Camera.hpp"
//#include"Shaderprogram.hpp"
//#include"geometry.hpp"
//#include"volumgeometry.hpp"
//#include <iostream>
//#include<vector>
//#include<algorithm>
//#include <imdebuggl.h>
//class HybridCaster {
//public:
//	CFrameBuffer* fb;
//	Shader* meshshader = NULL;
//	Shader* volumebfshader = NULL;
//	Shader* rcshader = NULL;
//	GLfloat* meshrgb = NULL;
//	GLfloat* meshdepth = NULL;
//	std::vector<Geometry*> meshHandle;
//	VolumeGeometry* volumeHandle;
//	Camera M_c;
//	void setbfshader(const GLchar* vertexPath, const GLchar* fragmentPath) {
//		if (volumebfshader)
//			delete volumebfshader;
//		volumebfshader = new Shader(vertexPath, fragmentPath);
//	}
//	void setmeshshader(const GLchar* vertexPath, const GLchar* fragmentPath) {
//		if (meshshader)
//			delete meshshader;
//		meshshader = new Shader(vertexPath, fragmentPath);
//	}
//	void setrcshader(const GLchar* vertexPath, const GLchar* fragmentPath) {
//		if (rcshader)
//			delete rcshader;
//		rcshader = new Shader(vertexPath, fragmentPath);
//	}
//	void render();
//	void init();
//	void reshape(int w, int h);
//	void Linkshader(glm::mat4* model);//setup transform matrix
//	void setShader(Shader* s) {
//		if (!M_s) {
//			M_s = new ShaderProgram();
//		}
//		M_s->setShader(s);
//	}
//	void setvolume(VolumeGeometry* s);
//	void keyboard(unsigned char key, int x, int y);
//	void drawRayPosition();
//	~HybridCaster() {
//		if (volumebfshader) delete volumebfshader;
//		if (rcshader) delete rcshader;
//		if (meshshader) delete meshshader;
//		if (meshrgb)delete meshrgb;
//		//if (volumedepth) delete volumedepth;
//		if (meshdepth)delete meshdepth;
//		if (fb)delete fb;
//		if (M_s)delete M_s;
//		for (int i = 0; i < meshHandle.size(); i++)delete meshHandle[i];
//	}
//	ShaderProgram* M_s = NULL;//current shader
//};