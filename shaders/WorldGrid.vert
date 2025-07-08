#version 330

layout(location = 0) in vec3 aPos;

uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos;
} view;

out vec3 nearPoint;
out vec3 farPoint;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

// shaders/WorldGrid.vert
void main() {
    nearPoint = UnprojectPoint(aPos.x * 2.0, aPos.z * 2.0, 0.0, view.view, view.proj);
    farPoint  = UnprojectPoint(aPos.x * 2.0, aPos.z * 2.0, 1.0, view.view, view.proj);
    gl_Position = view.proj * view.view * vec4(aPos, 1.0);
}