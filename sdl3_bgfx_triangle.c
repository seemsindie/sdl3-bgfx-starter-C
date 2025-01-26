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

#include "utils.h"
#include "shaders/spirv/fs_basic_triangle.sc.bin.h"
#include "shaders/spirv/vs_basic_triangle.sc.bin.h"

typedef struct PosColorVertex
{
    float x, y, z;
    uint32_t abgr;
} PosColorVertex;

static PosColorVertex s_vertices[3] = {
    {-0.5f, -0.5f, 0.0f, 0xff0000ff}, // Red
    {0.5f, -0.5f, 0.0f, 0xff00ff00},  // Green
    {0.0f, 0.5f, 0.0f, 0xffff0000}    // Blue
};

static const uint16_t s_indices[3] = {0, 1, 2};

bgfx_program_handle_t shader_program;
bgfx_vertex_layout_t layout;
bgfx_vertex_buffer_handle_t vbh;
bgfx_index_buffer_handle_t ibh;

void setup_buffers_and_shaders()
{
    bgfx_vertex_layout_begin(&layout, bgfx_get_renderer_type());
    bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_POSITION, 3,
                           BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8,
                           true, false);
    bgfx_vertex_layout_end(&layout);

    const bgfx_memory_t *vertex_mem = bgfx_copy(s_vertices, sizeof(s_vertices));
    vbh = bgfx_create_vertex_buffer(vertex_mem, &layout, BGFX_BUFFER_NONE);

    const bgfx_memory_t *index_mem = bgfx_copy(s_indices, sizeof(s_indices));
    ibh = bgfx_create_index_buffer(index_mem, BGFX_BUFFER_NONE);

    shader_program = load_shader_embedded(vs_basic_triangle_spv, sizeof(vs_basic_triangle_spv), fs_basic_triangle_spv, sizeof(fs_basic_triangle_spv));
    // u_user_color = bgfx_create_uniform("u_user_color", BGFX_UNIFORM_TYPE_VEC4, 1);
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

    SDL_Window *window = SDL_CreateWindow("Hello SDL", width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

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

    setup_buffers_and_shaders();

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

        bgfx_set_vertex_buffer(0, vbh, 0, 3);
        bgfx_set_index_buffer(ibh, 0, 3);

        bgfx_set_state(BGFX_STATE_DEFAULT, 0);
        bgfx_submit(0, shader_program, 0, false);

        bgfx_frame(false);

        bgfx_set_view_rect(0, 0, 0, width, height);
        bgfx_dbg_text_clear(0, false);
        bgfx_dbg_text_printf(0, 1, 0x4f, "Hello, bgfx: %d x %d!", width, height);

        bgfx_touch(0);
    }

    bgfx_destroy_program(shader_program);
    bgfx_shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}