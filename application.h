#ifndef JOLD_APPLICATION_H
#define JOLD_APPLICATION_H

#include <vulkan/vulkan.h>

/* 
todo
-proper close application condition
-input definitions
*/

typedef struct app *app_handle;
typedef struct window *window_handle;

struct window_description
{
  unsigned width;
  unsigned height;
  const char* name;
};


app_handle create_app();
void destroy_app(app_handle app);

window_handle create_window(app_handle app, struct window_description *descr, window_handle parent);
void destroy_window(window_handle window);

VkSurfaceKHR create_window_surface(window_handle window, VkInstance vk_instance);

int update_app(app_handle app);

// input

void swap_inputs(window_handle handle);

void get_down_keys(window_handle handle);
void get_pressed_keys(window_handle handle);
void get_text_inputs(window_handle handle);




#endif
