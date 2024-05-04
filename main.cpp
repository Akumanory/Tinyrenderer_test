#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <tga_image.h>
#include <tuple>
#include <utility>
#include <geometry.h>
#include <model.h>

// Defines
#define MODEL_DRAWING 1
#define LINES_DRAWING 0
#define TRIANGLES_DRAWING 0

namespace cfg {
// Screen size
const int32_t width     = 800;
const int32_t height    = 800;
const double  half_coef = 2.0;
} // namespace cfg

// Methods declarations 
void line(
    int32_t x_0, int32_t y_0,
    int32_t x_1, int32_t y_1,
    TGAImage &image, const TGAColor &color);

// Start point

int main()
{
    // setting colors data
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor green = TGAColor(  0, 255,   0, 255);
    const TGAColor red   = TGAColor(255,   0,   0, 255);
    const TGAColor blue  = TGAColor(  0,   0, 255, 255);

    std::ignore = green;
    std::ignore = red;
    std::ignore = blue;


    if(MODEL_DRAWING + TRIANGLES_DRAWING + LINES_DRAWING > 1) {
        std::cout << "Error can't define multiple types at once" << '\n';
        return 0;
    }


	TGAImage image(
        cfg::width, cfg::height, 
        TGAImage::Format::RGB);

#if MODEL_DRAWING
    // TODO(akumanory): Relative path works only for the exec
    // so for the recource path to work we need to go back from build/Debug
    // into the main directory by .. to escape folders
    Model model("../../resources/obj/african_head.obj");
    for (int i = 0; i < model.nfaces(); i++) {
        std::vector<int> face = model.face(i);
        for (size_t j = 0; j < 3; j++) {
            Vec3f vert_0 = model.vert(face[j]);
            Vec3f vert_1 = model.vert(face[(j + 1) % 3]);
            
            int x_0 = static_cast<int>((vert_0.x + 1.) * cfg::width  / cfg::half_coef);
            int y_0 = static_cast<int>((vert_0.y + 1.) * cfg::height / cfg::half_coef);
            int x_1 = static_cast<int>((vert_1.x + 1.) * cfg::width  / cfg::half_coef);
            int y_1 = static_cast<int>((vert_1.y + 1.) * cfg::height / cfg::half_coef);
            line(x_0, y_0, x_1, y_1, image, white);
        }
    }
#endif // MODEL_DRAWING    
#if LINES_DRAWING
    int32_t screen_x = cfg::width  - 1;
    int32_t screen_y = cfg::height - 1;
    line(0, 0, 
         screen_y, 0, 
         image, cfg::color::green); 
    line(screen_x, screen_x, 
         0, screen_y, 
         image, cfg::color::white);
    line(0, 0, 
         screen_y, screen_y, 
         image, cfg::color::red); 
    line(screen_x, 0, 
         0, screen_y, 
         image, cfg::color::blue);
#endif // LINES_DRAWING
#if TRIANGLES_DRAWING
    
#endif // TRIANGLES_DRAWING
	image.flip_vertically(); // NOTE(akumanory): i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

    return 0;
}

// Methods defenitions
void line(
    int32_t x_0, int32_t y_0,
    int32_t x_1, int32_t y_1,
    TGAImage &image, const TGAColor &color
) {
    bool steep = false;
    if (std::abs(x_0 - x_1) < std::abs(y_0 - y_1)) {
        std::swap(x_0, y_0);
        std::swap(x_1, y_1);
        steep = true;
    }
    if (x_0 > x_1) {
        std::swap(x_0, x_1);
        std::swap(y_0, y_1);
    }
    // TODO(akumanory): maybe make a struct for this thing
    // because of too many variable, maybe can make compact
    // and more readable (in the other hand maybe not necessesary)
    int32_t delta_x = x_1 - x_0;
    int32_t delta_y = y_1 - y_0;
    int32_t d_error2 = std::abs(delta_y) * 2;
    int32_t error2 = 0;
    int32_t y_final = y_0;

    for (int32_t x_final = x_0; x_final <= x_1 ; x_final++) {
        if (steep) {
            image.set(y_final, x_final, color);   
        } else {
            image.set(x_final, y_final, color);
        }
        error2 += d_error2;
        if (error2 > delta_x) {
            y_final += (y_1 > y_0 ? 1 : -1); 
            error2 -= delta_x * 2;
        }
    }
}
