#pragma once
#include "Shader.hpp"
#include "Camera.hpp"
#include"Shaderprogram.hpp"
#include"geometry.hpp"
#include"volumgeometry.hpp"
#include <iostream>
#include<vector>
#include<algorithm>
#include <imdebuggl.h>
#define NORMAL_CASTING 0
#define IMDEBUG 0
class HybridRendering {
public:
	float bmin[3];
	float bmax[3];
	HybridRendering() :M_c((glm::vec3(0.0f, 0.0f, 3.0f))){
		bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
		bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();
	}
	HybridRendering(int winw, int winh) :g_winWidth(winw), g_winHeight(winh), M_c((glm::vec3(0.0f, 0.0f, 3.0f))) {
		bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
		bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();
	}
	CFrameBuffer* fb;
	Shader* volumebfshader=NULL;
	Shader* rcshader = NULL;
	Shader* meshshader = NULL;
	Camera M_c;
	GLuint twodtex[2];
	GLuint rgbtex[2];
	GLfloat* meshrgb=NULL;
	GLfloat* meshrgbback = NULL;
	//GLfloat* volumedepth=NULL;
	GLfloat* meshdepthback = NULL;
	GLfloat* meshdepth=NULL;
	vector<Geometry*> meshHandle;
	VolumeGeometry* volumeHandle;
	void setbfshader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		if (volumebfshader)
			delete volumebfshader;
		volumebfshader = new Shader(vertexPath, fragmentPath);
	}
	void setmeshshader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		if (meshshader)
			delete meshshader;
		meshshader = new Shader(vertexPath, fragmentPath);
	}
	void setrcshader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		if (rcshader)
			delete rcshader;
		rcshader = new Shader(vertexPath, fragmentPath);
	}
	void render();
	void rendermesh();
	void init();
	void reshape(int w, int h);
	void Linkshader(glm::mat4* model);//setup transform matrix
	void setShader(Shader* s) { 
		if (!M_s) {
			M_s = new ShaderProgram();
		}
		M_s->setShader( s); }
	void setvolume(VolumeGeometry* s);
	GLboolean compileCheck(GLuint shader);
	void keyboard(unsigned char key, int x, int y);
	void drawRayPosition();
	~HybridRendering() {
		if (volumebfshader) delete volumebfshader;
		if (rcshader) delete rcshader;
		if (meshshader) delete meshshader;
		if (meshrgb)delete meshrgb;
		//if (volumedepth) delete volumedepth;
		if (meshdepth)delete meshdepth;
		if (fb)delete fb;
		if (M_s)delete M_s;
		for (int i = 0; i < meshHandle.size(); i++)delete meshHandle[i];
	}
	ShaderProgram* M_s=NULL;//current shader
	void regularcamera();
	void initmesh(const char* path);
	void initmesh(const char* path,glm::mat4 *mat);
	void initnormraycating();
	void inithybridcasting();
	void normcasingrender();
	void hybridcastingrender();
	GLuint inittex(GLuint dimension, GLuint type, GLuint datatype, GLuint bfTexWidth, GLuint bfTexHeight, GLfloat *depth = NULL);
	GLuint initFace2DTex(GLuint bfTexWidth, GLuint bfTexHeight);
	GLuint initColorTex(GLuint bfTexWidth, GLuint bfTexHeight, GLfloat *rgb);
	GLuint initDepthTex(GLuint bfTexWidth, GLuint bfTexHeight, GLfloat *depth);
private:
	float g_stepSize = 0.001f;
protected:
	void addmesh(Geometry* s) { meshHandle.push_back(s); }
	
