#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <bgfx/c99/bgfx.h>

#if defined(SDL_PLATFORM_WIN32)
#include <windows.h>
#elif defined(SDL_PLATFORM_MACOS)
#include <Cocoa/Cocoa.h>
#elif defined(SDL_PLATFORM_LINUX)
#include <X11/Xlib.h>
#include <wayland-client.h>
#elif defined(SDL_PLATFORM_EMSCRIPTEN)
#include <emscripten.h>
#endif

bgfx_program_handle_t load_shader_embedded(const uint8_t *vertex_shader,
                                           const uint32_t vs_size,
                                           const uint8_t *fragment_shader,
                                           const uint32_t fs_size)
{
    const bgfx_memory_t *vertex_mem = bgfx_make_ref(vertex_shader, vs_size);
    const bgfx_memory_t *fragment_mem = bgfx_make_ref(fragment_shader, fs_size);

    bgfx_shader_handle_t vertex_handle = bgfx_create_shader(vertex_mem);
    bgfx_shader_handle_t fragment_handle = bgfx_create_shader(fragment_mem);

    if (!BGFX_HANDLE_IS_VALID(vertex_handle) ||
        !BGFX_HANDLE_IS_VALID(fragment_handle))
    {
        fprintf(stderr, "Failed to create shaders from embedded data\n");
        exit(1);
    }

    bgfx_program_handle_t program = bgfx_create_program(vertex_handle, fragment_handle, true);

    if (!BGFX_HANDLE_IS_VALID(program))
    {
        fprintf(stderr, "Failed to create program from embedded shaders\n");
        exit(1);
    }

    // The shaders are destroyed automatically when the program is destroyed
    // because we passed 'true' as the last parameter to bgfx_create_program

    return program;
}

void destroy_shader_program(bgfx_program_handle_t program)
{
    bgfx_destroy_program(program);
}

void* get_native_window_handle(SDL_Window* window) {
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    if (!props) {
        return NULL;
    }

#if defined(SDL_PLATFORM_WIN32)
    return (void*)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

#elif defined(SDL_PLATFORM_MACOS)
    return (void*)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);

#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
        return (void*)(uintptr_t)SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
        return (void*)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
    }

#elif defined(SDL_PLATFORM_IOS)
    return (void*)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, NULL);

#elif defined(SDL_PLATFORM_ANDROID)
    return (void*)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER, NULL);

#elif defined(SDL_PLATFORM_EMSCRIPTEN)
    // For Emscripten, return the canvas ID as a string
    // Note: The caller needs to manage this string's lifecycle
    const char* canvas_id = SDL_GetStringProperty(props, "canvas", "#canvas");
    return (void*)SDL_strdup(canvas_id);
#endif

    return NULL;
}

void* get_native_display_handle(SDL_Window* window) {
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    if (!props) {
        return NULL;
    }

#if defined(SDL_PLATFORM_WIN32)
    // Windows doesn't really have a display handle concept in the same way
    return NULL;

#elif defined(SDL_PLATFORM_MACOS)
    // For macOS, you might want the NSScreen object
    return NULL;

#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
        return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
    } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
        return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
    }

#elif defined(SDL_PLATFORM_EMSCRIPTEN)
    // Web doesn't have a display handle concept
    return NULL;
#endif

    return NULL;
}