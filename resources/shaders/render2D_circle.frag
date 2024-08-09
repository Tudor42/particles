#version 450 core

out vec4 color;

in vec4 v_Color;

void main()
{
    vec2 coord = vec2(2*gl_PointCoord.x - 1, 2*gl_PointCoord.y - 1);  
    if(dot(coord, coord) > 1.f)                  //outside of circle radius?
        discard;
    color = v_Color;
}