// for raycasting
#version 400

layout(location = 0) in vec3 VerPos;
layout(location = 1) in vec3 VerClr;
layout (location = 2) in vec3 Normal;

out vec3 Color;
//out vec4 gl_Position;

uniform mat4 Model;
uniform mat4 MVP;
uniform vec3 scale;
void main()
{
	
    Color.x =  (VerPos.x/scale.x+1)/2;
    Color.y =  (VerPos.y/scale.y+1)/2;
    Color.z =  (VerPos.z/scale.z+1)/2;
    gl_Position = MVP * vec4(VerPos, 1.0);
}
