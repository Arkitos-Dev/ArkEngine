#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceModel;
layout (location = 4) in vec3 lightColor;

uniform vec3 uniColor;
uniform mat4 view;
uniform mat4 projection;

out vec3 myColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0);
    myColor = uniColor;
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(instanceModel))) * aNormal;
    FragPos = vec3(instanceModel * vec4(aPos, 1.0));
}