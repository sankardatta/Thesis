#version 150
in vec2 position;
out vec4 color;
//uniform mat4 MVP;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    color = vec4(0.3, 0.1, 0.2, 1.0);
    //gl_Position = MVP * v;
}