#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;
layout (location=0) out vec4 fFragClr;
uniform vec4 u_color;
in vec3 color;
void main(){
    fFragClr = vec4(mix(u_color.rgb,color,0.95),0.1);
}