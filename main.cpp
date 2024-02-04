#include <cstdint>
#include <cstdlib>
#include <tga_image.h>
#include <utility>
#include <geometry.h>
#include <model.h>

// Static variables

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor green = TGAColor(  0, 255,   0, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor blue  = TGAColor(  0,   0, 255, 255); // NOTE(akumanory): don't use when night lighte is acite, because disables all blue 

Model *model = nullptr; 
const int width  = 800;
const int height = 800;

// Methods declarations 

void line(
    int32_t x0, int32_t y0,
    int32_t x1, int32_t y1,
    TGAImage &image, TGAColor color);

// Start point

int main()
{
    // TODO(akumanory): Relative path works only for the exec
    // so for the recource path to work we need to go back from build/Debug
    // into the main directory by .. to escape folders
    model = new Model("../../resources/obj/african_head.obj");

	TGAImage image(width, height, TGAImage::Format::RGB);

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (size_t j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = static_cast<int>((v0.x + 1.) * width  / 2.);
            int y0 = static_cast<int>((v0.y + 1.) * height / 2.);
            int x1 = static_cast<int>((v1.x + 1.) * width  / 2.);
            int y1 = static_cast<int>((v1.y + 1.) * height / 2.);
            line(x0, y0, x1, y1, image, white);
        }
    }
	// image.set(52, 41, green);
    // line(0, 0, 99, 0, image, green); 
    // line(99, 99, 0, 99, image, white);
    // line(0, 0, 99, 99, image, red); 
    // line(99, 0, 0, 99, image, blue);
	image.flip_vertically(); // NOTE(akumanory): i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
    delete model;
    return 0;
}

// Methods defenitions

void line(
    int32_t x0, int32_t y0,
    int32_t x1, int32_t y1,
    TGAImage &image, TGAColor color
) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    // TODO(akumanory): maybe make a struct for this thing
    // because of too many variable, maybe can make compact
    // and more readable (in the other hand maybe not necessesary)
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int32_t d_error2 = std::abs(dy) * 2;
    int32_t error2 = 0;
    int32_t y = y0;

    for (int32_t x = x0; x <= x1 ; x++) {
        if (steep) {
            image.set(y, x, color);   
        } else {
            image.set(x, y, color);
        }
        error2 += d_error2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1); 
            error2 -= dx * 2;
        }
    }
}
