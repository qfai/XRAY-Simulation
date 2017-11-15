#pragma once
#include<tinyxml2.h>
using namespace tinyxml2;
#include<sstream>
class fixedboundingbox {
	public:
		GLfloat vertices[216]= {
			-1.0, -1.0, 1.0,0.0, 0.0, 1.0,
			1.0, -1.0, 1.0,0.0, 0.0, 1.0,
			1.0, 1.0, 1.0,0.0, 0.0, 1.0,
			1.0, 1.0, 1.0,0.0, 0.0, 1.0,
			-1.0, 1.0, 1.0,0.0, 0.0, 1.0,
			-1.0, -1.0, 1.0,0.0, 0.0, 1.0,
			-1.0, -1.0, -1.0, 0.0, 0.0, -1.0,
			-1.0, 1.0, -1.0, 0.0, 0.0, -1.0,
			1.0, 1.0, -1.0, 0.0, 0.0, -1.0,
			1.0, 1.0, -1.0, 0.0, 0.0, -1.0,
			1.0, -1.0, -1.0, 0.0, 0.0, -1.0,
			-1.0, -1.0, -1.0, 0.0, 0.0, -1.0,
			-1.0, -1.0, -1.0, -1.0, 0.0, 0.0,
			-1.0, -1.0, 1.0, -1.0, 0.0, 0.0,
			-1.0, 1.0, 1.0, -1.0, 0.0, 0.0,
			-1.0, 1.0, 1.0, -1.0, 0.0, 0.0,
			-1.0, 1.0, -1.0, -1.0, 0.0, 0.0,
			-1.0, -1.0, -1.0, -1.0, 0.0, 0.0,
			1.0, 1.0, 1.0,1.0, 0.0, 0.0,
			1.0, -1.0, 1.0,1.0, 0.0, 0.0,
			1.0, -1.0, -1.0,1.0, 0.0, 0.0,
			1.0, -1.0, -1.0,1.0, 0.0, 0.0,
			1.0, 1.0, -1.0,1.0, 0.0, 0.0,
			1.0, 1.0, 1.0,1.0, 0.0, 0.0,
			-1.0, 1.0, -1.0,0.0, 1.0, 0.0,
			-1.0, 1.0, 1.0,0.0, 1.0, 0.0,
			1.0, 1.0, 1.0,0.0, 1.0, 0.0,
			1.0, 1.0, 1.0,0.0, 1.0, 0.0,
			1.0, 1.0, -1.0,0.0, 1.0, 0.0,
			-1.0, 1.0, -1.0,0.0, 1.0, 0.0,
			-1.0, -1.0, 1.0, 0.0, -1.0, 0.0,
			-1.0, -1.0, -1.0, 0.0, -1.0, 0.0,
			1.0, -1.0, -1.0, 0.0, -1.0, 0.0,
			1.0, -1.0, -1.0, 0.0, -1.0, 0.0,
			1.0, -1.0, 1.0, 0.0, -1.0, 0.0,
			-1.0, -1.0, 1.0, 0.0, -1.0, 0.0, };

		void autoscale(float x, float y, float z) {
			for (int i = 0; i < 36; i++) {
				
					vertices[i * 6 + 0] = x*vertices[i * 6 + 0];
					vertices[i * 6 + 1] = y*vertices[i * 6 + 1];
					vertices[i * 6 + 2] = z*vertices[i * 6 + 2];
				
			}
		
		}
	};

