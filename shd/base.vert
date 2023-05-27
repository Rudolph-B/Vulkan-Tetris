#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in uint inType;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragTexCoord = inTexCoord;
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
        default:
            fragColor = vec3(0.0, 0.0, 1.0);
            break;
    }
}
