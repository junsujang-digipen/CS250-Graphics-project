#version 330 core
/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;


out vec3 vPosition;
out vec3 vNormal;
out vec4 vShadowPosition;

uniform mat4 Projection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ShadowMatrix;

void main(){
  vPosition = (ModelMatrix * vec4(pos, 1.0)).xyz; 
   vNormal = transpose(inverse(mat3(ModelMatrix))) * nrm;
   vShadowPosition = ShadowMatrix * vec4(vPosition, 1.0); 
    gl_Position = Projection * ViewMatrix * vec4(vPosition, 1.0);

}