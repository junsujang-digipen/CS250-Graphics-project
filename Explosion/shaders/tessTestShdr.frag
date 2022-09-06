#version 460

/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

out vec4 FragColor;

in vec3 gNormal;

uniform vec3 u_light = vec3(0., 100., 100.);
uniform vec3 u_ambient = vec3(0.3, 0.3, 0.3);
uniform vec3 u_materialCol = vec3(1.0, 1.0, 1.0);
uniform float shininess = 10.0; //will be uniform

uniform float u_time;
uniform vec4 u_color;
void main()
{
    vec3 n=normalize(gNormal);
    vec3 l=normalize(u_light);
    float nl=abs(dot(n,l));

    vec3 H = normalize(l + vec3(0,0,1));
    float sf = max(0.0, dot(n, H));
    sf = pow(sf, shininess);

    vec3 color = u_ambient + sf * u_materialCol + nl * u_color.rgb;
    FragColor=vec4(color,sin(u_time*0.01)+1);
}
