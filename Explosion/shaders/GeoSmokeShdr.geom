#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

uniform int u_radius=10;
uniform int u_circum=360;
//const int numMaxVert = u_radius*u_circum;
layout (triangles) in;
layout (points, max_vertices =90) out;
in VS_OUT {
    vec4 pos;
    vec3 nrm;
} gs_in[];

uniform float u_time;
out vec3 color;

vec2 random2( vec2 p, float seed ) {
	seed+=0.001f;
    return fract(sin(vec2(dot(p,vec2(seed*1.1,seed*3.2)),dot(p,vec2(seed*2.5,seed*1.2))))*seed*400.14259286 +vec2(seed*0.21355,seed*0.9832));
}
float random(vec2 p, float seed){

    vec2 temp = random2(p,seed);
    return fract(sin(temp.x*temp.y*(seed+0.001f)) + seed);
}
float smoothStep(float t)
{
	float t3 = t*t*t;
	return 6 * t3 * t * t - 15.f * t3 * t + 10 * t3;
}
float lerp(float lo, float hi, float t) { return lo * (1 - t) + hi * t; }
float noise2D(const float xoff, const float yoff,float seed)
{
	int xi = int(floor(xoff));
	int yi = int(floor(yoff));

	int xi0 = xi;
	int xi1 = (xi0 + 1);
	int yi0 = yi ;
	int yi1 = (yi0 + 1);

	float ran00 = random(vec2(xi0,yi0),seed);
	float ran10 = random(vec2(xi1,yi0),seed);
	float ran01 = random(vec2(xi0,yi1),seed);
	float ran11 = random(vec2(xi1,yi1),seed);

	float xf = xoff - xi;
	float yf = yoff - yi;
	float sx = smoothStep(xf);
	float sy = smoothStep(yf);

	float nx0 = lerp(ran00, ran10, sx);
	float nx1 = lerp(ran01, ran11, sx);
	return lerp(nx0, nx1, sy);
}

vec3 GetNormal()
{
   vec3 a = vec3(gs_in[0].pos) - vec3(gs_in[1].pos);
   vec3 b = vec3(gs_in[2].pos) - vec3(gs_in[1].pos);
   return normalize(cross(a, b));
}  
uniform mat4 u_modelToNDC;
void main() {    
	color = vec3(noise2D(gs_in[0].pos.x,gs_in[0].pos.y,15),
	noise2D(gs_in[1].pos.x,gs_in[1].pos.y,10),
	noise2D(gs_in[2].pos.x,gs_in[2].pos.y,20));
	for(int j = 0 ;j < 3;++j){
		for(int i = 0 ;i < 30;++i){
			int itemp = i+1;
			gl_Position = u_modelToNDC *(gs_in[j].pos
				 + vec4(noise2D(gs_in[j].pos.x*itemp +u_time,gs_in[j].pos.y*itemp+u_time,itemp)-0.5,
				noise2D(gs_in[j].pos.y*itemp+u_time,gs_in[j].pos.z*itemp+u_time,itemp)-0.5,
				noise2D(gs_in[j].pos.z*itemp+u_time,gs_in[j].pos.x*itemp+u_time,itemp)-0.5,
				0.0)*(noise2D(u_time*0.3,u_time*0.25,1.)+0.05)*0.5);
			EmitVertex();
			
		}
		EndPrimitive();
	}
}   