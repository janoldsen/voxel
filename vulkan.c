#include "vulkan.h"
#include <string.h>
#include <stdio.h>

#ifdef DEBUG
static const char* LAYER_NAMES[] =
    {
	"VK_LAYER_LUNARG_standard_validation",
	"VK_LAYER_GOOGLE_threading",
	"VK_LAYER_LUNARG_parameter_validation",
	"VK_LAYER_LUNARG_object_tracker",
	"VK_LAYER_LUNARG_image",
	"VK_LAYER_LUNARG_core_validation"
	"VK_LAYER_LUNARG_swapchain",
	"VK_LAYER_GOOGLE_unique_objects",
	"VK_LAYER_LUNARG_screenshot",
	/* VK_LAYER_RENDERDOC_Capture" */
    };
#elif

static const LAYER_NAMES[] = {};

#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

VkInstance create_instance()
{

    struct VkApplicationInfo app_info =
	{
	    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	    .pNext = NULL,
	    .pApplicationName = "test_app",
	    .applicationVersion = 0,
	    .pEngineName = "jold_voxel",
	    .engineVersion = 0,
	    .apiVersion = 0
	};


    uint32_t instance_layer_property_count;
    vkEnumerateInstanceLayerProperties(&instance_layer_property_count, NULL);
    VkLayerProperties instance_layer_properties[instance_layer_property_count];
    vkEnumerateInstanceLayerProperties(&instance_layer_property_count, instance_layer_properties);

    /* remove unwanted layers */

    unsigned enabled_layer_count = 0;
    const char* enabled_layer_names[instance_layer_property_count];
    
    
    
    for (unsigned i = 0; i < instance_layer_property_count; ++i)
	{
	    printf("layer_name %s\n", instance_layer_properties[i].layerName);
	    for(unsigned j = 0; j < ARRAY_SIZE(LAYER_NAMES); ++j)
		{
		    if (strcmp(instance_layer_properties[i].layerName, LAYER_NAMES[j]) == 0)
			{
			    enabled_layer_names[enabled_layer_count++] = instance_layer_properties[i].layerName;
			    break;
			}
		}
	}

    printf("enabled_layers %d\n", enabled_layer_count);

    
    
    struct VkInstanceCreateInfo create_info =
	{
	    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	    .pNext = NULL,
	    .flags = 0,
	    .pApplicationInfo = &app_info,
	    .enabledLayerCount = enabled_layer_count,
	    .ppEnabledLayerNames = enabled_layer_names,
	    .enabledExtensionCount = 0,
	    .ppEnabledExtensionNames  = NULL
	};

    VkInstance instance;
    vkCreateInstance(&create_info, NULL, &instance);


    return instance;
}


VkPhysicalDevice create_physical_device(VkInstance instance)
{
    uint32_t phys_dev_count = 1;
    VkPhysicalDevice physical_device;
    vkEnumeratePhysicalDevices(instance, &phys_dev_count, &physical_device);

    return physical_device;
}

VkDevice create_device(VkPhysicalDevice physical_device)
{
    float queue_priority = 1.0f;
    struct VkDeviceQueueCreateInfo device_queue_create_info =
	{
	    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	    .queueCount = 1,
	    .pQueuePriorities = &queue_priority
	};


    uint32_t device_layer_property_count;
    vkEnumerateDeviceLayerProperties(physical_device, &device_layer_property_count, NULL);
    VkLayerProperties device_layer_properties[device_layer_property_count];
    vkEnumerateDeviceLayerProperties(physical_device, &device_layer_property_count, device_layer_properties);

    unsigned enabled_layer_count = 0;
    const char* enabled_layer_names[device_layer_property_count];

    for (unsigned i = 0; i < device_layer_property_count; ++i)
	{
	    printf("layer_name %s\n", device_layer_properties[i].layerName);
	    for(unsigned j = 0; j < ARRAY_SIZE(LAYER_NAMES); ++j)
		{
		    if (strcmp(device_layer_properties[i].layerName, LAYER_NAMES[j]) == 0)
			{
			    enabled_layer_names[enabled_layer_count++] = device_layer_properties[i].layerName;
			    break;
			}
		}
	}

    
    struct VkDeviceCreateInfo create_info =
	{
	    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	    .queueCreateInfoCount = 1,
	    .pQueueCreateInfos = &device_queue_create_info,
	    .enabledLayerCount = enabled_layer_count,
	    .ppEnabledLayerNames = enabled_layer_names
	};

    VkDevice device;
    vkCreateDevice(physical_device, &create_info, NULL, &device);
}
