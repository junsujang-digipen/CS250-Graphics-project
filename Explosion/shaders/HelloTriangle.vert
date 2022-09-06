#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec3 vVertexClrCoord;

layout (location=0) out vec3 vClrCoord;

void main() {
  gl_Position = vec4(vVertexPosition, 0.0, 1.0);
  vClrCoord = vVertexClrCoord;
}
