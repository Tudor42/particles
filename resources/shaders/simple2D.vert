#version 450 core

layout(location = 0) in vec2 a_Position;

uniform mat4 view;
out vec4 v_Color;

void main()
{
    gl_Position = view * vec4(a_Position, 0, 1.0);
    v_Color = vec4(1.0, 1.0, 1.0, 1.0);
}