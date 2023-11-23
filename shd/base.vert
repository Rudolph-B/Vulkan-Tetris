#version 450

layout(binding = 0) uniform UniformBufferObject {
    int model;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 outPosition;

void main() {
    float show;
    switch (ubo.model) {
        case 0:
            show = 1.0;
            break;
        case 1:
            show = 1.0;
            break;
        default:
            show = 1.0;
            break;
    }

    gl_Position = vec4(inPosition, 0.0, 1.0);
    outPosition = inPosition;
    fragColor = inColor * show;
}