GLuint g_winWidth = 640;
GLuint g_winHeight = 640;
};
GLuint HybridRendering::inittex(GLuint dimension, GLuint type, GLuint datatype, GLuint bfTexWidth, GLuint bfTexHeight, GLfloat * depth)
{
	GLuint backFace2DTex;
	glGenTextures(1, &backFace2DTex);
	GL_ERROR();
	glBindTexture(dimension, backFace2DTex);
	glTexParameteri(dimension, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(dimension, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(dimension, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(dimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL_ERROR();
	if (depth)
		glTexImage2D(dimension, 0, type, bfTexWidth, bfTexHeight, 0, type, datatype, depth);
	else
		glTexImage2D(dimension, 0, type, bfTexWidth, bfTexHeight, 0, type, datatype, NULL);
	glBindTexture(dimension, 0);
	GL_ERROR();
	return backFace2DTex;

}
void HybridRendering::setvolume(VolumeGeometry * s)
{
	for (int kk = 0; kk < 3; kk++) {
		bmin[kk] = (std::min)(bmin[kk], s->bmin[kk]);
		bmax[kk] = (std::max)(bmax[kk], s->bmax[kk]);
		s->bmin[kk] = bmin[kk];
		s->bmax[kk] = bmax[kk];
	}
	volumeHandle = s;
}

// check the compilation result
GLboolean  HybridRendering::compileCheck(GLuint shader) {
	GLint err;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
	if (GL_FALSE == err) {
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char* log = (char *)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);
			cerr << "Shader log: " << log << endl;
			free(log);
		}
	}
	return err;
}



// init the 2D texture for render backface 'bf' stands for backface
GLuint HybridRendering::initFace2DTex(GLuint bfTexWidth, GLuint bfTexHeight) {
	GLuint backFace2DTex=inittex(GL_TEXTURE_2D,GL_RGBA16F, GL_FLOAT, bfTexWidth, bfTexHeight);
	return backFace2DTex;
}

// initialize depth texture of mesh
GLuint HybridRendering::initDepthTex(GLuint bfTexWidth, GLuint bfTexHeight, GLfloat *depth) {
	GLuint depthTex = inittex(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_FLOAT,bfTexWidth,bfTexHeight, depth);
	return depthTex;
}


// initialize visble color texture of mesh
GLuint HybridRendering::initColorTex(GLuint bfTexWidth, GLuint bfTexHeight, GLfloat *rgb) {
	GLuint colorTex = inittex(GL_TEXTURE_2D, GL_RGBA, GL_FLOAT, bfTexWidth, bfTexHeight, rgb);
	return colorTex;
}

// init 3D texture to store the volume data used fo ray casting

void HybridRendering::keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '\x1B':
		exit(EXIT_SUCCESS);
		break;
	case 'X':
		//g_angle = (g_angle + 1) % 360;
		M_c.rotateX(1);
		break;
	case 'x':
		//g_angle = (g_angle - 1) % 360;
		M_c.rotateX(-1);
		break;
	case 'Y':
		//g_angle = (g_angle + 1) % 360;
		M_c.rotateY(1);
		break;
	case 'y':
		//g_angle = (g_angle - 1) % 360;
		M_c.rotateY(-1);
		break;
	case 'Z':
		//g_angle = (g_angle + 1) % 360;
		M_c.rotateZ(1);
		break;
	case 'z':
		//g_angle = (g_angle - 1) % 360;
		M_c.rotateZ(-1);
		break;
	}
#if NORMAL_CASTING==1
	
	initnormraycating();
	
#else
	inithybridcasting();
#endif
}

void HybridRendering::drawRayPosition()
{
	//Shader rayposition("E:/vtk_project/opengl_project/main_project/raypoisition.vert", "E:/vtk_project/opengl_project/main_project/raypoisition.frag");
	setShader(volumebfshader);
	Linkshader(&(volumeHandle->m_local));
	GLint scaleLoc = glGetUniformLocation(M_s->program(), "scale");
	GL_ERROR();
	if (scaleLoc >= 0) {
		glUniform3f(scaleLoc, volumeHandle->scale.x, volumeHandle->scale.y, volumeHandle->scale.z);
	}
	else {
		cout << "scale " << "is not bind to the uniform" << endl;
	}

	if(fb->gettexsize()<2)
		fb->AttachTextures();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->getFb());

	glViewport(0, 0, g_winWidth, g_winHeight);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	volumeHandle->drawBox(GL_BACK);

	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDepthFunc(GL_GREATER);
	volumeHandle->drawBox(GL_FRONT);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
#if IMDEBUG==1
	imdebugTexImagef(GL_TEXTURE_2D, fb->getTex(0), GL_RGBA);
	imdebugTexImagef(GL_TEXTURE_2D, fb->getTex(1), GL_RGBA);
