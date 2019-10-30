#include "config.h"
#include "player.h"
#include <SDL.h>
#include <engine/camera.h>
#include <engine/color.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/shader.h>
#include <engine/list.h>
#include <engine/logger.h>
#include <engine/settings.h>
#include <engine/tilemap.h>
#include <engine/ui/button.h>
#include <engine/ui/progress_bar.h>
#include <engine/ui/switch.h>
#include <engine/ui/textbox.h>
#include <stdlib.h>

// TODO: animated bar, dropdown

int main(int argc, const char *argv[]) {

    if (!render_init(1024, 768, "SimpleGame")) {
        log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    util_init();

    render_clear_color(COLOR_WHITE);

    Rect screen = util_rect(0, 0, settings_get_int("window_width"),
                            settings_get_int("window_height"));

    Point mouse;

    Camera *camera = camera_create();
    // camera_lookat(camera, 0, 0, 0);

    char aFpsBuf[64];
    double delta = util_delta_time();
    double fps = util_fps();
    double averageFps = util_fps();
    double fpsTime = 0;
    int fpsTimesAdded = 1;

    Player *p = player_create(100, 500, 100, 100);
    p->rect.x = 500;
    p->rect.y = 400;

    Point prevPos = util_point(0, 0);
    Point curPos = util_point(0, 0);

    int grab = 0;

    while (1) {
        // TODO: wrap this
        util_update_keyboard();
        SDL_Event event;
        int mdx = 0;
        int mdy = 0;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto cleanup;
            } else if (event.type == SDL_MOUSEMOTION) {
                mdx = event.motion.xrel;
                mdy = event.motion.yrel;
            }
        }

        // Update
        util_update();
        delta = util_delta_time();
        double deltaS = delta / 1000;
        double deltaX = 1000 / delta;

        if (util_is_keyup(SDL_SCANCODE_K)) {
            grab = !grab;
            SDL_SetRelativeMouseMode(grab);
            SDL_ShowCursor(grab);
        }

        fpsTime += delta;
        if (fpsTime > 500) {
            averageFps = fps / fpsTimesAdded;
            fps = util_fps();
            fpsTimesAdded = 1;
            fpsTime = 0;
        } else {
            fps += util_fps();
            fpsTimesAdded++;
        }

        vec3 axis;
        glm_vec3_zero(axis);
        if (mdx != 0) {
            axis[1] = 1;
            camera_rotate(camera, -mdx * deltaS, axis);
        }
        if (mdy != 0) {
            axis[0] = 1;
            camera_rotate(camera, mdy * deltaS, axis);
        }

        if (util_is_keypress(SDL_SCANCODE_D))
            camera_move(camera, STRAFE_LEFT, 10 * deltaS);
        else if (util_is_keypress(SDL_SCANCODE_A))
            camera_move(camera, STRAFE_RIGHT, 10 * deltaS);
        if (util_is_keypress(SDL_SCANCODE_W))
            camera_move(camera, MOVE_FORWARD, 10 * deltaS);
        else if (util_is_keypress(SDL_SCANCODE_S))
            camera_move(camera, MOVE_BACKWARD, 10 * deltaS);
        /*if(util_is_keypress(SDL_SCANCODE_SPACE))
                directionY = -1;
        else if(util_is_keypress(SDL_SCANCODE_LSHIFT))
                directionY = 1;
*/
        log_info("%f %f %f, %d, %d\n", camera->pos[0], camera->pos[1],
                 camera->pos[2], mdx, mdy);
        util_str_format(aFpsBuf, sizeof(aFpsBuf), "FPS: %.02f", averageFps);

        mouse = util_mouse_pos();
        Point coords;

        camera_update(camera);

        // Render
        render_clear();

        render_voxel(0, 0, 0, 5);

        render_present();
        // SDL_Delay(1);
    }

cleanup:
    util_quit();
    settings_save("settings.ini");
    settings_quit();
    render_quit();
    return EXIT_SUCCESS;
}
