#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout (location=0) in vec3 vClrCoord;
layout (location=0) out vec4 fFragClr;

precision mediump float;

uniform vec4 u_color;


void main(){
    vec3 a = vClrCoord;
    fFragClr = u_color;
}