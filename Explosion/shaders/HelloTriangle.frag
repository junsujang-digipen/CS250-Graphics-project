#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout (location=0) in vec3 vClrCoord;

layout (location=0) out vec4 fFragClr;

precision mediump float;

uniform vec2 u_resolution;
uniform float u_time;

vec2 random2( vec2 p, float seed ) {
    return fract(sin(vec2(dot(p,vec2(seed*1.1,seed*3.2)),dot(p,vec2(seed*2.5,seed*1.2))))*seed*400.14259286);
}

void main(){
    vec2 position = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vClrCoord;

    position *=10;

    vec2 int_position = floor(position);
    vec2 float_position = fract(position);
    
    float m_dist = 2.;
    vec2 m_point = vec2(0.);     

    const float randomSeed = 121.334;
    const float PI = 3.14159;
    for (int j=-1; j<=1; j++ ) {
        for (int i=-1; i<=1; i++ ) {
            vec2 neighbor = vec2(float(i),float(j));
            vec2 point = random2(int_position + neighbor,randomSeed);

            point = 0.5 + 0.5*sin(u_time + 3*PI*point);//鸥老 框流烙 颇飘, 货肺款 规过 备惑?
            
            vec2 diff = neighbor + point - float_position;
            float dist = length(diff);

            if( dist < m_dist ) {
                m_dist = dist;
                m_point = point;
            }
        }
    }

    color += dot(m_point,vec2(0.5,0.5));

    fFragClr = vec4(color ,1.0);
}