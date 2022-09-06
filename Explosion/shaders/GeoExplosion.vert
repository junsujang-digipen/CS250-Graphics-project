#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/


layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;

out VS_OUT {
    vec4 pos;
    vec3 nrm;
} vs_out;

void main(void) 
{
    gl_Position = vec4(pos, 1.0);
    vs_out.pos = gl_Position;
    vs_out.nrm = nrm;
}