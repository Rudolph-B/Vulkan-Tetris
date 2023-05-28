//
// Created by Rudolph on 2023/05/27.
//

#ifndef VULKAN_TETRIS_STRUCTS_H
#define VULKAN_TETRIS_STRUCTS_H

#include <vulkan/vulkan_core.h>
#include <array>
#include <optional>
#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

struct Vertex {
    glm::vec2 pos;
    glm::int32_t type;
    glm::int32_t age;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32_UINT;
        attributeDescriptions[1].offset = offsetof(Vertex, type);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32_UINT;
        attributeDescriptions[2].offset = offsetof(Vertex, age);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && type == other.type && age == other.age;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec2>()(vertex.pos) ^ (hash<glm::int16>()(vertex.type) << 1)) >> 1) ^ (hash<glm::int16>()(vertex.age) << 1);
        }
    };
}

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
    alignas(16) int model;
    alignas(16) int ticks;
};

/**
 * @brief A struct to hold various keyboard input
 */
struct KeyBoard {
    /* PIECE CONTROL */
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool space = false;

    /* GAME CONTROL */
    bool pageUp = false;
    bool pageDown = false;
    bool esc = false;
    bool enter = false;

    /* SHADER CONTROL */
    bool n1 = false;
    bool n2 = false;
    bool n3 = false;
    bool n4 = false;
};

#endif //VULKAN_TETRIS_STRUCTS_H