class VolumeGeometry {
public:
	float bmin[3] = { 0,0,0 };
	float bmax[3] = { 1,1,1 };
	int xiSize=0, yiSize=0, ziSize=0;
	fixedboundingbox bbox;
	glm::mat4 m_local;
	//saved texture infos
	GLuint vertexdat;//save boudingbox vao
	GLuint TFF1dtex;
	GLuint twodtex[2];
	GLuint datatex;
	GLuint g_vao;
	glm::vec3 scale = glm::vec3(1,1,1);
	//GLubyte* data;//for calc gradient
	unsigned char* pDataTexture;// data handle
	//GLuint g_volTexObj;
	/*void initdepthtex(GLuint h,GLuint w) {
		glGenRenderbuffers(2, &twodtex[0]);
		GL_ERROR();
		glBindRenderbuffer(GL_RENDERBUFFER, twodtex[0]);
		glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		GL_ERROR();
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, twodtex[1]);
		glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		GL_ERROR();
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	}*/
	void initVAO();
	void calculateGradient(int x, int y, int z, double& gx, double& gy, double& gz, GLubyte* data);
	void calculateGradient(int x, int y, int z, double& gx, double& gy, double& gz, vtkImageData* data);
	void drawBox(GLenum glFaces);
	//void setTFF(const char* filename) {
	//	TFF1dtex = initTFF1DTex(filename);
	//}
	//void setdata(const char* filename, GLuint w, GLuint h, GLuint d) {
	//	initVol3DTex(filename, w, h, d);
	//}
	GLuint initTFF1DTex(const char* filename);
	void initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d);
	VolumeGeometry(const char* filename, const char* ttffilename = NULL,bool flag=false);
	void setscale(float x, float y, float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}
	//VolumeGeometry(const char* filename, const char* ttffilename = NULL);
	VolumeGeometry(const char* filename, GLuint w, GLuint h, GLuint d, const char* ttffilename = NULL) {
		xiSize = w;
		yiSize = h;
		ziSize = d;
		initVol3DTex(filename, w, h, d);

		if (ttffilename) {
			TFF1dtex = initTFF1DTex(ttffilename);
		}
		else {
		
			glGenTextures(1, &TFF1dtex);
			glBindTexture(GL_TEXTURE_1D, TFF1dtex);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);// filter function
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// initialize transfer function
			unsigned char TF1DTable[256][4];
			for (size_t i = 0; i < 256; ++i)
				TF1DTable[i][0] = TF1DTable[i][1] = TF1DTable[i][2] = TF1DTable[i][3] = 0.01 * i;
			for (int i = 93; i < 116; i++) {
				TF1DTable[i][0] = TF1DTable[i][1] = TF1DTable[i][2] = 255;
				TF1DTable[i][3] = 0.2 * 115;
			}
			for (int i = 116; i < 122; i++) {
				TF1DTable[i][0] = TF1DTable[i][2] = 255 - 42.5 * i;
				TF1DTable[i][3] = 255 - 19 * i;
			}

			glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, TF1DTable);
			glBindTexture(GL_TEXTURE_1D, 0);
		}

	//	bbox.autoscale(bmax[0] - bmin[0], bmax[1] - bmin[1], bmax[2] - bmin[2]);
	//	initVAO();
	}
	~VolumeGeometry() {
		if (pDataTexture) delete[] pDataTexture;
	}
};
void VolumeGeometry::drawBox(GLenum glFaces) {
	glEnable(GL_CULL_FACE);
	glCullFace(glFaces);
	//glEnable(GL_CULL_FACE);
	//glCullFace(glFaces);
	GL_ERROR();
	
	glBindVertexArray(g_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	GL_ERROR();
	// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);  // glDrawElements is used for drawing array with additional indexes
	glBindVertexArray(0);
	
	glDisable(GL_CULL_FACE);
}

void VolumeGeometry::calculateGradient(int x, int y, int z, double& gx, double& gy, double& gz, GLubyte* data)
{
	int index = z * xiSize * yiSize + y * xiSize + x;
	int value = data[index];

	if (x > 0)
		if (x < xiSize - 1) gx = (data[index + 1] - data[index - 1]) / 2.0;
		else gx = value - data[index - 1];
	else gx = data[index + 1] - value;

	if (y > 0)
		if (y < yiSize - 1) gy = (data[index + xiSize] - data[index - xiSize]) / 2.0;
		else gy = value - data[index - xiSize];
	else gy = data[index + xiSize] - value;

	if (z > 0)
		if (z < ziSize - 1) gz = (data[index + xiSize * yiSize] - data[index - xiSize * yiSize]) / 2.0;
		else gz = value - data[index - xiSize * yiSize];
	else gz = data[index + xiSize * yiSize] - value;
}

void VolumeGeometry::calculateGradient(int x, int y, int z, double & gx, double & gy, double & gz, vtkImageData * data)
{
	int index = z * xiSize * yiSize + y * xiSize + x;
	int value = static_cast<int*>(data->GetScalarPointer(x, y, z))[0];

	//pixel = static_cast<double*>(data->GetScalarPointer(x+1, y, z));
	//int indexplusone = pixel[0];
	if (x > 0)
		if (x < xiSize - 1) gx = (static_cast<int*>(data->GetScalarPointer(x + 1, y, z))[0] - static_cast<int*>(data->GetScalarPointer(x -1, y, z))[0]) / 2.0;
		else gx = value - static_cast<int*>(data->GetScalarPointer(x - 1, y, z))[0];
	else gx = static_cast<int*>(data->GetScalarPointer(x + 1, y, z))[0] - value;

	if (y > 0)
		if (y < yiSize - 1) gy = (static_cast<int*>(data->GetScalarPointer(x, y+1, z))[0] - static_cast<int*>(data->GetScalarPointer(x, y -1, z))[0]) / 2.0;
		else gy = value - static_cast<int*>(data->GetScalarPointer(x, y - 1, z))[0];
	else gy = static_cast<int*>(data->GetScalarPointer(x, y + 1, z))[0] - value;

	if (z > 0)
		if (z < ziSize - 1) gz = (static_cast<int*>(data->GetScalarPointer(x, y , z+1))[0] - static_cast<int*>(data->GetScalarPointer(x, y, z - 1))[0]) / 2.0;
		else gz = value - static_cast<int*>(data->GetScalarPointer(x, y, z - 1))[0];
	else gz = static_cast<int*>(data->GetScalarPointer(x, y, z + 1))[0] - value;
}

void VolumeGeometry::initVAO()
{
	//scale = std::max(std::max(bmax[0], bmax[1]), bmax[2]);
	bbox.autoscale(scale.x,scale.y,scale.z);

	GL_ERROR();

	glGenVertexArrays(1, &g_vao);
	// vao like a closure binding 3 buffer object: verlocdat vercoldat and veridxdat
	glBindVertexArray(g_vao);
	glEnableVertexAttribArray(0); // for vertexloc
	glEnableVertexAttribArray(1); // for vertexcol
	glEnableVertexAttribArray(2); // for normal
	GL_ERROR();
								  // the vertex location is the same as the vertex color
	glGenBuffers(1, &vertexdat);
	glBindBuffer(GL_ARRAY_BUFFER, vertexdat);
	glBufferData(GL_ARRAY_BUFFER, 216 * sizeof(GLfloat), bbox.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // vertex location
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // vertex color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // vertex normal
	glBindVertexArray(0);
	GL_ERROR();
}

// init the 1 dimentional texture for transfer function
GLuint VolumeGeometry:: initTFF1DTex(const char* filename) {
	// read in the user defined data of transfer function
	XMLDocument doc;
	doc.LoadFile(filename);
	unsigned char TF1DTable[256][4] = { 0 };
	std::stringstream cast;
	XMLElement* root = doc.FirstChildElement("VoreenData")->FirstChildElement("TransFuncIntensity");
	float domainx, domainy, lowx, highx;
	if (!root)return 0;
	XMLElement* keys = root->FirstChildElement("Keys");
	if (root->FirstChildElement("domain")) {
		cast << root->FirstChildElement("domain")->Attribute("x");
		cast >> domainx;
		cast.clear();
		cast << root->FirstChildElement("domain")->Attribute("y");
		cast >> domainy;
		cast.clear();
	}
	else {
		domainx = 0;
		domainy = 1;
	}
	if (root->FirstChildElement("lower")) {
		cast << root->FirstChildElement("lower")->Attribute("value");
		cast >> lowx;
		cast.clear();
	}
	else { lowx = 0; }

	if (root->FirstChildElement("upper")) {
		cast << root->FirstChildElement("upper")->Attribute("value");
		cast >> highx;
		cast.clear();
	}
	else { highx = 1; }
	keys = keys->FirstChildElement();
	float intensity, gap;
	int color, index, findex = 0;
	while (keys) {
		cast << keys->FirstChildElement("intensity")->Attribute("value");
		cast >> intensity;
		cast.clear();
		intensity = (intensity - domainx) * 255 / (domainy - domainx);
		index = floor(intensity);
		if (index != findex) {
			//interpolation
			cast << keys->FirstChildElement("colorL")->Attribute("r");
			cast >> color;
			cast.clear();
			gap = float(color - TF1DTable[findex][0]) / (index - findex);
			color = TF1DTable[findex][0];
			//	cout << gap<<endl;
			for (int i = findex; i <= index; i++)
				TF1DTable[i][0] = int(color + (i - findex)*gap);
			cast << keys->FirstChildElement("colorL")->Attribute("g");
			cast >> color;
			cast.clear();
			gap = float(color - TF1DTable[findex][1]) / (index - findex);
			color = TF1DTable[findex][1];
			for (int i = findex; i <= index; i++)
				TF1DTable[i][1] = int(color + (i - findex)*gap);
			cast << keys->FirstChildElement("colorL")->Attribute("b");
			cast >> color;
			cast.clear();
			gap = float(color - TF1DTable[findex][2]) / (index - findex);
			color = TF1DTable[findex][2];
			for (int i = findex; i <= index; i++)
				TF1DTable[i][2] = int(color + (i - findex)*gap);
			cast << keys->FirstChildElement("colorL")->Attribute("a");
			cast >> color;
			cast.clear();
			gap = float(color - TF1DTable[findex][3]) / (index - findex);
			color = TF1DTable[findex][3];
			for (int i = findex; i <= index; i++)
				TF1DTable[i][3] = int(color + (i - findex)*gap);
		}
		else {
			cast << keys->FirstChildElement("colorL")->Attribute("r");
			cast >> color;
			cast.clear();
			TF1DTable[index][0] = color;
			cast << keys->FirstChildElement("colorL")->Attribute("g");
			cast >> color;
			cast.clear();
			TF1DTable[index][1] = color;
			cast << keys->FirstChildElement("colorL")->Attribute("b");
			cast >> color;
			cast.clear();
			TF1DTable[index][2] = color;
			cast << keys->FirstChildElement("colorL")->Attribute("a");
			cast >> color;
			cast.clear();
			TF1DTable[index][3] = color;
		}
		findex = index;
		keys = keys->NextSiblingElement();
	}
	GLuint tff1DTex;
	glGenTextures(1, &tff1DTex);
	glBindTexture(GL_TEXTURE_1D, tff1DTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);// filter function
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, TF1DTable);
	glBindTexture(GL_TEXTURE_1D, 0);
	
	return tff1DTex;
}
void Geometry::draw(GLuint facetype)
{
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(facetype);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//setup transform things in render class
	GL_ERROR();
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, NumElement);
	GL_ERROR();

	glBindVertexArray(0);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
}
void VolumeGeometry::initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d) {
	FILE *fp;
	size_t size = w * h * d;
	if (size > 1000000) {
		pDataTexture = new unsigned char[size];
		if (!(fp = fopen(filename, "rb"))) {
			cout << "Error: opening .raw file failed" << endl;
			exit(EXIT_FAILURE);
		}
		else {
			cout << "OK: open .raw file successed" << endl;
		}
		if (fread(pDataTexture, sizeof(char), size, fp) != size) {
			cout << "Error: read .raw file failed" << endl;
			exit(1);
		}
		else {
			cout << "OK: read .raw file successed" << endl;
		}
		fclose(fp);
		
		glGenTextures(1, &datatex);
		// bind 3D texture target
		glBindTexture(GL_TEXTURE_3D, datatex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		// pixel transfer happens here from client to OpenGL server
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//控制的是所读取数据的对齐方式，默认4字节对齐
		glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pDataTexture);
		GL_ERROR();
		glBindTexture(GL_TEXTURE_3D, 0);
	}
	else {
		GLubyte* data = new GLubyte[size];			  // 8bit
		if (!(fp = fopen(filename, "rb"))) {
			cout << "Error: opening .raw file failed" << endl;
			exit(EXIT_FAILURE);
		}
		else {
			cout << "OK: open .raw file successed" << endl;
		}
		if (fread(data, sizeof(char), size, fp) != size) {
			cout << "Error: read .raw file failed" << endl;
			exit(1);
		}
		else {
			cout << "OK: read .raw file successed" << endl;
		}
		fclose(fp);

		glGenTextures(1, &datatex);
		// bind 3D texture target
		glBindTexture(GL_TEXTURE_3D, datatex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		// pixel transfer happens here from client to OpenGL server
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//控制的是所读取数据的对齐方式，默认4字节对齐

		pDataTexture = new unsigned char[xiSize * yiSize * ziSize * 4];
		double gx, gy, gz;
		for (int z = 0; z < ziSize; ++z) {
			for (int y = 0; y < yiSize; ++y) {
				for (int x = 0; x < xiSize; ++x) {
					int index = z * xiSize * yiSize + y * xiSize + x;
					calculateGradient(x, y, z, gx, gy, gz, data);
					double norm = sqrt(gx * gx + gy * gy + gz * gz);
					if (fabs(norm) > 10e-10) {
						gx = (gx / norm + 1.0) * 0.5 * 255;
						gy = (gy / norm + 1.0) * 0.5 * 255;
						gz = (gz / norm + 1.0) * 0.5 * 255;
					}
					pDataTexture[index * 4] = data[index];
					pDataTexture[index * 4 + 1] = gx;
					pDataTexture[index * 4 + 2] = gy;
					pDataTexture[index * 4 + 3] = gz;
				}
			}
		}
		//glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, pDataTexture);
		GL_ERROR();
		//glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_RGBA8, xiSize, yiSize, ziSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, pDataTexture);
		glBindTexture(GL_TEXTURE_3D, 0);
		delete[]data;
	}
	cout << "volume texture created" << endl;
	//return g_volTexObj;
}

VolumeGeometry::VolumeGeometry(const char * filename, const char * ttffilename,bool flag)
{
	if (!flag) {
		vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
		/*if (!dicomReader->CanReadFile(filename))
		{
			cout << "can't read this " << filename << endl;
			return;
		}*/
		dicomReader->SetDirectoryName(filename);
		dicomReader->Update();
		dicomReader->SetDataScalarTypeToUnsignedChar();
		vtkImageData *input = 0;
		//GLubyte* data = reinterpret_cast<GLubyte*>(dicomReader->GetMemoryBuffer());// new GLubyte[dicomReader->];
		int a[6];
		dicomReader->GetDataExtent(a);
		//for (int i = 0; i < 6; i++) { cout << a[i] << endl; }
		int h = dicomReader->GetHeight();
		int w = dicomReader->GetWidth();
		int d = a[5];
		xiSize = h;
		yiSize = w;
		ziSize = d;
		//GLubyte* data;// = new GLubyte[h*w*d];

		//data= reinterpret_cast<GLubyte*>(dicomReader->GetDataOrigin());
		input = dicomReader->GetOutput();
		int* dims = input->GetDimensions();
		std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;
		glGenTextures(1, &datatex);
		// bind 3D texture target
		glBindTexture(GL_TEXTURE_3D, datatex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		// pixel transfer happens here from client to OpenGL server
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//控制的是所读取数据的对齐方式，默认4字节对齐

		pDataTexture = new unsigned char[h * w * d * 4];
		double gx, gy, gz;
		for (int z = 0; z < d; ++z) {
			for (int y = 0; y < w; ++y) {
				for (int x = 0; x < h; ++x) {
					int index = z * d * w + y * h + x;
					calculateGradient(x, y, z, gx, gy, gz, input);
					double norm = sqrt(gx * gx + gy * gy + gz * gz);
					if (fabs(norm) > 10e-10) {
						gx = (gx / norm + 1.0) * 0.5 * 255;
						gy = (gy / norm + 1.0) * 0.5 * 255;
						gz = (gz / norm + 1.0) * 0.5 * 255;
					}
					pDataTexture[index * 4] = static_cast<int*>(input->GetScalarPointer(x, y, z))[0];
					pDataTexture[index * 4 + 1] = gx;
					pDataTexture[index * 4 + 2] = gy;
					pDataTexture[index * 4 + 3] = gz;
				}
				/*if (z > 400) {
					cout << "here" << endl; }*/
			}
		}

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, pDataTexture);
		cout << "volume texture created" << endl;
		ofstream f;
		f.open("E:/vtk_project/opengl_projectx86/real_data.raw");
		if (f.is_open())
		{
			f << xiSize << "\t" << yiSize << "\t" << ziSize << endl;
			for (int z = 0; z < d; ++z) {
				for (int y = 0; y < w; ++y) {
					for (int x = 0; x < h; ++x) {
						int index = z * d * w + y * h + x;
						f << pDataTexture[index * 4];
						f << pDataTexture[index * 4 + 1];
						f << pDataTexture[index * 4 + 2];
						f << pDataTexture[index * 4 + 3];
					}
				}
			}
			f.close();
			cout << "volume texture saved" << endl;
		}
		else {
			cout << "can't open file" << endl;
		}
	}
	else {
		fstream f;
		f.open("E:/vtk_project/opengl_projectx86/real_data.raw");
		if (f.is_open()) {
			f >> xiSize;
			f >> yiSize;
			f >> ziSize;
		}
		else {
			cout << "file reading erro";
			exit(0);
		}
		int d = ziSize;
		int w = yiSize;
		int h = xiSize;
		pDataTexture= new unsigned char[h * w * d * 4];
		for (int z = 0; z < d; ++z) {
			for (int y = 0; y < w; ++y) {
				for (int x = 0; x < h; ++x) {
					int index = z * d * w + y * h + x;
					f >>pDataTexture[index * 4];
					f >> pDataTexture[index * 4 + 1];
					f >> pDataTexture[index * 4 + 2];
					f >> pDataTexture[index * 4 + 3];
				}
			}
		}

		glGenTextures(1, &datatex);
		// bind 3D texture target
		glBindTexture(GL_TEXTURE_3D, datatex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		// pixel transfer happens here from client to OpenGL server
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//控制的是所读取数据的对齐方式，默认4字节对齐

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, pDataTexture);
	
	}
	if (ttffilename) {
		TFF1dtex = initTFF1DTex(ttffilename);
	}
	else {
		//using default ttf function
	}
	initVAO();
}
