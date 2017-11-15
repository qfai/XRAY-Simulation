#pragma once
#include <glew.h>  
#include <glut.h>  
#include<iostream>
#include<vector>
#include "tiny_obj_loader.h"

class Geometry {
public:

	float bmin[3] = { 0 };
	float bmax[3] = {0};
	GLuint NumElement=0;
	GLuint m_vao;
	GLuint gbo;
	GLfloat* objdata=NULL;
	glm::mat4 m_local;
	bool readObjData(const char* path,int ratio=1);//ratio means how small this mesh scaled , if 0 ,not scale
	void draw(GLuint facetype=GL_BACK);
	void initVBO();
	~Geometry() {
	}

};
static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f) {
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
	}
}
bool Geometry::readObjData(const char* path, int ratio) {
	if (objdata)
		delete objdata;
	bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
	bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string err;
	std::vector<tinyobj::material_t> materials;
	bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	if (!ret) {
		std::cerr << "Failed to load " << path << std::endl;
		return false;
	}

	printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("# of materials = %d\n", (int)materials.size());
	printf("# of shapes    = %d\n", (int)shapes.size());
	
	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	NumElement = 0;
	for (size_t s = 0; s < shapes.size(); s++) {
		NumElement += shapes[s].mesh.indices.size();
	}
	objdata = (GLfloat*)malloc(sizeof(GLfloat) * 8 * NumElement);//vertice*3+coord*2+norm*3
	printf("# of Triangles    = %d\n", (int)NumElement);
	//int index = 0;
	for (size_t s = 0; s < shapes.size(); s++) {
		for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
			tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

			float tc[3][2];
			if (attrib.texcoords.size() > 0) {
				assert(attrib.texcoords.size() > 2 * idx0.texcoord_index + 1);
				assert(attrib.texcoords.size() > 2 * idx1.texcoord_index + 1);
				assert(attrib.texcoords.size() > 2 * idx2.texcoord_index + 1);
				tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
				tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
				tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
				tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
				tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
				tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
			}
			else {
				tc[0][0] = 0.0f;
				tc[0][1] = 0.0f;
				tc[1][0] = 0.0f;
				tc[1][1] = 0.0f;
				tc[2][0] = 0.0f;
				tc[2][1] = 0.0f;
			}

			float v[3][3];
			for (int k = 0; k < 3; k++) {
				int f0 = idx0.vertex_index;
				int f1 = idx1.vertex_index;
				int f2 = idx2.vertex_index;
				assert(f0 >= 0);
				assert(f1 >= 0);
				assert(f2 >= 0);

				v[0][k] = attrib.vertices[3 * f0 + k];
				v[1][k] = attrib.vertices[3 * f1 + k];
				v[2][k] = attrib.vertices[3 * f2 + k];
				if (ratio == 0) {
					for (int kk = 0; kk < 3; kk++) {
						bmin[kk] = 0;
						bmax[kk] = 1;
					}
				}
				else {

					bmin[k] = std::min(v[0][k] / ratio, bmin[k]);
					bmin[k] = std::min(v[1][k] / ratio, bmin[k]);
					bmin[k] = std::min(v[2][k] / ratio, bmin[k]);
					bmax[k] = std::max(v[0][k] * ratio, bmax[k]);
					bmax[k] = std::max(v[1][k] * ratio, bmax[k]);
					bmax[k] = std::max(v[2][k] * ratio, bmax[k]);
				}
			}

			float n[3][3];
			if (attrib.normals.size() > 0) {
				int f0 = idx0.normal_index;
				int f1 = idx1.normal_index;
				int f2 = idx2.normal_index;
				assert(f0 >= 0);
				assert(f1 >= 0);
				assert(f2 >= 0);
				for (int k = 0; k < 3; k++) {
					n[0][k] = attrib.normals[3 * f0 + k];
					n[1][k] = attrib.normals[3 * f1 + k];
					n[2][k] = attrib.normals[3 * f2 + k];
				}
			}
			else {
				// compute geometric normal
				CalcNormal(n[0], v[0], v[1], v[2]);
				n[1][0] = n[0][0];
				n[1][1] = n[0][1];
				n[1][2] = n[0][2];
				n[2][0] = n[0][0];
				n[2][1] = n[0][1];
				n[2][2] = n[0][2];
			}
			int index = 3 * 8 * f;
			for (int kk = 0; kk < 3; kk++) {
				objdata[index + kk * 8] = v[kk][0];
				objdata[index + kk * 8 + 1] = v[kk][1];
				objdata[index + kk * 8 + 2] = v[kk][2];
				/*objdata[index + kk * 8] = (v[kk][0]-bmin[0])/(bmax[0]-bmin[0])*2-1;
				objdata[index + kk * 8+1] = (v[kk][1] - bmin[1]) / (bmax[1] - bmin[1])*2-1;
				objdata[index + kk * 8 + 2] = (v[kk][2] - bmin[2]) / (bmax[2] - bmin[2])*2-1;*/
				objdata[index + kk * 8 + 3] = n[kk][0];
				objdata[index + kk * 8 + 4] = n[kk][1];
				objdata[index + kk * 8 + 5] = n[kk][2];
				objdata[index + kk * 8 + 6] = tc[kk][0];
				objdata[index + kk * 8 + 7] = tc[kk][1];
			}

			//index += 8 * 3;
		}
	}
	return true;
}
void Geometry::initVBO() {

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &gbo);
	glBindBuffer(GL_ARRAY_BUFFER, gbo);
	glBufferData(GL_ARRAY_BUFFER, NumElement*8 * sizeof(GLfloat), objdata, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // for verpos
	glEnableVertexAttribArray(1); // for vercoord
	glEnableVertexAttribArray(2); // for vernorm
//	glBindBuffer(GL_ARRAY_BUFFER, gbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // vertex normal
	glBindVertexArray(0);
	//m_vao = meshVao;
}
