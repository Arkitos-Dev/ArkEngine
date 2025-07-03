#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 uniColor;
uniform mat4 transform;
uniform mat4 projection;


out vec3 myColor;
out vec2 TexCoord;

void main() {
    gl_Position =  transform * vec4(aPos, 1.0);
    myColor = uniColor;
    TexCoord = aTexCoord;
}