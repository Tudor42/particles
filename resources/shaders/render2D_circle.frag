#version 450 core

out vec4 color;

in vec4 v_Color;

void main()
{
    vec2 coord = vec2(2*gl_PointCoord.x - 1, 2*gl_PointCoord.y - 1);  
    if(dot(coord, coord) > 1.f)
        discard;
    if(length(coord) > 0.99f){
        color = vec4(0.5f, 0.5f, 0.5f, 1.f);
    } else {
        color = v_Color;
    }
}