#endif
	glDepthFunc(GL_LESS);
//	setShader(NULL);
}

void HybridRendering::regularcamera()
{
	/*for (int i = 0; i < 3; i++) {
		bmin[i] = 0;
		bmax[i] = 1;
	}*/
		float maxExtent = 0.5f * (bmax[0] - bmin[0]);
	if (maxExtent < 0.5f * (bmax[1] - bmin[1])) {
		maxExtent = 0.5f * (bmax[1] - bmin[1]);
	}
	if (maxExtent < 0.5f * (bmax[2] - bmin[2])) {
		maxExtent = 0.5f * (bmax[2] - bmin[2]);
	}
	 glScalef(1.0f / maxExtent, 1.0f / maxExtent, 1.0f / maxExtent);

    // Centerize object.
    glTranslatef(-0.5 * (bmax[0] + bmin[0]), -0.5 * (bmax[1] + bmin[1]),
                 -0.5 * (bmax[2] + bmin[2]));
	M_c.setscale(1.0f / maxExtent);
	M_c.translate(glm::vec3(-0.5 * (bmax[0] + bmin[0]), -0.5 * (bmax[1] + bmin[1]),
		-0.5 * (bmax[2] + bmin[2])));
}

void HybridRendering::initmesh(const char * path)
{
		Geometry* gtest = new Geometry();
		if (gtest->readObjData(path, 1))
		{
			gtest->initVBO();
			addmesh(gtest);
		}
		for (int kk = 0; kk < 3; kk++) {
			bmin[kk] = (std::min)(bmin[kk], gtest->bmin[kk]);
			bmax[kk] = (std::max)(bmax[kk], gtest->bmax[kk]);
		}
	
}

inline void HybridRendering::initmesh(const char * path, glm::mat4* mat)
{
	Geometry* gtest = new Geometry();
	if (gtest->readObjData(path, 1))
	{
		gtest->initVBO();
		addmesh(gtest);
	}
	for (int kk = 0; kk < 3; kk++) {
		bmin[kk] = (std::min)(bmin[kk], gtest->bmin[kk]);
		bmax[kk] = (std::max)(bmax[kk], gtest->bmax[kk]);
	}
	gtest->m_local =glm::mat4( *mat);
}

void HybridRendering::initnormraycating()
{
	if (!rcshader | !volumebfshader) {
		cout << "rcshader or volumebfshader didn't set " << endl;
	}
	drawRayPosition();

	setShader(rcshader);
	Linkshader(&(volumeHandle->m_local));
	//M_s->Use();
	GLint entryFaceLoc = glGetUniformLocation(M_s->program(), "EntryPoints");
	if (entryFaceLoc >= 0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, fb->getTex(0));
		glUniform1i(entryFaceLoc, 2);
	}
	else {
		cout << "EntryPoints " << "is not bind to the uniform" << endl;
	}
	GL_ERROR();
	GLint backFaceLoc = glGetUniformLocation(M_s->program(), "ExitPoints");
	if (backFaceLoc >= 0) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, fb->getTex(1));
		glUniform1i(backFaceLoc, 3);
	}
	else {
		cout << "ExitPoints " << "is not bind to the uniform" << endl;
	}
	GLint volumeLoc = glGetUniformLocation(M_s->program(), "VolumeTex");
	if (volumeLoc >= 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, volumeHandle->datatex);
		glUniform1i(volumeLoc, 0);
	}
	else {
		cout << "VolumeTex " << "is not bind to the uniform" << endl;
	}
	GLint transferFuncLoc = glGetUniformLocation(M_s->program(), "TransferFunc");
	if (transferFuncLoc >= 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D, volumeHandle->TFF1dtex);
		glUniform1i(transferFuncLoc, 1);
	}
	else {
		cout << "TransferFunc " << "is not bind to the uniform" << endl;
	}
	GLint stepSizeLoc = glGetUniformLocation(M_s->program(), "StepSize");
	GL_ERROR();
	if (stepSizeLoc >= 0) {
		glUniform1f(stepSizeLoc, g_stepSize);
	}
	else {
		cout << "StepSize " << "is not bind to the uniform" << endl;
	}
	GL_ERROR();
	GLint lightColorLoc = glGetUniformLocation(M_s->program(), "lightColor");
	if (lightColorLoc >= 0) {
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightColor" << "is not bind to the uniform" << endl;
	}

	GLint lightPosLoc = glGetUniformLocation(M_s->program(), "lightPos");
	if (lightPosLoc >= 0) {
		glUniform3f(lightPosLoc, 2.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightPos" << "is not bind to the uniform" << endl;
	}

	GLint viewPosLoc = glGetUniformLocation(M_s->program(), "viewPos");
	if (viewPosLoc >= 0) {
		glUniform3f(viewPosLoc, 0.0f, 0.0f, 2.0f);
	}
	else {
		cout << "viewPos" << "is not bind to the uniform" << endl;
	}
	GLint screenSizeLoc = glGetUniformLocation(M_s->program(), "DimensionSize");
	if (screenSizeLoc >= 0) {
		glUniform3f(screenSizeLoc, (float)volumeHandle->xiSize, (float)volumeHandle->yiSize, (float)volumeHandle->ziSize);
	}
	else {
		cout << "ScreenSize" << "is not bind to the uniform" << endl;
	}
}

