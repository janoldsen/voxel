#include "renderer.h"
#inbclude "vulkan.h"


#define MAX_SWAPCHAIN_IMAGE_COUNT 3
struct swapchain
{
    VkSurfaceKHR surface;
    VkSwapchain swapchain;
    unsigned image_count;
    VkImage images[MAX_SWAPCHAIN_IMAGE_COUNT];
    
};


struct renderer
{
    VkInstance instance;
    VkPhysicalDevice phys_device;
    VkDevice device;
    window_handle window;
    struct swapchain swapchain;
    uint32_t present_queue_family;
    VkQueue present_queue;
    VkCommandPool present_command_pool;
};



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

void create_instance(struct renderer* renderer)
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

    renderer->instance = instance;
    
}


void create_physical_device(struct renderer* renderer)
{
    uint32_t phys_dev_count = 1;
    VkPhysicalDevice physical_device;
    vkEnumeratePhysicalDevices(renderer->instance, &phys_dev_count, &physical_device);

    renderer->phys_device = physical_device;
}

void create_device(struct renderer *renderer)
{
    float queue_priority = 1.0f;
    struct VkDeviceQueueCreateInfo device_queue_create_info =
	{
	    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	    .queueFamilyIndex = renderer->present_queue_family;
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

    renderer->device = device;    
        
}
 
void create_swap_chain(struct renderer *renderer)
{
    VkSurface surface = create_window_surface(renderer->window, renderer->instance);

    
    uin32_t queue_family_property_count;
    VkPhysicalDeviceMemroyProperties(physical_device, &queue_family_property_count, NULL);

    VkQueueFamilyProperties queue_family_properties[queue_family_property_count];
    VkPhysicalDeviceMemroyProperties(physical_device, &queue_family_property_count, queue_family_properties);

    unsigned present_queue_flags = VK_QUEUE_COMPUTE_ BIT;
    for (unsigned i = 0; i < queue_family_property_count; ++i)
	{
	    VkBool32 supported;
	    vkGetPhysicalDeviceSurfaceSupportKHR(renderer->phys_device,
						 i,
						 surface,
						 &supported);
	    
	    if (queue_family_property[i] & present_queue_flags
		&& supported)
		{
		    renderer->present_queue_family = i;
		    break;
		}
	}


    VkQueue present_queue;
    vkGetDeviceQueue(device, renderer->present_queue_family, 0, &present_queue);

    renderer->present_queue = persent_queue;

    struct VkCommandPoolCreateInfo command_pool_create_info =
	{
	    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
	    .queueFamilyIndex = renderer->present_queue_family_index
	};

    VkCommandPool command_pool;
    vkCreateCommandPool(renderer->device, &command_pool_create_info, NULL, &command_pool);

    renderer->present_command_pool = command_pool;
    

    VkSurfaceCapabilitiesKHR surface_capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderer->phys_device, surface, &surface_capabilities);



    
    VkSwapchainCreateInfoKHR swapchain_create_info =
	{
	    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	    .surface = renderer->swapchain.surface,
	    .minImageCount = MAX_SWAPCHAIN_IMAGE_COUNT,
	    .imageFormat = VK_FORMAT_R8G8B8A8_UNORM,
	    .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
	    .imageExtent = surface_capabilities.currentExtent,
	    .imageArrayLayers = 1,
	    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
	    .pQueueFamilyIndices = &renderer->persent_queue_family,
	    .preTransform = surface_capabilities.currentTransform,
	    .presentMode = VK_PRESENT_MODE_FIFO_KHR
	    .oldSwapchain = VK_INVALID_HANDLE
	};

    VkSwapchain swapchain;
    vkCreateSwapchainKHR(renderer->device, &swapchain_create_info, NULL, &swapchain);
    renderer->swapchain.surface = surface;
    renderer->swapchain.swapchain = swapchain;

    renderer->swapchain.image_count = MAX_SWAPCHAIN_IMAGE_COUNT;
    vkGetSwapchainImagesKHR(renderer->device,
			    renderer->swapchain.swapchain,
			    renderer->swapchain.image_count,
			    renderer->swapchain.images);

    
    
}

  

struct renderer *create_renderer(window_handle window)
{
    struct renderer *renderer = malloc(sizeof(renderer));

    renderer->window = window;

    create_instance(renderer);
    create_physical_device(renderer);
    create_device(renderer);

    create_swap_chain(renderer);

    
}


void update_renderer(struct renderer *renderer)
{
    struct VkCommandBufferAllocateInfo command_buffer_allocate_info =
	{
	    .sType = VK_STRUCTURE_COMMAND_BUFFER_ALLOCATE_INFO,
	    .commandPool = renderer->present_command_pool,
	    .bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	    .commandBufferCount = 1
	};
    
    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(renderer->device, &command_buffer_allocate_info, &command_buffer);


    struct VkCommandBufferBeginInfo command_buffer_begin_info =
	{
	    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	    
	};
    
    vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
    
    
    

    vkFreeCommandBuffers(renderer->device, renderer->present_command_pool, 1, &command_buffer);

}
