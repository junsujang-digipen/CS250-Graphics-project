#version 330 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;

in vec4 a_position;
in vec3 a_normal;

out vec3 v_normal;
out vec3 v_position;


uniform mat4 u_modelToWorld;
uniform mat4 u_worldToCamera;
uniform mat3 u_normalMatrix;
uniform mat4 u_modelToNDC;

void main()
{
    vec4 a = a_position;

    gl_Position = u_modelToNDC * a;

    v_normal    = u_normalMatrix * a_normal;
    v_position = (u_worldToCamera * u_modelToWorld * a).xyz;
}
