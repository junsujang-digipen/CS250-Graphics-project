#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/


layout (location = 0) in vec3 pos;
layout (location = 0) out vec3 color;
uniform mat4 u_modelToNDC;

void main(void) 
{
    gl_Position = u_modelToNDC * vec4(pos, 1.0);
    color = pos;
}