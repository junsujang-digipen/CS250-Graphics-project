#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;

//layout (location=0) in vec3 color;
layout (location=0) out vec4 fFragClr;
 in vec2 UV;
uniform vec4 u_color;
uniform sampler2D tex;
//uniform int type = 0;


void main(){
    //fFragClr = u_color + vec4(color,1);
    //mix(texture(tex, UV).rgb,color,1.f);
    
    vec3 col  = vec3(texture(tex, UV).rgb);
    
    
    fFragClr = u_color + vec4(col, 1.0);
}
