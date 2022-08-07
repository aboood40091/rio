#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

out vec4 FragColor;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}
