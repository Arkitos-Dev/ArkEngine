#version 330 core
out vec4 FragColor;

in vec3 myColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 baseColor;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    FragColor = texColor * vec4(baseColor, 1.0);
}