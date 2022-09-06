#version 460

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

layout(triangles)in;
layout(triangle_strip,max_vertices=30)out;

uniform mat4 u_modelToNDC;
uniform float Shrink=.9;
uniform mat3 u_normalMatrix;

uniform vec3 u_windVec = vec3(0., 100., 100.);
out vec3 gNormal;

vec3 Verts[3];
vec3 Center;
vec3 Normal;
vec3 basicNormal;

in VS_OUT {
    vec4 pos;
    vec3 nrm;
} gs_in[];
uniform float u_time;
vec2 random2( vec2 p, float seed ) {
    return fract(sin(vec2(dot(p,vec2(seed*1.1,seed*3.2)),dot(p,vec2(seed*2.5,seed*1.2))))*seed*400.14259286);
}
void EmitShrunkVertex(int i)
{
    vec3 move_direction=Verts[i]-Center;
    gl_Position=u_modelToNDC *vec4(Center+Shrink*move_direction,1);
    
    gNormal=Normal;
    EmitVertex();
}
void EmitGrass(int i1,int i2){
    vec3 stPos = Verts[i1];
    vec3 direction = Verts[i2]-Verts[i1];
    //vec3 normDir = normalize(direction);
    vec2 ranNum = random2(stPos.xy,1.234);

    float minRan = min(ranNum.x,ranNum.y);
    float maxRan = max(ranNum.x,ranNum.y);

    minRan = max(minRan,0.35);
    maxRan = min(maxRan,0.65);

    vec3 grassPos1 = stPos + direction*minRan;
    vec3 grassPos2 = stPos + direction*maxRan;
    float height = Shrink*(minRan*maxRan);
    vec3 grassPos3 = (grassPos1 + grassPos2)/2. + basicNormal*height;
    vec3 grassPos4 = (grassPos3  - grassPos1)/2.;
    vec3 grassPos5 = (grassPos3 - grassPos2)/2.;
    gNormal=cross(grassPos2 - grassPos1,grassPos3 - grassPos1);
    vec3 wind =  vec3(0.);
    if(u_windVec != vec3(0.)){
        wind =  normalize(u_windVec)*0.05 *(sin(u_time) + 1.5);
    }
    vec3 leftRightMove = vec3(normalize(gNormal) * (minRan + maxRan -0.5)/10.) + wind;
    grassPos3 = basicNormal*height + leftRightMove;
    grassPos4 += leftRightMove/6.;
    grassPos5 += leftRightMove/6.;
    //++wind
    grassPos3 = (grassPos1 + grassPos2)/2. + normalize(grassPos3)*height;
    grassPos4 = grassPos1 +  normalize(grassPos4)*height/2.;
    grassPos5 = grassPos2 + normalize(grassPos5)*height/2.;


    gNormal=cross(grassPos5 - grassPos1,grassPos4 - grassPos1);
    gl_Position=u_modelToNDC *vec4(grassPos1,1);
    EmitVertex();
    gl_Position=u_modelToNDC *vec4(grassPos5,1);
    EmitVertex();
    gl_Position=u_modelToNDC *vec4(grassPos4,1);
    EmitVertex();
    EndPrimitive();

    gl_Position=u_modelToNDC *vec4(grassPos2,1);
    EmitVertex();
    gl_Position=u_modelToNDC *vec4(grassPos5,1);
    EmitVertex();
    gl_Position=u_modelToNDC *vec4(grassPos1,1);
    EmitVertex();
    EndPrimitive();


    gNormal=cross(grassPos5 - grassPos4,grassPos3 - grassPos4);
    gl_Position=u_modelToNDC *vec4(grassPos4,1);
    EmitVertex();
    gl_Position=u_modelToNDC *vec4(grassPos5,1);
    EmitVertex();
    gl_Position=u_modelToNDC *vec4(grassPos3,1);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    Verts[0]=gs_in[0].pos.xyz;
    Verts[1]=gs_in[1].pos.xyz;
    Verts[2]=gs_in[2].pos.xyz;
    
    Center=(Verts[0]+Verts[1]+Verts[2])/3.;
    
    vec3 A=Verts[2]-Verts[0];
    vec3 B=Verts[1]-Verts[0];
    basicNormal = vec3(normalize(cross(A,B)));
    Normal=u_normalMatrix*normalize(cross(A,B));
    
    EmitGrass(0,1);
    EmitGrass(1,2);
    EmitGrass(2,0);

    EmitShrunkVertex(0);
    EmitShrunkVertex(1);
    EmitShrunkVertex(2);
    
    EndPrimitive();
}
