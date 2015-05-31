#version 150
in vec4 color;
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D ourTexture;

void main()
{
    //outColor = vec4(0.0, 0.0, 1.0, 1.0);
    //outColor2 = vec4(0.0, 1.0, 0.0, 1.0);
    //outColor = texture(ourTexture, TexCoord);
    outColor = color;
}


