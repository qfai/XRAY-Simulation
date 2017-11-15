#include <iostream>  
#include "CFrameBuffer.h"  
CFrameBuffer::CFrameBuffer()
{
	m_FboID = 0;
	m_RboID = 0;
	//m_tex = 0;
	m_bIsBegined = false;
	m_width = 0;
	m_height = 0;
	m_curbuff = 0;
}

CFrameBuffer::~CFrameBuffer()
{
	if (m_bIsBegined)
	{
		end();
		m_bIsBegined = false;
	}
	for(std::vector<GLuint>::iterator it=m_tex.begin();it!=m_tex.end();it++)
		glDeleteTextures(1, &(*it));
	glDeleteRenderbuffers(1, &m_RboID);
	glDeleteFramebuffers(1, &m_FboID);
}

void CFrameBuffer::init(int width, int height)
{
	glewInit();
	m_width = width;
	m_height = height;

	GLuint tex;
	GL_ERROR();

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_tex.push_back(tex);
	//glDisable(GL_TEXTURE_2D);

	GL_ERROR();

	//glEnable(GL_RENDERBUFFER);
	//GL_ERROR();
	glGenRenderbuffers(1, &m_RboID);
	GL_ERROR();
	glBindRenderbuffer(GL_RENDERBUFFER, m_RboID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	GL_ERROR();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glDisable(GL_RENDERBUFFER);

	GL_ERROR();


	//glEnable(GL_FRAMEBUFFER);
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RboID);

	GL_ERROR();

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "Framebuffer complete." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
			break;

		default:
			std::cout << "[ERROR] Unknow error." << std::endl;
			break;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_FRAMEBUFFER);

	m_curbuff = 0;
}

bool CFrameBuffer::begin()
{
	if (m_bIsBegined)
	{
		return false;
	}
	else
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);  
		//glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&m_curbuff);  
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_FboID);  

		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &m_curbuff);
		glPushAttrib(GL_VIEWPORT_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FboID);
		//cout<<"begin after : GL_FRAMEBUFFER_BINDING_EXT = "<<FboId<<endl;  
		glViewport(0, 0, m_width, m_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, m_width, 0, m_height, -1000, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_bIsBegined = true;
		return true;
	}
}

bool CFrameBuffer::end()
{
	if (m_bIsBegined)
	{
		//glPopAttrib();  
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_curbuff);  
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_curbuff);
		m_bIsBegined = false;
		return true;
	}
	else
	{
		return false;
	}
}

void CFrameBuffer::saveFrameBuff(const char* fileName)
{
//	IplImage* pImage = cvCreateImage(cvSize(m_width, m_height), 8, 3);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, m_tex);
//	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, pImage->imageData);
//	glDisable(GL_TEXTURE_2D);

//	cvFlip(pImage, NULL, 0);
//	cvSaveImage(fileName, pImage);
//	cvReleaseImage(&pImage);
}

void CFrameBuffer::AttachTextures()
{
	if (m_tex.size() >= 15)
	{
		std::cout << "framebuffer is full" << std::endl;
	}
	GLuint tex;

		GL_ERROR();

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		GL_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_tex.push_back(tex);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_tex.size() - 1, GL_TEXTURE_2D, tex, 0);

		GL_ERROR();

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			switch (status)
			{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				std::cout << "Framebuffer complete." << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
				break;

			default:
				std::cout << "[ERROR] Unknow error." << std::endl;
				break;
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
