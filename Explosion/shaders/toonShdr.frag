#version 330 core
/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
precision mediump float;


uniform vec3 u_light = vec3(0., 100., 100.);
uniform vec3 u_ambient = vec3(0.0, 0.0, 0.0);
uniform vec3 u_materialCol = vec3(1.0, 1.0, 1.0);
uniform vec4 u_color;
in vec3  v_normal;
in vec3 v_position;

out vec4 Fragment;

uniform float fogDensity = 0.001f;//will be uniform
uniform float shininess = 10.0; //will be uniform
const float A = 0.3;//will be uniform
const float B = 0.5;//will be uniform
const float C = 0.8;//will be uniform
const float D = 1.0;//will be uniform
uniform vec3 fogColor = vec3(0.0, 0.0, 0.0);//will be uniform

#define LOG2 1.442695

float stepmix(float edge0, float edge1, float E, float x)
{
    float T = clamp(0.5 * (x - edge0 + E) / E, 0.0, 0.1);
    return mix(edge0, edge1, T);
}

void main()
{
    vec3  n  = normalize(v_normal);
    vec3  l  = normalize(u_light);
    float nl = max(dot(n, l), 0.);
    vec3 H = normalize(l + vec3(0,0,1));
    float sf = max(0.0, dot(n, H));
    sf = pow(sf, shininess);

    float E = fwidth(nl);
	if (nl > A - E && nl < A + E){nl = stepmix(A, B, E, nl);}
	else if (nl > B - E && nl < B + E){nl = stepmix(B, C, E, nl);}
	else if (nl > C - E && nl < C + E){nl = stepmix(C, D, E, nl);}
    else if(nl<A){nl=A;}
    else if(nl<B){nl=B;}
    else if(nl<C){nl=C;}
    else {nl=D;}

    E = fwidth(nl);
    const float ss = 0.95;//shininess standard
	if (sf > ss - E && sf < ss + E)
	{
	    sf = smoothstep(ss - E, ss + E, sf);

	}
	else
	{
        sf = step(ss, sf);

    }


    vec3 color = u_ambient + sf * u_materialCol + nl * u_color.rgb;
    
     float fogDistance = length(v_position);
    float fogAmount = 1. - exp2(-fogDensity/10. * fogDensity/10. * fogDistance * fogDistance * LOG2);
    
    fogAmount = clamp(fogAmount, 0., 1.);

    color = mix((color), fogColor, fogAmount).rgb;
    
    Fragment   = vec4(color, 1);
}
