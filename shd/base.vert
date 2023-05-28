#version 450

layout(binding = 0) uniform UniformBufferObject {
    int model;
    int ticks;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in uint inType;
layout(location = 2) in uint inAge;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 outPosition;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    switch (inType) {
        case 0:
            fragColor = vec3(1.0, 1.0, 1.0);
            break;
        case 1:
            fragColor = vec3(1.0, 0.0, 0.0);
            break;
        case 2:
            fragColor = vec3(0.0, 1.0, 0.0);
            break;
        case 3:
            fragColor = vec3(0.0, 0.0, 1.0);
            break;
        case 4:
            fragColor = vec3(0.0, 1.0, 1.0);
            break;
        case 5:
            fragColor = vec3(1.0, 0.0, 1.0);
            break;
        case 6:
            fragColor = vec3(1.0, 1.0, 0.0);
            break;
        default:
            fragColor = vec3(1.0, 1.0, 1.0);
            break;
    }
}