void HybridRendering::inithybridcasting()
{	if (!rcshader | !volumebfshader | !meshshader) {
		cout << "shader is not ready" << endl;
		exit(0);
	}
	

	GL_ERROR();
	setShader(meshshader);
	Linkshader(&(meshHandle[0]->m_local));
	GLint lightColorLoc = glGetUniformLocation(M_s->program(), "lightColor");
	if (lightColorLoc >= 0) {
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightColor" << "is not bind to the uniform" << endl;
	}

	GLint lightPosLoc = glGetUniformLocation(M_s->program(), "lightPos");
	if (lightPosLoc >= 0) {
		glUniform3f(lightPosLoc, 2.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightPos" << "is not bind to the uniform" << endl;
	}

	GLint viewPosLoc = glGetUniformLocation(M_s->program(), "viewPos");
	if (viewPosLoc >= 0) {
		glUniform3f(viewPosLoc, 0.0f, 0.0f, 2.0f);
	}
	else {
		cout << "viewPos" << "is not bind to the uniform" << endl;
	}
	//fb->AttachTextures();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (vector<Geometry*>::iterator it = meshHandle.begin(); it != meshHandle.end(); it++)
		(*it)->draw(GL_BACK);
	if(!meshdepth)
		meshdepth = (GLfloat *)calloc(g_winWidth * g_winHeight, sizeof(GLfloat));
	glReadPixels(0, 0, g_winWidth, g_winHeight, GL_DEPTH_COMPONENT, GL_FLOAT, meshdepth);
	twodtex[0]= initDepthTex(g_winWidth, g_winHeight, meshdepth);
	if (!meshrgb)
		meshrgb= (GLfloat *)calloc(g_winWidth * g_winHeight * 4, sizeof(GLfloat));
	glReadPixels(0, 0, g_winWidth, g_winHeight, GL_RGBA, GL_FLOAT, meshrgb);
	rgbtex[0] = initColorTex(g_winWidth, g_winHeight, meshrgb);
	//glCopyBufferSubData(fb->getDepTez(), fb->getTex(1),0, 0,fb->getsize());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_ERROR();
	for (vector<Geometry*>::iterator it = meshHandle.begin(); it != meshHandle.end(); it++)
		(*it)->draw(GL_FRONT);
	GL_ERROR();
	if(!meshdepthback)
		meshdepthback = (GLfloat *)calloc(g_winWidth * g_winHeight, sizeof(GLfloat));
	glReadPixels(0, 0, g_winWidth, g_winHeight, GL_DEPTH_COMPONENT, GL_FLOAT, meshdepthback);
	twodtex[1] = initDepthTex(g_winWidth, g_winHeight, meshdepthback);
	if(!meshrgbback)
		meshrgbback = (GLfloat *)calloc(g_winWidth * g_winHeight * 4, sizeof(GLfloat));
	glReadPixels(0, 0, g_winWidth, g_winHeight, GL_RGBA, GL_FLOAT, meshrgbback);
	rgbtex[1] = initColorTex(g_winWidth, g_winHeight, meshrgbback);
	drawRayPosition();
	/*setShader(volumebfshader);
	Linkshader(&(volumeHandle->m_local));
	if(fb->gettexsize()<2)
		fb->AttachTextures();
	glBindFramebuffer(GL_FRAMEBUFFER, fb->getFb());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->getFb());
	glViewport(0, 0, g_winWidth, g_winHeight);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	volumeHandle->drawBox(GL_BACK);
	GL_ERROR();
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_GREATER);
	volumeHandle->drawBox(GL_FRONT);
	glDepthFunc(GL_LESS);
	GL_ERROR();*/
	/*glBindBuffer(GL_COPY_READ_BUFFER, fb->getDepTez());
	glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
	glBindBuffer(GL_COPY_WRITE_BUFFER, volumeHandle->twodtex[1]);
	glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);*/
#if IMDEBUG==1
	//imdebugTexImagef(GL_TEXTURE_2D, rgbtex[0], GL_RGBA);
	//imdebugTexImagef(GL_TEXTURE_2D, rgbtex[1], GL_RGBA);
	//imdebugTexImagef(GL_TEXTURE_2D, twodtex[0] /*initDepthTex(g_winWidth, g_winHeight, volumedepth)*/, GL_LUMINANCE);
//	imdebugTexImagef(GL_TEXTURE_2D, twodtex[1], GL_LUMINANCE);
	/*imdebugTexImagef(GL_TEXTURE_2D, fb->getDepTez(), GL_LUMINANCE);
	imdebugTexImagef(GL_TEXTURE_2D, fb->getTex(0), GL_RGBA);
	imdebugTexImagef(GL_TEXTURE_2D, fb->getTex(1), GL_RGBA);*/
#endif

	//if (depthData)free( depthData);
	GL_ERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_ERROR();
	//volume back
	

	setShader(rcshader);
	Linkshader(&(volumeHandle->m_local));
	lightColorLoc = glGetUniformLocation(M_s->program(), "lightColor");
	if (lightColorLoc >= 0) {
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightColor" << "is not bind to the uniform" << endl;
	}

	lightPosLoc = glGetUniformLocation(M_s->program(), "lightPos");
	if (lightPosLoc >= 0) {
		glUniform3f(lightPosLoc, 2.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightPos" << "is not bind to the uniform" << endl;
	}

	viewPosLoc = glGetUniformLocation(M_s->program(), "viewPos");
	if (viewPosLoc >= 0) {
		glUniform3f(viewPosLoc, 0.0f, 0.0f, 2.0f);
	}
	else {
		cout << "viewPos" << "is not bind to the uniform" << endl;
	}

	GLint screenSizeLoc = glGetUniformLocation(M_s->program(), "DimensionSize");
	if (screenSizeLoc >= 0) {
		glUniform3f(screenSizeLoc, (float)volumeHandle->xiSize, (float)volumeHandle->yiSize, (float)volumeHandle->ziSize);
	}
	else {
		cout << "ScreenSize" << "is not bind to the uniform" << endl;
	}

	GLint stepSizeLoc = glGetUniformLocation(M_s->program(), "StepSize");
	GL_ERROR();
	if (stepSizeLoc >= 0) {
		glUniform1f(stepSizeLoc, g_stepSize);
	}
	else {
		cout << "StepSize" << "is not bind to the uniform" << endl;
	}
	GL_ERROR();

	GLint transferFuncLoc = glGetUniformLocation(M_s->program(), "TransferFunc");
	if (transferFuncLoc >= 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, volumeHandle->TFF1dtex);
		glUniform1i(transferFuncLoc, 0);
	}
	else {
		cout << "TransferFunc" << "is not bind to the uniform" << endl;
	}
	GL_ERROR();
	GLint entryFaceLoc = glGetUniformLocation(M_s->program(), "EntryPoints");
	if (entryFaceLoc >= 0) {
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, fb->getTex(0));
		glUniform1i(entryFaceLoc, 7);
	}
	else {
		cout << "EntryPoints " << "is not bind to the uniform" << endl;
	}
	GL_ERROR();
	GLint backFaceLoc = glGetUniformLocation(M_s->program(), "ExitPoints");
	if (backFaceLoc >= 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fb->getTex(1));
		glUniform1i(backFaceLoc, 1);
	}
	else {
		cout << "ExitPoints " << "is not bind to the uniform" << endl;
	}
	
	GL_ERROR();

	GLint volumeLoc = glGetUniformLocation(M_s->program(), "VolumeTex");
	if (volumeLoc >= 0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_3D, volumeHandle->datatex);
		glUniform1i(volumeLoc, 2);
	}
	else {
		cout << "VolumeTex" << "is not bind to the uniform" << endl;
	}

	lightColorLoc = glGetUniformLocation(M_s->program(), "lightColor");
	if (lightColorLoc >= 0) {
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightColor" << "is not bind to the uniform" << endl;
	}

	lightPosLoc = glGetUniformLocation(M_s->program(), "lightPos");
	if (lightPosLoc >= 0) {
		glUniform3f(lightPosLoc, 2.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightPos" << "is not bind to the uniform" << endl;
	}

	viewPosLoc = glGetUniformLocation(M_s->program(), "viewPos");
	if (viewPosLoc >= 0) {
		glUniform3f(viewPosLoc, 0.0f, 0.0f, 2.0f);
	}
	else {
		cout << "viewPos" << "is not bind to the uniform" << endl;
	}

	GLint depthLoc = glGetUniformLocation(M_s->program(), "MeshDepth");
	if (depthLoc >= 0) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, twodtex[0]);
		glUniform1i(depthLoc, 3);
	}
	else {
		/*if (depthData) {
		free(rgbData);
		}*/
		cout << "depthRecord" << "is not bind to the uniform" << endl;
	}

	GLint colorLoc = glGetUniformLocation(M_s->program(), "MeshColor");
	if (colorLoc >= 0) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, rgbtex[0]);
		glUniform1i(colorLoc, 4);
	}
	else {
		/*if (depthData) {
		free(rgbData);
		}*/
		cout << "depthRecord" << "is not bind to the uniform" << endl;
	}

	GLint depthBackLoc = glGetUniformLocation(M_s->program(), "MeshDepthBack");
	if (depthBackLoc >= 0) {
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, twodtex[1]);
		glUniform1i(depthBackLoc, 5);
	}
	else {
		cout << "depthBackRecord" << "is not bind to the uniform" << endl;
	}

	GLint colorBackLoc = glGetUniformLocation(M_s->program(), "MeshColorBack");
	if (colorBackLoc >= 0) {
		glActiveTexture(GL_TEXTURE6);
		//rgbTexIdx = initColorTex(g_winWidth, g_winHeight, rgbDataBack);
		glBindTexture(GL_TEXTURE_2D, rgbtex[1]);
		glUniform1i(colorBackLoc, 6);
	}
	else {
		/*if (depthData) {
		free(rgbData);
		}*/
		cout << "depthBackRecord" << "is not bind to the uniform" << endl;
	}

}

