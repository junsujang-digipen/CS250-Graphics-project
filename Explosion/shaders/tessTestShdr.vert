#version 460

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout (location = 0) in vec3 Position;

out vec3 vPosition;
uniform mat4 u_modelToNDC;
void main()
{
    vPosition = vec3(vec4(Position,1.));
}
