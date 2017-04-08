#include <stdio.h>
#include "vulkan.h"
#include "application.h"


int main()
{

    VkInstance vk_instance = create_instance();
    VkPhysicalDevice vk_physical_device = create_physical_device(vk_instance);
    VkDevice vk_device = create_device(vk_physical_device);
    
    
    struct window_description window_descr =
	{
	    .width = 800,
	    .height = 600,
	    .name = "voxel"
	};
  
    app_handle app = create_app();
    window_handle window = create_window(app, &window_descr, NULL);

    
    while(app && update_app(app))
	{
	
	}

    destroy_app(app);
    return 0;
}