void HybridRendering::normcasingrender()
{
	//draw normal volume
	//drawRayPosition();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_ERROR();
	volumeHandle->drawBox(GL_FRONT);
	GL_ERROR();
	//get front face 
	//get back face
	//draw


	glutSwapBuffers();
}

void HybridRendering::hybridcastingrender()
{
	//draw normal volume
	//drawRayPosition();
	//regularcamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_ERROR();
	volumeHandle->drawBox(GL_BACK);
	//	GL_ERROR();
	//get front face 
	//get back face
	//draw


	//////fb->AttachTextures();
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->getFb());
	//glViewport(0, 0, g_winWidth, g_winHeight);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//GL_ERROR();
	//for (vector<Geometry*>::iterator it = meshHandle.begin(); it != meshHandle.end(); it++)
	//	(*it)->draw(GL_FRONT);
	//GL_ERROR();
	//imdebugTexImagef(GL_TEXTURE_2D, fb->getDepTez(), GL_LUMINANCE);//need to read depth out or use another framebuffer

	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	////glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb2->getFb());
	////glViewport(0, 0, g_winWidth, g_winHeight);
	////glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//GL_ERROR();
	//for (vector<Geometry*>::iterator it = meshHandle.begin(); it != meshHandle.end(); it++)
	//	(*it)->draw(GL_BACK);
	//GL_ERROR();
	//meshHandle[0]->depthData= (GLfloat *)calloc(g_winWidth * g_winHeight, sizeof(GLfloat));
	////imdebugTexImagef(GL_TEXTURE_2D, 0, GL_LUMINANCE);
	//glReadPixels(0, 0, g_winWidth, g_winHeight, GL_DEPTH_COMPONENT, GL_FLOAT, meshHandle[0]->depthData);
	////imdebugTexImagef(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_LUMINANCE);
	//

	////imdebugTexImagef(GL_TEXTURE_2D, fb->getTex(0), GL_RGBA);//this have things
	////imdebugTexImagef(GL_TEXTURE_2D, fb2->getTex(0), GL_RGBA);
	////
	////glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glutSwapBuffers();
	//
}

