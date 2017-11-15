

#include <iostream>
#include<vector>
#include <algorithm>
using namespace std;
#include "CFrameBuffer.h" 
#include <cmath>

// GLEW
#include <glew.h>

// GLFW
//#include <glfw3.h>
#include<freeglut.h>
// GLM Mathematics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/transform2.hpp>
#include <imdebuggl.h>
// Other includes
#include "Shader.hpp"
#include "Camera.hpp"
#include"Shaderprogram.hpp"
#include"geometry.hpp"
#include"volumgeometry.hpp"
#include"hybridrendering.hpp"


//#define NORMAL_CASTING 1

using namespace std;
using glm::mat4;
using glm::vec3;
using glm::vec2;

int xiSize = 95, yiSize = 100, ziSize = 161;
int g_texWidth = 640;
int g_texHeight = 640;

//first implement a normal shading class and shade a volume


HybridRendering Renderer;
void render() {
	//Renderer.rendermesh();
	Renderer.render();
	//glutSwapBuffers();
}
void reshape(int w, int h) {
	Renderer.reshape(w, h);
}
void keyboard(unsigned char key,int x,int y) {
	Renderer.keyboard(key, x, y);
	Renderer.render();
	//Renderer.rendermesh();
}
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutCreateWindow("GLUT Test");
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	
	glutKeyboardFunc(&(keyboard));
	glutDisplayFunc(&(render));
	glutReshapeFunc(&(reshape));
//	glutIdleFunc(&rotateDisplay);
	glViewport(0, 0, 640, 640);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	Renderer.init();
	 // initialize MESH AND VOLUME
	//Renderer.glInfo();
//	Renderer.initmesh("E:/vtk_project/opengl_projectx86/main_project/UL4.obj");	
	
	ifstream f;
	f.open("D:\\volume\\axis_determined\\silicon_mesh\\whole_file_input.matr", ios::in);
	if (!f.is_open()) {
		cout << "Erro when Loading Matrix File" << endl;
		return false;
	}
	//glm::mat4 ma2 = glm::rotate(3.14f,glm::vec3(1,0,0));
	//glm::mat4 ma = glm::rotate(3.14f, glm::vec3(1, 0, 0));
	//ma *= glm::rotate(-3.14f /2.0f, glm::vec3(0,0,1));
	/*ma*=	glm::rotate(274.283f*3.14f /  180, glm::vec3(0, 0, 1));
	 ma *= glm::rotate(9.003f*3.14f / 180, glm::vec3(0, 1, 0));
	 ma *=  glm::rotate(359.247f*3.14f / 180, glm::vec3(1, 0, 0));
	ma *= ma*glm::translate(glm::vec3(2.0196,-24.7619,-24.5565));*/
	glm::mat4 ma;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			f >> ma[j][i];
		}
	}
	//Renderer.M_c.setscale(8);
	//ma =ma* ma2;
	//ma = glm::inverse(ma);

	VolumeGeometry volume("E:/vtk_project/opengl_projectx86/main_project/slf.raw", 512, 512, 507, "E:/vtk_project/opengl_projectx86/main_project/heart.tfi");
	////VolumeGeometry volume("D:/volume/axis_determined/slf_cbct", "E:/vtk_project/opengl_projectx86/main_project/transfer_function.tfi",true);
	
	Renderer.bmax[0] = std::max(Renderer.bmax[0], 76.65f);
	Renderer.bmax[1] = std::max(Renderer.bmax[1], 76.65f);
	Renderer.bmax[2] = std::max(Renderer.bmax[2], 75.8988f);
	Renderer.bmin[0] = std::min(Renderer.bmin[0], -76.65f);
	Renderer.bmin[1] = std::min(Renderer.bmin[1], -76.65f);
	Renderer.bmin[2] = std::min(Renderer.bmin[2], -75.8988f);
	volume.setscale(76.65, 76.65, 75.8988);
	Renderer.setvolume(&volume);
	volume.initVAO();
#if NORMAL_CASTING!=1
Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UL1.obj",&ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UL2.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UL3.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UL4.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UL5.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UL6.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UL7.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UR1.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UR2.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UR3.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UR4.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UR5.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UR6.obj", &ma);
	Renderer.initmesh("D:/volume/axis_determined/silicon_mesh/UR7.obj", &ma);

#endif
	
	////VolumeGeometry volume("E:/vtk_project/opengl_projectx86/main_project/tooth_crop_95100161.raw", xiSize, yiSize, ziSize, "E:/vtk_project/opengl_projectx86/main_project/transfer_function.tfi");
	

	//volume.m_local = glm::rotate(3.1415f / 2, glm::vec3(0, 0, 1))*glm::rotate(-3.1415f / 2, glm::vec3(0, 1, 0));
	////Renderer.M_c.rotateX(-2);
#if NORMAL_CASTING==1
	Renderer.regularcamera();
	Renderer.setbfshader("E:/vtk_project/opengl_projectx86/main_project/backface.vert", "E:/vtk_project/opengl_projectx86/main_project/backface.frag");
	Renderer.setrcshader("E:/vtk_project/opengl_projectx86/main_project/normcasting.vert", "E:/vtk_project/opengl_projectx86/main_project/normcasting.frag");
	//Renderer.regularcamera();
	Renderer.initnormraycating();
#else
	Renderer.setbfshader("E:/vtk_project/opengl_projectx86/main_project/backface.vert", "E:/vtk_project/opengl_projectx86/main_project/backface.frag");
	Renderer.setmeshshader("E:/vtk_project/opengl_projectx86/main_project/mesh.vert", "E:/vtk_project/opengl_projectx86/main_project/mesh.frag");
	Renderer.setrcshader("E:/vtk_project/opengl_projectx86/main_project/raycasting.vert", "E:/vtk_project/opengl_projectx86/main_project/bighybridraycasting.frag");
	Renderer.regularcamera();
	Renderer.inithybridcasting();
#endif // NORMAL_CASING

	glutMainLoop();

	system("pause");
	return EXIT_SUCCESS;
}


void rotateDisplay() {
	//g_angle = (g_angle + 1) % 3600;
	glutPostRedisplay();
}
