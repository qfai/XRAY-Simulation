// for render mesh
#version 400

layout(location = 0) in vec3 VerPos;
layout(location = 1) in vec3 Normal;//normal
layout(location = 2) in vec2 VerClr;

out vec3 Color;
//out vec4 gl_Position;
out vec3 normal;
out vec3 FragPos;

uniform mat4 Model;
uniform mat4 MVP;

void main()
{
	//normal = Model;
	normal = mat3(transpose(inverse(Model))) * Normal;
	FragPos = /*vec3(Model * vec4(VerPos, 1.0f))*/ VerPos; // why wrong
    Color = /*(ambient + diffuse + specular) */ Normal;
    gl_Position = MVP * vec4(VerPos, 1.0);
}