void HybridRendering::reshape(int w, int h) {
	g_winWidth = w;
	g_winHeight = h;
	//g_texWidth = w;
	//g_texHeight = h;
	//rendermesh();
	render();
}

void HybridRendering::Linkshader(glm::mat4 *model)
{//care only about mvp and model which is controlled by camera other loc should be done by user
	M_s->Use();
	GLint mvploc = glGetUniformLocation(M_s->program(), "MVP");
	GLint modelloc = glGetUniformLocation(M_s->program(), "Model");
	glm::mat4 projection = glm::perspective(M_c.Zoom, (GLfloat)g_winWidth / g_winHeight, 0.1f, 400.f);
	glm::mat4 view =  M_c.GetViewMatrix(model);//glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0), glm::vec3(0,1,0));//
	//glm::mat4 model;
	glm::mat4 mvp = projection * view  ;//multiply by right
	glm::mat4 testmvp= { { 1.5,0,0,0},
	{ 0,-0.7,-0.9,-0.9},
	{ 0,-1.38,0.44,0.44},
	{ -0.77,1.03,2.58,2.78 } };
	//testmvp[0] = { 1.5,0,0,0};
	//glm::mat4 testmvp;
	if (mvploc >= 0) {
		glUniformMatrix4fv(mvploc, 1, GL_FALSE, &mvp[0][0]);
	}
	else {
		cerr << "can't get the MVP" << endl;
	}
	if (modelloc >= 0) {
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, &(*model)[0][0]);
	}
	else {
		cerr << "can't get the model" << endl;
	}
}

