#version 330 core

uniform vec4 wvp[4];
uniform vec4 user[3];
uniform vec4 color0;
uniform vec4 color1;

in vec3 Vertex;
in vec2 TexCoord0;
in vec4 ColorRate;

out vec4 Color;
out vec2 TexCoord;

void main()
{
    vec4 uwvp[4] = vec4[4](
        vec4(
            wvp[0][0] * user[0][0] + wvp[0][1] * user[1][0] + wvp[0][2] * user[2][0],
            wvp[0][0] * user[0][1] + wvp[0][1] * user[1][1] + wvp[0][2] * user[2][1],
            wvp[0][0] * user[0][2] + wvp[0][1] * user[1][2] + wvp[0][2] * user[2][2],
            wvp[0][0] * user[0][3] + wvp[0][1] * user[1][3] + wvp[0][2] * user[2][3] + wvp[0][3]
        ),
        vec4(
            wvp[1][0] * user[0][0] + wvp[1][1] * user[1][0] + wvp[1][2] * user[2][0],
            wvp[1][0] * user[0][1] + wvp[1][1] * user[1][1] + wvp[1][2] * user[2][1],
            wvp[1][0] * user[0][2] + wvp[1][1] * user[1][2] + wvp[1][2] * user[2][2],
            wvp[1][0] * user[0][3] + wvp[1][1] * user[1][3] + wvp[1][2] * user[2][3] + wvp[1][3]
        ),
        vec4(
            wvp[2][0] * user[0][0] + wvp[2][1] * user[1][0] + wvp[2][2] * user[2][0],
            wvp[2][0] * user[0][1] + wvp[2][1] * user[1][1] + wvp[2][2] * user[2][1],
            wvp[2][0] * user[0][2] + wvp[2][1] * user[1][2] + wvp[2][2] * user[2][2],
            wvp[2][0] * user[0][3] + wvp[2][1] * user[1][3] + wvp[2][2] * user[2][3] + wvp[2][3]
        ),
        vec4(
            wvp[3][0] * user[0][0] + wvp[3][1] * user[1][0] + wvp[3][2] * user[2][0],
            wvp[3][0] * user[0][1] + wvp[3][1] * user[1][1] + wvp[3][2] * user[2][1],
            wvp[3][0] * user[0][2] + wvp[3][1] * user[1][2] + wvp[3][2] * user[2][2],
            wvp[3][0] * user[0][3] + wvp[3][1] * user[1][3] + wvp[3][2] * user[2][3] + wvp[3][3]
        )
    );
    vec4 pos = vec4(Vertex, 1);

    gl_Position = vec4(dot(uwvp[0], pos),
                       dot(uwvp[1], pos),
                       dot(uwvp[2], pos),
                       dot(uwvp[3], pos));

    Color = color0 * (1.0 - ColorRate.r) + color1 * ColorRate.r;
    TexCoord = TexCoord0;
}
