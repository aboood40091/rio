#version 330 core

uniform sampler2D texture0;
uniform float rate;

in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec4 color = texture(texture0, TexCoord);
    FragColor.r = Color.r * (color.r * rate + (1 - rate));
    FragColor.g = Color.g * (color.g * rate + (1 - rate));
    FragColor.b = Color.b * (color.b * rate + (1 - rate));
    FragColor.a = Color.a * (color.a * rate + (1 - rate));
}
