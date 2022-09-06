#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;

layout (location=0) in vec3 color;
layout (location=0) out vec4 fFragClr;
uniform vec4 u_color;



void main(){
    fFragClr = u_color + vec4(color,1);
}