#version 450 core
/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

const int numMaxVert = 4;
layout (triangles) in;
layout (line_strip, max_vertices =numMaxVert) out;
in VS_OUT {
    vec4 pos;
    vec3 nrm;
} gs_in[];

uniform float u_time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 500.0;
    vec3 direction = normal * cos(u_time ) * magnitude; 
    return position + vec4(direction, 0.0);
} 

vec4 explode1(vec4 position, vec3 normal)
{
    float magnitude = 500.0;
    vec3 direction = normal * sin(u_time) * magnitude; 
    return position + vec4(direction, 0.0);
} 


vec3 GetNormal()
{
   vec3 a = vec3(gs_in[0].pos) - vec3(gs_in[1].pos);
   vec3 b = vec3(gs_in[2].pos) - vec3(gs_in[1].pos);
   return normalize(cross(a, b));
}  

void main() {    
    vec3 normal = GetNormal();
      gl_Position = gs_in[0].pos;
    gl_Position.x = explode(gs_in[0].pos, normal).x;
    gl_Position.y =  explode1(gs_in[0].pos, normal).y;
  
    EmitVertex();
      gl_Position = gs_in[1].pos;
       gl_Position.x = explode(gs_in[1].pos, normal).x;
    gl_Position.y =  explode1(gs_in[1].pos, normal).y;
    EmitVertex();
      gl_Position = gs_in[2].pos;
        gl_Position.x = explode(gs_in[2].pos, normal).x;
    gl_Position.y =  explode1(gs_in[2].pos, normal).y;
    EmitVertex();
    EndPrimitive();
}   