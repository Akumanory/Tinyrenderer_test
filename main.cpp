#include <cstdint>
#include <cstdlib>
#include <tga_image.h>
#include <utility>

// Static variables

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor green = TGAColor(  0, 255,   0, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor blue  = TGAColor(  0,   0, 255, 255); // NOTE(akumanory): don't use when night lighte is acite, because disables all blue light from the screen

// Methods declarations 

void line(
    int32_t x0, int32_t y0,
    int32_t x1, int32_t y1,
    TGAImage &image, TGAColor color);

// Start point

int main()
{
	TGAImage image(100, 100, TGAImage::Format::RGB);
	// image.set(52, 41, green);
    line(0, 0, 99, 0, image, green); 
    line(99, 99, 0, 99, image, white);
    line(0, 0, 99, 99, image, red); 
    line(99, 0, 0, 99, image, blue);
	image.flip_vertically(); // NOTE(akumanory): i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

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
            image.set(x, y, color);   
        } else {
            image.set(y, x, color);
        }
        error2 += d_error2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1); 
            error2 -= dx * 2;
        }
    }
}
