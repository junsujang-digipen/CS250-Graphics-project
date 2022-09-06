#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout (location = 0) in vec3 pos;
//layout (location = 0) out vec3 color;
layout (location = 2) in vec2 uv;
out vec2 UV;
uniform mat4 u_modelToNDC;
uniform sampler2D tex;
uniform int PlaneView = 0;

void main(void) 
{
    vec3 position = pos;
    UV = uv;
    if(PlaneView == 1){
       position.z += float(texture(tex, UV).a)*2.-1.;
    }
    gl_Position = u_modelToNDC * vec4(position, 1.0);
    
    //color = vec3(1.);

    
    
}