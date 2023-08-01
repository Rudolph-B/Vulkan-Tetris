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
    glm::int32_t type; // Colour
    glm::int32_t age; // Placed/Not placed

    /**
     * Returns the binding description for the vertex and a shader
     * @return
     */
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    /**
     * Returns the attribute descriptions for the vertex
     * @return
     */
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

/**
 * Used to compute the hash of a Vertex object
 */
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
    int model;
};

/**
 * @brief A struct to hold various Keyboard input
 */
enum ActionType {
    press, hold, release
};

/**
 * @brief A struct to hold various Keyboard input
 */
struct Action {
    /* PIECE CONTROL */
    ActionType left = release;
    ActionType right = release;
    ActionType rotate_left = release;
    ActionType rotate_right = release;
    ActionType down = release;
    ActionType drop = release;

    /* GAME CONTROL */
    ActionType pageUp = release;
    ActionType pageDown = release;
    ActionType esc = release;
    ActionType enter = release;

    /* SHADER CONTROL */
    ActionType n1 = release;
    ActionType n2 = release;
};

#endif //VULKAN_TETRIS_STRUCTS_H
