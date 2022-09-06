#version 450 core
/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;

out VS_OUT {
    vec4 pos;
    vec3 nrm;
} vs_out;


void main(){
    //vec3 vPos = (ModelMatrix * vec4(pos, 1.0)).xyz; 
    vs_out.nrm  =  nrm;
    gl_Position = vec4(pos, 1.0);
    vs_out.pos = gl_Position;
}