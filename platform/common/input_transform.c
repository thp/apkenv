
#include "../../apkenv.h"

#include "input_transform.h"

extern struct ModuleHacks global_module_hacks;
extern struct GlobalState global;

int input_transform_x(int x, int y)
{
    int width, height;
    float aspect_ratio;
    global.platform->get_size(&width, &height);
    aspect_ratio = (float)width / (float)height;

    /* no rotation hack for glesv2 */
    if(global.loader_seen_glesv2) return x;

    /* custom rotation hack for world of goo */
    if(global_module_hacks.gles_viewport_hack) return y;

    if(global_module_hacks.current_orientation != global.platform->get_orientation()) {
        if(global_module_hacks.current_orientation == ORIENTATION_LANDSCAPE) {
            return y * aspect_ratio;
        }
        else {
            return height - y;
        }
    }
    else {
        return x;
    }
}

int input_transform_y(int x, int y)
{
    int width, height;
    float aspect_ratio;
    global.platform->get_size(&width, &height);
    aspect_ratio = (float)width / (float)height;

    /* no rotation hack for glesv2 */
    if(global.loader_seen_glesv2) return y;

    /* custom rotation hack for world of goo */
    if(global_module_hacks.gles_viewport_hack) return width - x;

    if(global_module_hacks.current_orientation != global.platform->get_orientation()) {
        if(global_module_hacks.current_orientation == ORIENTATION_LANDSCAPE) {
            return height - x/aspect_ratio;
        }
        else {
            return x;
        }
    }
    else {
        return y;
    }
}

float input_transform_float_x(float x, float y)
{
    int width, height;
    global.platform->get_size(&width, &height);

    /* no rotation hack for glesv2 */
    if(global.loader_seen_glesv2) return x;

    if(global_module_hacks.current_orientation != global.platform->get_orientation()) {
        if(global_module_hacks.current_orientation == ORIENTATION_LANDSCAPE) {
            return y;
        }
        else {
            return x;
        }
    }
    else {
        return x;
    }
}

float input_transform_float_y(float x, float y)
{
    int width, height;
    global.platform->get_size(&width, &height);

    /* no rotation hack for glesv2 */
    if(global.loader_seen_glesv2) return y;

    if(global_module_hacks.current_orientation != global.platform->get_orientation()) {
        if(global_module_hacks.current_orientation == ORIENTATION_LANDSCAPE) {
            return x;
        }
        else {
            return y;
        }
    }
    else {
        return y;
    }
}

