

#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>



// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const GLfloat YAW = 0.0f;
const GLfloat PITCH = 90.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll;
	GLfloat Scale=1;
	// Camera options
	GLfloat translatex=0;
	GLfloat translatey=0;
	GLfloat translatez=0;
	GLfloat Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, 0.0f)), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Up = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		//this->updateCameraVectors();
		Roll = 0;
	}
	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Up = WorldUp;
		this->Yaw = yaw;
		this->Pitch = pitch;
		Roll = 0;
		//this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix(glm::mat4* usermatrix) {
		glm::mat4 view = glm::lookAt(this->Position, this->Front, this->Up);
		
		view *= glm::rotate(-Pitch, glm::vec3(1.0, 0.0, 0.0));
		view *= glm::rotate(-Yaw, glm::vec3(0.0, 1.0, 0.0));
		view *= glm::rotate(-Roll, glm::vec3(0.0, 0.0, 1.0));
		//view *= glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f));
		view *= glm::scale(glm::vec3((float)Scale, (float)Scale, (float)Scale));//*glm::id;
		view *= glm::translate(glm::vec3(translatex, translatey, translatez));view *= *usermatrix;
		return view;
	}
	glm::mat4 GetViewMatrix()
	{
		glm::mat4 view = glm::lookAt(this->Position,this->Front, this->Up);

		view *= glm::rotate(-Pitch, glm::vec3(1.0, 0.0, 0.0));
		view *= glm::rotate(-Yaw, glm::vec3(0.0, 1.0, 0.0));
		view *= glm::rotate(-Roll, glm::vec3(0.0, 0.0, 1.0));
		//view *= glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f));
		view *= glm::scale(glm::vec3((float)Scale, (float)Scale, (float)Scale));//*glm::id;
		view *= glm::translate(glm::vec3(translatex, translatey, translatez));
		return view;
	}
	void setscale(float x) { Scale = x; }
	void rotateX(float angle) { Pitch += angle/10; }
	void rotateY(float angle) { Yaw += angle/10; }
	void rotateZ(float angle) { Roll += angle/10; }
	void translate(glm::vec3 x) {translatex = x[0]; translatey = x[1];
	translatez = x[2];
	}
private:

};

