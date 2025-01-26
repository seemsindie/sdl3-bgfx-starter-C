#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
// #include <SDL3/SDL_main.h>
#include <bgfx/c99/bgfx.h>
#include <SDL3/SDL.h>
#if defined(SDL_PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif

void *get_native_window_handle(SDL_Window *window)
{
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
    {
        Display *xdisplay = (Display *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
        Window xwindow = (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        if (xdisplay && xwindow)
        {
            return (void *)(uintptr_t)xwindow;
        }
    }
    else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
    {
        struct wl_display *display = (struct wl_display *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
        struct wl_surface *surface = (struct wl_surface *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
        if (display && surface)
        {
            return (void *)surface;
        }
    }
}

void *get_native_display_handle(SDL_Window *window)
{
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
    {
        Display *xdisplay = (Display *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
        Window xwindow = (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        if (xdisplay && xwindow)
        {
            return (void *)(uintptr_t)xdisplay;
        }
    }
    else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
    {
        struct wl_display *display = (struct wl_display *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
        struct wl_surface *surface = (struct wl_surface *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
        if (display && surface)
        {
            return (void *)display;
        }
    }
}

int main(int argc, char *argv[])
{
    int width = 640;
    int height = 480;

    // SDL_SetMainReady(); // not sure about this one
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Hello SDL", width, height, SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        SDL_Log("could not create window: %s\n", SDL_GetError());
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    bgfx_init_t init;
    bgfx_init_ctor(&init);

    init.type = BGFX_RENDERER_TYPE_VULKAN;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData.ndt = get_native_display_handle(window);
    init.platformData.nwh = get_native_window_handle(window);

    bgfx_init(&init);

    bgfx_reset(width, height, BGFX_RESET_VSYNC, init.resolution.format);
    bgfx_set_debug(BGFX_DEBUG_TEXT);
    bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    SDL_Event event;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
            {
                width = event.window.data1;
                height = event.window.data2;
                bgfx_reset(width, height, BGFX_RESET_VSYNC, init.resolution.format);
            }
        }

        bgfx_set_view_rect(0, 0, 0, width, height);
        bgfx_touch(0);

        bgfx_dbg_text_clear(0, false);
        bgfx_dbg_text_printf(0, 1, 0x4f, "Hello, bgfx: %d x %d!", width, height);

        bgfx_frame(false);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}