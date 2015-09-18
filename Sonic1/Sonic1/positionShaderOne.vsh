#version 150
in vec3 position;
in vec3 inColor;
out vec4 color;
uniform mat4 MVP;

void main()
{
    //gl_Position = vec4(position, 0.0, 1.0);
    vec4 v = vec4(position, 1.0);
    color = vec4(inColor, 1.0);
    gl_Position = MVP * v;
    //gl_Position = v;
}