void HybridRendering::render()
{
#if NORMAL_CASTING==1
	glClearColor(0.8,0.8,0.8,0);
	normcasingrender();
#else
	
	hybridcastingrender();

#endif // NORMAL_CASTING
}
void HybridRendering::rendermesh() {
	if (meshHandle.size() < 1)
		return;
	setShader(meshshader);
	//glm::mat4 tmp;
	Linkshader(&(meshHandle[0]->m_local));
	GLint lightColorLoc = glGetUniformLocation(M_s->program(), "lightColor");
	if (lightColorLoc >= 0) {
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightColor" << "is not bind to the uniform" << endl;
	}

	GLint lightPosLoc = glGetUniformLocation(M_s->program(), "lightPos");
	if (lightPosLoc >= 0) {
		glUniform3f(lightPosLoc, 2.0f, 1.0f, 1.0f);
	}
	else {
		cout << "lightPos" << "is not bind to the uniform" << endl;
	}

	GLint viewPosLoc = glGetUniformLocation(M_s->program(), "viewPos");
	if (viewPosLoc >= 0) {
		glUniform3f(viewPosLoc, 0.0f, 0.0f, 2.0f);
	}
	else {
		cout << "viewPos" << "is not bind to the uniform" << endl;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_ERROR();
	for (vector<Geometry*>::iterator it = meshHandle.begin(); it != meshHandle.end(); it++)
		(*it)->draw(GL_BACK);
	GL_ERROR();
	glutSwapBuffers();
}
void HybridRendering::init()
{
	GL_ERROR();
	fb = new CFrameBuffer();
	fb->init(g_winWidth, g_winHeight);
	GL_ERROR();
}