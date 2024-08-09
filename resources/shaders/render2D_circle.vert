#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform float pointSize;
uniform mat4 view;
out vec4 v_Color;

void main()
{
    gl_Position = view * vec4(a_Position, 1.0);
    v_Color = a_Color;
    gl_PointSize = pointSize;
}