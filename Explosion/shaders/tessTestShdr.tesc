#version 460

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout(vertices=3)out;

in vec3 vPosition[];
out vec3 tcPosition[];

uniform int TessLevelInner = 1;
uniform int TessLevelOuter = 1;

//uniform mat4 Modelview;

float map(float input_value,float input_start,float input_end,float output_start,float output_end)
{
    float t=(input_value-input_start)/(input_end-input_start);
    float result=mix(output_start,output_end,t);
    return result;
}

void main()
{
    
    // use the gl_InocationID -- index
    tcPosition[gl_InvocationID]=vPosition[gl_InvocationID];
    
    gl_TessLevelInner[0]=float(TessLevelInner);
    
    gl_TessLevelOuter[0]=float(TessLevelOuter);
    gl_TessLevelOuter[1]=float(TessLevelOuter);
    gl_TessLevelOuter[2]=float(TessLevelOuter);
    
}
