#version 450 core

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;
uniform sampler2D depthMap;
uniform sampler2DShadow depthMap1;
in vec3 vPosition; 
in vec3 vNormal; 
in vec4 vShadowPosition;


out vec4 Fragment;
uniform vec4 u_color = vec4(1.);
uniform mat3 LightPosition = mat3(0.);
uniform vec3 LightColor = vec3(1.0,1.0,1.0);
uniform vec3 DiffuseMaterial = vec3(1.0, 1.0, 1.0);
uniform vec3 AmbientMaterial = vec3(0.0, 0.0, 0.0);
uniform bool DoShadowBehindLight = false;
void main()
{
    vec3 finalVal = vec3(0.);
        int i = 0;
        vec3 ambient = LightColor * AmbientMaterial;
        vec3 N = normalize(vNormal);
        vec3 L = normalize(vec3(LightPosition[0][0],LightPosition[0][1],LightPosition[0][2]) - vPosition);
        float NDotL = max(dot(N,L),0.0);
        vec3 diffuse = NDotL * mix(DiffuseMaterial,u_color.xyz,0.7);
        float shadow = (0.);

         //shadow = (textureProj(depthMap,vShadowPosition));

         vec3 projCoords = vShadowPosition.xyz / vShadowPosition.w;

         //projCoords = projCoords * 0.5 + 0.5;
    
         float closestDepth =  texture(depthMap,projCoords.xy).r;

         float currentDepth = projCoords.z;

         shadow = currentDepth > closestDepth  ? 0.0 : 1.0;

        if(!DoShadowBehindLight && vShadowPosition.z < 0){
            shadow = 1.;
        }
        
      
        finalVal =  diffuse * shadow + ambient;
        
    Fragment = vec4(finalVal, 1.0);
   // Fragment = pow(Fragment,vec4(1./2.2));
}
