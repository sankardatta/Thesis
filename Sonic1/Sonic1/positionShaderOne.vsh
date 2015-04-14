#version 150
in vec2 position;
//uniform mat4 MVP;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    //gl_Position = MVP * v;
}