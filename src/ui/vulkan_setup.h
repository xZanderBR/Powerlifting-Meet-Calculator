#pragma once

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace VulkanSetup {

    // Initialize Vulkan with the required instance extensions
    void InitializeVulkan(ImVector<const char*>& instance_extensions);

    // Setup Vulkan window with given surface and dimensions
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

    // Cleanup Vulkan resources
    void CleanupVulkan();

    // Cleanup Vulkan window resources
    void CleanupVulkanWindow();

    // Render a frame
    void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);

    // Present a frame
    void FramePresent(ImGui_ImplVulkanH_Window* wd);

    // Vulkan global variables
    extern VkAllocationCallbacks*   g_Allocator;
    extern VkInstance               g_Instance;
    extern VkPhysicalDevice         g_PhysicalDevice;
    extern VkDevice                 g_Device;
    extern uint32_t                 g_QueueFamily;
    extern VkQueue                  g_Queue;
    extern VkDebugReportCallbackEXT g_DebugReport;
    extern VkPipelineCache          g_PipelineCache;
    extern VkDescriptorPool         g_DescriptorPool;

    extern ImGui_ImplVulkanH_Window g_MainWindowData;
    extern int                      g_MinImageCount;
    extern bool                     g_SwapChainRebuild;

    // Utility functions
    void CheckVkResult(VkResult err);
    bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);

}
