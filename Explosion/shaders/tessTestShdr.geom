#version 460

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout(triangles)in;
layout(triangle_strip,max_vertices=3)out;

uniform mat4 u_modelToNDC;
uniform float Shrink=.9;
uniform mat3 u_normalMatrix;

out vec3 gNormal;

vec3 Verts[3];
vec3 Center;
vec3 Normal;

in VS_OUT {
    vec4 pos;
    vec3 nrm;
} gs_in[];


void EmitShrunkVertex(int i)
{
    vec3 move_direction=Verts[i]-Center;
    gl_Position=u_modelToNDC *vec4(Center+Shrink*move_direction,1);
    
    gNormal=Normal;
    EmitVertex();
}

void main()
{
    Verts[0]=gs_in[0].pos.xyz;
    Verts[1]=gs_in[1].pos.xyz;
    Verts[2]=gs_in[2].pos.xyz;
    
    Center=(Verts[0]+Verts[1]+Verts[2])/3.;
    
    vec3 A=Verts[2]-Verts[0];
    vec3 B=Verts[1]-Verts[0];
    Normal=u_normalMatrix*normalize(cross(A,B));
    
    EmitShrunkVertex(0);
    EmitShrunkVertex(1);
    EmitShrunkVertex(2);
    
    EndPrimitive();
}
