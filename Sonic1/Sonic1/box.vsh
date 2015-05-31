#version 150

in vec2 texCoord;
in vec3 position;
in vec3 inColor;

out vec4 color;
out vec2 TexCoord;

uniform mat4 MVP;

void main()
{
    TexCoord = texCoord;
    //gl_Position = vec4(position, 1.0);
    vec4 boxPos = vec4(position, 1.0);
    gl_Position = MVP * boxPos;
    color = vec4( inColor, 1.0);
    //color = vec4(abs(inColor.x), abs(inColor.y), abs(inColor.z), 1.0);
}
