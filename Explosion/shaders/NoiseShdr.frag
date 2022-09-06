#version 450 core
/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

precision mediump float;

layout (location=0) in vec3 color;
layout (location=0) out vec4 fFragClr;
uniform vec4 u_color;

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


uniform float seed = 0.2f;//will be uniform
uniform float size = 512;//will be uniform
uniform float freq = 0.01f;//will be uniform
uniform int type = 0;//will be uniform
uniform int numLayer = 5;//will be uniform

void main(){
	//vec2 position = gl_FragCoord.xy;
	vec2 position = color.xy * size;
	position *= freq;
	float noiseVal = noise2D(position.x,position.y,seed);
	
	if(type == 1){
		noiseVal *=10;
		noiseVal -= int(noiseVal);
	}else if(type ==2){
		for(int i = 1;i<numLayer;++i){
			float layerSeed = (seed+0.01f) * i;
			//position.x +=float(i)*size*seed;
			float tempN = noise2D(position.x,position.y,seed);
			noiseVal += abs(tempN*2-1);
		}
		noiseVal /= float(numLayer);
	}
	else if(type == 3)
	{
	float frequencyMult = 1.1; 
	float amplitudeMult = 0.35; 
	float amplitude = 1; 
	float noiseValue = 0;
	vec2 p = position * freq;
	for (int i = 0; i < numLayer; ++i) { 
			noiseValue = noiseValue + noise2D(p.x,p.y,seed) * amplitude;
			p = p * frequencyMult;
			amplitude = amplitude * amplitudeMult;
		}
		 noiseVal = (sin((p.x+ noiseValue * 100.f) * 2.f * 3.14f / 200.f) + 1.f) / 2.f;       
	}
	vec3 col =vec3(noiseVal);
    fFragClr = u_color + vec4(col,1);
}
