/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#ifndef VULKAN_TETRIS_ENGINE_H
#define VULKAN_TETRIS_ENGINE_H

//<editor-fold desc="/* LIBRARY INCLUDE */" defaultstate="collapsed">
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/hash.hpp"

//</editor-fold>

#include "structs.h"
#include "constants.h"
#include "window.h"

//<editor-fold desc="/* STANDARD INCLUDES */" defaultstate="collapsed">
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>
//</editor-fold>

class Engine {
public:
    //<editor-fold desc="/* PUBLIC CONSTANTS */" defaultstate="collapsed">

    /* SPECIFY WHICH VALIDATION LAYERS TO ENABLE */
    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    /* SPECIFY WHICH DEVICE EXTENSIONS TO ENABLE */
    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    //</editor-fold>

    //<editor-fold desc="/* PUBLIC FUNCTIONS */" defaultstate="collapsed">
    explicit Engine(Window *window);
    ~Engine();

    bool framebufferResized = false;
    void waitDeviceIdle();
    void drawFrame();
    void copyVertexBuffer();
    void copyIndexBuffer();
    void updateVertices(const std::vector<Vertex> &nVertices);
    void updateSettings(Action keyboard);
    //</editor-fold>
private:
    //<editor-fold desc="/* PRIVATE PARAMETERS */" defaultstate="collapsed">
    Window* window;

    /* VK PARAMETERS */
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;

    std::vector<Vertex> objVertices;
    std::vector<uint32_t> objIndices;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer vertexStagingBuffer;
    VkDeviceMemory vertexStagingBufferMemory;
    VkDeviceSize vertexBufferSize = 800 * sizeof(uint32_t); // Actual max is 231

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkBuffer indexStagingBuffer;
    VkDeviceMemory indexStagingBufferMemory;
    VkDeviceSize indexBufferSize = 1200 * sizeof(uint32_t); // Actual max is 1200

    UniformBufferObject uniformBufferObject = {0};
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    //</editor-fold>

    //<editor-fold desc="/* PRIVATE METHODS */" defaultstate="collapsed">
    void createSwapChain();
    void cleanupSwapChain();
    void recreateSwapChain();

    void createInstance();
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void updateUniformBuffer(uint32_t currentImage);

    VkShaderModule createShaderModule(const std::vector<char>& code);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pDevice);
    bool isDeviceSuitable(VkPhysicalDevice p_device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice p_device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice p_device);
    static std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    static std::vector<char> readFile(const std::string& filename);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    void cleanVertexBuffer();
    void cleanIndexBuffer();
    //</editor-fold>

};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

#endif //VULKAN_TETRIS_ENGINE_H
