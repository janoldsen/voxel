#ifndef JOLD_VULKAN_H
#define JOLD_VULKAN_H

#include <vulkan/vulkan.h>

VkInstance create_instance();
VkPhysicalDevice create_physical_device(VkInstance instance);
VkDevice create_device(VkPhysicalDevice physical_device);

#endif
