#version 460

layout(triangles,equal_spacing,cw)in;

in vec3 tcPosition[];

out VS_OUT{
    vec4 pos;
    vec3 nrm;
}vs_out;

vec3 GetNormal()
{
   vec3 a = vec3(tcPosition[0]) - vec3(tcPosition[1]);
   vec3 b = vec3(tcPosition[2]) - vec3(tcPosition[1]);
   return normalize(cross(a, b));
}  

void main()
{
    // vec3 gl_TessCoord -- normalized coordinates
    //  for tri interpret them as barycentric coords
    float u=gl_TessCoord.x;
    float v=gl_TessCoord.y;
    float w=gl_TessCoord.z;
    
    vec3 p0=tcPosition[0] * u;
    vec3 p1= (tcPosition[1] + GetNormal()) * v;
    vec3 p2=tcPosition[2] * w;
    
    vec3 point_on_triangle=p0+p1+p2;
    vec3 point_on_unit_sphere=normalize(point_on_triangle);

    gl_Position=vec4(point_on_triangle.x,point_on_triangle.y,point_on_triangle.z,1);
    vs_out.pos=gl_Position;
}
