#version 150
in vec4 color;
out vec4 outColor;
void main()
{
    //outColor = vec4(0.0, 0.0, 1.0, 1.0);
    //outColor2 = vec4(0.0, 1.0, 0.0, 1.0);
    outColor = color;
}