/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include "engine.h"

/**
 * @brief Creates a debug messenger to handle validation layer messages.
 *
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @param pDebugMessenger
 * @return
 */
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

/**
 * @brief Destroys the debug messenger.
 *
 * @param instance
 * @param debugMessenger
 * @param pAllocator
 */
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

Engine::Engine(Window *window) {
    this->window = window;

    /* SELECT GRAPHICAL DEVICE */
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();

    /* BUILD GRAPHICS PIPELINE */
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();

    /* BUILD PIPELINE RESOURCES */
    createFramebuffers();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
}

Engine::~Engine() {
    /*
     * Mostly clean up in reverse order of creation to ensure pointers are not lost.
     */
    cleanupSwapChain();

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    //<editor-fold desc="/* CLEAN UNIFORM BUFFERS */" defaultstate="collapsed">
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
    //</editor-fold>

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    cleanIndexBuffer();
    cleanVertexBuffer();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroyDevice(device, nullptr);

    if (E_VALIDATION_LAYERS) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

/**
 * @brief Cleans up the swap chain.
 */
void Engine::cleanupSwapChain() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

/**
 * @brief Recreates the swap chain when the window is resized/changed.
 */
void Engine::recreateSwapChain() {
    int width = 0, height = 0;

    // Wait until the window changes are done
    glfwGetFramebufferSize(window->glfwWindow, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window->glfwWindow, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    // Cleanup old swap chain
    cleanupSwapChain();

    // Create new swap chain
    createSwapChain();
    createImageViews();
    createFramebuffers();
}

/**
 * @brief Creates the Vulkan instance.
 */
void Engine::createInstance() {
    /* ENSURE VALIDATION LAYER SUPPORT */
    if (E_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    /* CREATE APPLICATION INFO */
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan-Tetris";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    /* FILL CREATE INFO */
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    /* GET REQUIRED EXTENSIONS */
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (E_VALIDATION_LAYERS) {
        /* GET VALIDATION LAYERS IF NEEDED */
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    /* CREATE INSTANCE */
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

/**
 * @brief Specify parameters for the debug messenger
 */
void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

/**
 * @brief Creates the debug messenger
 */
void Engine::setupDebugMessenger() {
    if (!E_VALIDATION_LAYERS) return;

    // Get debug messenger create info
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    // Create debug messenger
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

/**
 * @brief Links GLFW window to Vulkan Surface
 */
void Engine::createSurface() {
    if (glfwCreateWindowSurface(instance, window->glfwWindow, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Window surface!");
    }
}

/**
 * @brief Enumerates over physical devices and chooses the one best suited
 */
void Engine::pickPhysicalDevice() {
    uint32_t deviceCount = 0;

    // Get number of devices
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    // Get devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Check if device is suitable
    for (const auto& pDevice : devices) {
        if (isDeviceSuitable(pDevice)) {
            physicalDevice = pDevice;
            break;
        }
    }

    // No device found
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

/**
 * @brief Creates the logical device
 */
void Engine::createLogicalDevice() {
    // Get queue families
    QueueFamilyIndices qfIndices = findQueueFamilies(physicalDevice);
    std::set<uint32_t> uniqueQueueFamilies = {qfIndices.graphicsFamily.value(), qfIndices.presentFamily.value()};

    // Iterate over queue families and create queue create info
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Specify logical device info
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // Add queue families to create logical device
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    // Add device features && extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (E_VALIDATION_LAYERS) {
        // Add validation layers
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    // Attempt to create logical device
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    // Get queue handles from logical device
    vkGetDeviceQueue(device, qfIndices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, qfIndices.presentFamily.value(), 0, &presentQueue);
}

/**
 * @brief Creates the swap chain that holds the frame buffers
 */
void Engine::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    // Get number of images in swap chain
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    // Specify swap chain info
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Check if graphics and present queue families are different
    QueueFamilyIndices qfIndices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {qfIndices.graphicsFamily.value(), qfIndices.presentFamily.value()};
    if (qfIndices.graphicsFamily != qfIndices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    // Create swap chain
    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    // Get swap chain image count
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    // Put swap chain images into vector
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    // Store swap chain info
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

/**
 * @brief Describes hot the swap chain images should be used
 */
void Engine::createImageViews() {
    // Resize vector to fit all image views
    swapChainImageViews.resize(swapChainImages.size());

    // Iterate over swap chain images and create image views
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

/**
 * @brief Creates the render pass for the graphics pipeline
 */
void Engine::createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    // Not using stencil
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Specify single subpass
    VkSubpassDescription subPass{};
    subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPass.colorAttachmentCount = 1;
    // Add color attachment ref to subpass
    subPass.pColorAttachments = &colorAttachmentRef;

    // Specify render pass info
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    // Add color attachment to render pass
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subPass;

    // Create render pass
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

/**
 * @brief Descriptor set layout tells the pipeline about the uniform buffer object
 */
void Engine::createDescriptorSetLayout() {
    // Specify uniform buffer binding
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    // Add uniform buffer to vertex and fragment shader
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    // Create descriptor set layout
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

/**
 * @brief Creates the graphics pipeline
 */
void Engine::createGraphicsPipeline() {
    /* LOAD SHADERS */
    auto vertShaderCode = readFile("shd/base.vert.spv");
    auto fragShaderCode = readFile("shd/base.frag.spv");

    //<editor-fold desc="/* CREATE SHADER MODULES */" defaultstate="collapsed">
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // Vertex shader stage info
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    // Fragment shader stage info
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    //</editor-fold>

    //<editor-fold desc="/* BIND VERTICES TO VERTEX SHADER */" defaultstate="collapsed">
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    //</editor-fold>

    //<editor-fold desc="/* SPECIFY GEOMETRY FORMAT */" defaultstate="collapsed">
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    // Triangle from every 3 vertices no reuse
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    //</editor-fold>

    //<editor-fold desc="/* VIEWPORT AND SCISSOR */" defaultstate="collapsed">
    // Actual viewport and scissor is set in command buffer
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    // Allow viewport and scissor to be set dynamically in command buffer
    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    //</editor-fold>

    //<editor-fold desc="/* RASTERIZER */" defaultstate="collapsed">
    // Specify rasterizer info
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    //</editor-fold>

    //<editor-fold desc="/* MULTISAMPLING */" defaultstate="collapsed">
    // Specify multisampling info or lack thereof
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    //</editor-fold>

    //<editor-fold desc="/* COLOR BLENDING */" defaultstate="collapsed">
    // Disabled (passes through fragment shader unchanged)
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    // Specify color blending info
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    //</editor-fold>

    // Specify pipeline layout with descriptor set layout (uniform bufffer)
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

    // Create pipeline layout
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    // Specify pipeline info
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    // Create graphics pipeline
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    // Create shader modules
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

/**
 * @brief Create frame buffers to match swap chain image views for the render pass to render into
 */
void Engine::createFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    // Create a framebuffer for each image view
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 1> attachments = {
                swapChainImageViews[i]
        };

        // Specify info for the framebuffer
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        // Create framebuffer
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

/**
 * @brief Create a command pool for the graphics queue family
 */
void Engine::createCommandPool() {
    // Find queue family indices for the graphics family
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    // Specify info for the command pool
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    // Create command pool
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

/**
 * @brief Describes how an image should be used. (Was used previously to createImage views for textures as well)
 */
VkImageView Engine::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
    // Specify info for the image view
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    // Actual creation of the image view
    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

/**
 * @brief Creates required buffers to move vertices to the GPU
 */
void Engine::createVertexBuffer() {
    createBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexStagingBuffer, vertexStagingBufferMemory);
    createBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
}

/**
 * @brief Copies the vertices to the GPU. First to staging buffer, then to the vertex buffer
 */
void Engine::copyVertexBuffer() {
    void* data;
    vkMapMemory(device, vertexStagingBufferMemory, 0, vertexBufferSize, 0, &data);
    memcpy(data, objVertices.data(), objVertices.size() * sizeof (objVertices[0]));
    vkUnmapMemory(device, vertexStagingBufferMemory);

    copyBuffer(vertexStagingBuffer, vertexBuffer, vertexBufferSize);
}

/**
 * @brief Cleans up the vertex buffers
 */
void Engine::cleanVertexBuffer() {
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);
    vkDestroyBuffer(device, vertexStagingBuffer, nullptr);
    vkFreeMemory(device, vertexStagingBufferMemory, nullptr);
}

/**
 * @brief Creates required buffers to move indices to the GPU
 */
void Engine::createIndexBuffer() {
    createBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexStagingBuffer, indexStagingBufferMemory);
    createBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
}

/**
 * @brief Copies the indices to the GPU. First to staging buffer, then to the index buffer
 */
void Engine::copyIndexBuffer() {
    void* data;
    vkMapMemory(device, vertexStagingBufferMemory, 0, indexBufferSize, 0, &data);
    memcpy(data, objIndices.data(), objIndices.size() * sizeof (objIndices[0]));
    vkUnmapMemory(device, vertexStagingBufferMemory);

    copyBuffer(vertexStagingBuffer, indexBuffer, indexBufferSize);
}

/**
 * @brief Cleans up the index buffers
 */
void Engine::cleanIndexBuffer() {
    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);
    vkDestroyBuffer(device, indexStagingBuffer, nullptr);
    vkFreeMemory(device, indexStagingBufferMemory, nullptr);
}

/**
 * @brief Creates required buffers to copy the uniform buffer object into during rendering
 */
void Engine::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    // Resize the uniform buffers and memory to the amount of frames in flight
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    // Create the uniform buffers and memory
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

/**
 * @brief Get new vertices that should be drawn and update the vertex and index buffers
 * @param nVertices
 */
void Engine::updateVertices(const std::vector<Vertex> &nVertices) {
    /* CLEAN INDICES AND VERTICES */
    objIndices.clear();
    objVertices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (const auto& nVertex : nVertices) {
        // Adjust the vertex positions to screen
        Vertex vertex{
                .pos = {nVertex.pos[0] * 2 - 1.0, -1.0 * (nVertex.pos[1] * 2 - 1.0)},
                .type = nVertex.type,
                .age = nVertex.age
        };

        // Add the vertex to the list of vertices if it is not already in there
        if (uniqueVertices.count(vertex) == 0) {
            uniqueVertices[vertex] = static_cast<uint32_t>(objVertices.size());
            objVertices.push_back(vertex);
        }

        // Add the index of the vertex to the list of indices
        objIndices.push_back(uniqueVertices[vertex]);
    }

    /* UPDATE INDEX&VERTEX BUFFER SIZE */
    if (indexBufferSize <= objIndices.size() * sizeof(uint32_t) || vertexBufferSize <= objVertices.size() * sizeof(Vertex)) {
        // Clean old buffers
        cleanIndexBuffer();
        cleanVertexBuffer();

        // Update the buffer sizes to be larger
        indexBufferSize = 2 * objIndices.size() * sizeof(uint32_t);
        vertexBufferSize = 2 * objVertices.size() * sizeof(Vertex);

        // Create new buffers
        createVertexBuffer();
        createIndexBuffer();
    }

    // Copy the vertices and indices to the buffers
    copyVertexBuffer();
    copyIndexBuffer();
}

/**
 * @brief Creates a pool to allocate descriptor sets from
 */
void Engine::createDescriptorPool() {
    // Specify pool size for the normal uniform buffer
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    // Specify pool info
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    // Create the descriptor pool
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

/**
 * @brief Creates the descriptor sets
 */
void Engine::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

    // Specify the pool to alloca from
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    // Allocate the descriptor sets
    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    // Configure the descriptor sets
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        // Specify the buffer info to be used
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        // Specify which descriptor set to update
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        // Update the descriptor set
        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }
}

/**
 * @brief Creates arbitrary buffers
 */
void Engine::createBuffer(
    VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
    VkDeviceMemory &bufferMemory
) {
    // Specify buffer info
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // Create the buffer
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    // Get memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    // Specify memory allocation info
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    // Allocate memory
    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    // Bind the buffer to the memory
    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

/**
 * @brief Initializes a command buffer
 * @return
 */
VkCommandBuffer Engine::beginSingleTimeCommands() {
    // Specify command buffer allocation info
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    // Allocate command buffer
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    // Specify command buffer begin info
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    // Begin command buffer
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

/**
 * @brief Ends a single time command buffer and submits it
 * @param commandBuffer
 */
void Engine::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    // End command buffer
    vkEndCommandBuffer(commandBuffer);

    // Specify submit info
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    // Submit command buffer to queue
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    // Free command buffer
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

/*
 * @brief Copies from arbitrary source buffer to arbitrary destination buffer
 */
void Engine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    // Begin single time commands
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

/**
 * @brief Finds arbitrary device memory
 */
uint32_t Engine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    // Get memory properties
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    // Cycle through the memory types
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        // Check if the memory type is suitable
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

/**
 * @brief Allocate the command buffers
 */
void Engine::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    // Specify the primary command buffer allocation info
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    // Allocate the primary command buffers
    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

/**
 * @brief Record the all commands used to render a frame to the command buffer
 */
void Engine::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    // Specify the command buffer begin info
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    // Begin recording the command buffer
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    /* START DRAWING */

    // Specify the render pass info
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    // Specify the framebuffer to render to
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    // Attach background color to the render pass
    VkClearValue clearValue{};
    clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    // Record the render pass to the command buffer
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Bind the graphics pipeline to the command buffer
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    // Specify the dynamic viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    // Specify the dynamic scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Bind the vertex and index buffer to the command buffer
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // Bind the descriptor sets to the command buffer
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

    // Draw the indexes
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(objIndices.size()), 1, 0, 0, 0);

    // End the render pass
    vkCmdEndRenderPass(commandBuffer);

    // End recording the command buffer
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

}


/**
 * @brief Create the semaphores and fences, to synchronize rendering
 */
void Engine::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    // Specify the semaphore creation info
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    // Specify the fence creation info
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    // Create the semaphores and fences
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

/**
 * @brief Update the uniform buffer object
 */
void Engine::updateUniformBuffer(uint32_t currentImage) {
    memcpy(uniformBuffersMapped[currentImage], &uniformBufferObject, sizeof(uniformBufferObject));
}

/**
 * @brief Draw a frame to the screen
 */
void Engine::drawFrame() {
    // Wait for the fence to signal that the currentFrame is in fact finished
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // Grab the next image from the swap chain with its semaphore
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    // Check if the swap chain is out of date
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(currentFrame);
    vkResetFences(device, 1, &inFlightFences[currentFrame]);
    vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);

    // Record the command buffer
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    // Submit the command buffer to the graphics queue
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Specify the wait for the semaphores
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    // Add the actual command buffer
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    // Specify the signal semaphores upon completion
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Submit the command buffer
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    // Specify that the result should be presented to the swap chain
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    // Wait for the semaphores that will be signaled when rendering is done
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // Specify the swap chains to present to
    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Queue the image to be presented
    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    // Check if the swap chain is out of date
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    // Advance the next frame
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

/**
 * @brief Creates a shader module from a given code
 * @param code
 * @return
 */
VkShaderModule Engine::createShaderModule(const std::vector<char> &code) {
    // Specify shader info
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    // Create shader module
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

/**
 * @brief Selects a suitable swap surface format
 * @param availableFormats
 * @return
 */
VkSurfaceFormatKHR Engine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    // Cycle through the available formats and select the first fitting one
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // If no fitting format was found, just return the first one LOL
    return availableFormats[0];
}

/**
 * @brief Selects a suitable swap present mode (When the current image is presented to the screen)
 * @param availablePresentModes
 * @return
 */
VkPresentModeKHR Engine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
    // Cycle through the available present modes and select the first fitting one
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    // If no fitting present mode was found, just return the default one. Screen tearing LOL
    return VK_PRESENT_MODE_FIFO_KHR;
}

/**
 * @brief Selects a suitable swap extent (resolution of the swap chain images)
 * @param capabilities
 * @return
 */
VkExtent2D Engine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        // Can't manually select the extent
        return capabilities.currentExtent;
    } else {
        // We need to manually select the extent
        int width, height;

        // Get the window size
        glfwGetFramebufferSize(window->glfwWindow, &width, &height);

        // Init extent with the window size
        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        // Clamp the extent
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

/**
 * @brief 
 * @param pDevice 
 * @return 
 */
SwapChainSupportDetails Engine::querySwapChainSupport(VkPhysicalDevice pDevice) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

/**
 * @brief Check if the physical device is suitable for the application. Hase required queue families, extensions, swap chain support and features
 * @param pDevice The physical device to check
 * @return True if the device is suitable
 */
bool Engine::isDeviceSuitable(VkPhysicalDevice pDevice) {
    // Get queue family indices
    QueueFamilyIndices qfIndices = findQueueFamilies(pDevice);

    // Check if extensions are supported
    bool extensionsSupported = checkDeviceExtensionSupport(pDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        // Check if the device has sufficient swap chain support
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    // Check if the device supports the required features
    return qfIndices.isComplete() && extensionsSupported && swapChainAdequate;
}

/**
 * @brief Check if the device supports all required extensions
 * @param pDevice The physical device to check
 * @return True if the device supports all required extensions
 */
bool Engine::checkDeviceExtensionSupport(VkPhysicalDevice pDevice) {
    // Get extension count
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);

    // Get extension properties
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());

    // Build set of required extensions
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    // Remove required extensions from available extensions
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    // Return true if all required extensions are supported
    return requiredExtensions.empty();
}

/**
 * @brief Find the queue families supported by the device
 * @param pDevice The physical device to check
 * @return The queue families supported by the device
 */
QueueFamilyIndices Engine::findQueueFamilies(VkPhysicalDevice pDevice) {
    QueueFamilyIndices qfIndices;

    // Get queue family count
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);

    // Get queue family properties
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilies.data());

    // Iterate over queue families
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        // Check for graphics support
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            qfIndices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        // Check for present support
        vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, surface, &presentSupport);
        if (presentSupport) {
            qfIndices.presentFamily = i;
        }

        // Check if both a graphics and present queue family have been found
        if (qfIndices.isComplete()) {
            break;
        }

        i++;
    }

    // Return queue family indices
    return qfIndices;
}

/**
 * @brief Gets all required extensions
 *
 * @return Required extensions
 */
std::vector<const char *> Engine::getRequiredExtensions() {
    /* ADD GLFW LAYER EXTENSIONS */
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    /* ADD VALIDATION LAYER EXTENSION */
    if (E_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

/**
 * @brief Checks if all the requested validation layers are available
 *
 * @return True if all the requested validation layers are available
 */
bool Engine::checkValidationLayerSupport() {
    /* READ LAYERS INTO VECTOR */
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    /* CHECK IF ALL REQUESTED LAYERS ARE AVAILABLE */
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        /* SEARCH FOR SPECIFIC LAYER */
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        /* IF A LAYER IS NOT FOUND, RETURN FALSE */
        if (!layerFound) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Reads a file into a buffer
 * @param filename The file to read
 *
 * @return The buffer containing the file
 */
std::vector<char> Engine::readFile(const std::string &filename) {
    /* OPEN FILE IS VALID*/
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    /* ENSURE FILE IS VALID*/
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    /* GET FILE SIZE AND ALLOCATE BUFFER */
    std::streamsize fileSize = (std::streamsize) file.tellg();
    std::vector<char> buffer(fileSize);

    /* RESET FILE POINTER AND READ FILE */
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

/**
 * @brief Creates a debug messenger
 * @param instance The instance to create the debug messenger for
 * @param createInfo The debug messenger creation info
 * @param allocator The allocator to use
 * @param debugMessenger The debug messenger to create
 *
 * @return The result of the debug messenger creation
 */
VkBool32 Engine::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData
) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

/**
 * @brief Wrapper to wait for the device to be idle
 */
void Engine::waitDeviceIdle() {
    vkDeviceWaitIdle(device);
}

/**
 * @brief Adjust how the rendering is done
 * @param keyboard
 */
void Engine::updateSettings(Keyboard keyboard) {
    if (keyboard.n1) {
        uniformBufferObject.model = 0;
    }
    else if (keyboard.n2) {
        // Hard mode
        uniformBufferObject.model = 1;
    }
}

