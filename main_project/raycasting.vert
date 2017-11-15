#version 330


layout (location = 0) in vec3 VerPos;
// have to use this variable!!!, or it will be very hard to debug for AMD video card
layout (location = 1) in vec3 VerClr;  
layout (location = 2) in vec3 Normal;

out vec3 normal;
out vec3 EntryPoint;
out vec4 ExitPointCoord;
out mat4 mvpMat;

uniform mat4 Model;
uniform mat4 MVP;

void main()
{
    EntryPoint = VerPos;
    gl_Position = MVP * vec4(VerPos,1.0);
	normal = mat3(transpose(inverse(Model))) * Normal;
    ExitPointCoord = gl_Position;
	mvpMat = MVP;
}
