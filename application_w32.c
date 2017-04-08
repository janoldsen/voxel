#define VK_USE_PLATFORM_WIN32_KHR
#define UNICODE
#include "vulkan.h"
#include "application.h"

#include <windows.h>
#include <stdio.h>




#define MAX_WINDOWS 4
struct app
{
    unsigned window_count;
    struct window *windows[MAX_WINDOWS];
    unsigned root_window_count;
};

struct window
{
    HWND h_wnd;
    struct app *app;
};



LRESULT CALLBACK window_proc(HWND wnd_h, UINT msg, WPARAM w_param, LPARAM l_param)
{

    switch(msg)
    {
    case WM_CLOSE:
	PostQuitMessage(0); 
    break;
    default:
	return DefWindowProc(wnd_h, msg, w_param, l_param);	
    }
	


  
}


#define MAX_NAME_LENGTH 255

app_handle create_app()
{
    HINSTANCE h_instance = GetModuleHandle(NULL);
    
    WCHAR *className = L"app_class";
	
    WNDCLASS wc = {
	.lpfnWndProc = window_proc,
	.hInstance = h_instance,
	.lpszClassName = className
    }; 
	
    RegisterClassW(&wc);


  
    struct app *app_ptr = malloc(sizeof(struct app));


    return app_ptr;
  
}

void destroy_app(app_handle app)
{
    free(app);

}

struct window *create_window(struct app *app, struct window_description *descr, struct window *parent_window)
{
    HINSTANCE h_instance = GetModuleHandle(NULL);

    WCHAR name[MAX_NAME_LENGTH];
    MultiByteToWideChar(CP_UTF8, 0, descr->name, -1, name, MAX_NAME_LENGTH);

    WCHAR *className = L"app_class";
    
    HWND wnd_h = CreateWindowEx(
				0,
				className,
				name,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				descr->width, descr->height,
				NULL, NULL,
				h_instance,
				NULL
				);

    if (wnd_h == NULL)
	return NULL;

    ShowWindow(wnd_h, TRUE);

    struct window *window = malloc(sizeof(struct window));

    window->h_wnd = wnd_h;
    window->app = app;
    
    return window;
}

void destroy_window(struct window *window)
{
    DestroyWindow(window->h_wnd);
    free(window);
}

VkSurfaceKHR create_window_surface(struct window *window, VkInstance vk_instance)
{  
    struct VkWin32SurfaceCreateInfoKHR surface_create_info =
	{
	    .sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR,
	    .hinstance = GetModuleHandle(NULL),
	    .hwnd = window->h_wnd
	};

    VkSurfaceKHR surface;
    vkCreateWin32SurfaceKHR(vk_instance, &surface_create_info, NULL, &surface);

    return surface;
}


int update_app(app_handle app)
{

    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
	    if (msg.message == WM_QUIT)
		{
		    return 0;
		}
		

	    
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}

    return 1;
}
		
