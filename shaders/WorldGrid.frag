#version 330

in vec3 nearPoint;
in vec3 farPoint;

out vec4 outColor;

uniform mat4 fragView;
uniform mat4 fragProj;
uniform float near;
uniform float far;

void main() {
    // Ray von Kamera durch das Fragment
    vec3 rayDir = normalize(farPoint - nearPoint);
    float t = -nearPoint.y / rayDir.y;
    if (t < 0.0) discard;

    vec3 worldPos = nearPoint + t * rayDir;

    // Grid-Linien berechnen
    float gridSize = 1.0;
    float line = min(
    abs(fract(worldPos.x / gridSize - 0.5) - 0.5) / fwidth(worldPos.x),
    abs(fract(worldPos.z / gridSize - 0.5) - 0.5) / fwidth(worldPos.z)
    );

    float grid = 1.0 - smoothstep(0.0, 0.05, line);

    // Fading mit Entfernung
    float fade = exp(-0.02 * abs(worldPos.y));

    vec3 gridColor = vec3(0.33, 0.33, 0.33) * grid * fade;
    outColor = vec4(gridColor, 0.7 * grid * fade);
}