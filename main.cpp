#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <tga_image.h>
#include <tuple>
#include <utility>
#include <geometry.h>
#include <model.h>

// Defines
#define MODEL_DRAWING 0
#define LINES_DRAWING 0
#define TRIANGLES_DRAWING 1
#define LOGS

enum class Colors : int32_t
{
    White,
    Red,
    Green,
    Blue
};

using ColorMap = std::map<Colors, const TGAColor>;


namespace cfg {
// Screen size
const int32_t width     = 200;
const int32_t height    = 200;
} // namespace cfg

// Methods declarations
void line(
    int32_t x_0, int32_t y_0,
    int32_t x_1, int32_t y_1,
    TGAImage &image, const TGAColor &color);

void line(
    Vec2i point_0, Vec2i point_1,
    TGAImage &image, const TGAColor &color);

void trianle_line_sweep(
    Vec2i vertex_0,
    Vec2i vertex_1,
    Vec2i vertex_2,
    TGAImage &image, const ColorMap &color);

// Start point

int main()
{
    // setting colors data
    const TGAColor white(255, 255, 255, 255);
    const TGAColor green(  0, 255,   0, 255);
    const TGAColor red(  255,   0,   0, 255);
    const TGAColor blue(   0,   0, 255, 255);

    std::ignore = green;
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
         image, green); 
    line(screen_x, screen_x, 
         0, screen_y, 
         image, white);
    line(0, 0, 
         screen_y, screen_y, 
         image, red); 
    line(screen_x, 0, 
         0, screen_y, 
         image, blue);
#endif // LINES_DRAWING
#if TRIANGLES_DRAWING
    const std::array<Vec2i, 3> triangle_0 = {
        Vec2i(10, 70), 
        Vec2i(50, 160), 
        Vec2i(70, 80)
    };
    const std::array<Vec2i, 3> triangle_1 = {
        Vec2i(180, 50), 
        Vec2i(150, 1), 
        Vec2i(70, 180)
    };
    
    const std::array<Vec2i, 3> triangle_2 = {
        Vec2i(180, 150), 
        Vec2i(120, 160), 
        Vec2i(130, 180)
    };

    const ColorMap colors_map = {
        { Colors::White, white },
        { Colors::Red,   red   }, 
        { Colors::Green, green },
        { Colors::Blue,  blue  }
    };

    trianle_line_sweep(
        triangle_0[0],
        triangle_0[1],
        triangle_0[2],
        image,
        colors_map
    );
    // image.set(triangle_0[0].x, triangle_0[0].y, blue);
    // image.set(triangle_0[1].x, triangle_0[1].y, blue);
    // image.set(triangle_0[2].x, triangle_0[2].y, blue);

    trianle_line_sweep(
        triangle_1[0],
        triangle_1[1],
        triangle_1[2],
        image,
        colors_map   
    );
    trianle_line_sweep(
        triangle_2[0],
        triangle_2[1],
        triangle_2[2],
        image,
        colors_map   
    );
#endif // TRIANGLES_DRAWING

	image.flip_vertically(); // NOTE(akumanory): i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

    return 0;
}

// Methods defenitions
void trianle_line_sweep(
    Vec2i vertex_0, 
    Vec2i vertex_1, 
    Vec2i vertex_2,
    TGAImage &image, const ColorMap &color
) {
#ifdef LOGS
    std::cout << "Vertxes before sort by y" << "\n";
    std::cout << "vertex_0 x = " << vertex_0.x << " y = " << vertex_0.y << "\n";
    std::cout << "vertex_1 x = " << vertex_1.x << " y = " << vertex_1.y << "\n";
    std::cout << "vertex_2 x = " << vertex_2.x << " y = " << vertex_2.y << "\n";
#endif // LOGS

    // NOTE(akumanory): Not good triangles
    if (vertex_0.y == vertex_1.y &&
        vertex_0.y == vertex_2.y)
    {
        std::cout << "Bad triangles cords" << std::endl;
        return;
    }

    if (vertex_0.y > vertex_1.y) 
    {
        std::swap(vertex_0, vertex_1);
    }
    if (vertex_0.y > vertex_2.y) 
    {
        std::swap(vertex_0, vertex_2);
    }
    if (vertex_1.y > vertex_2.y) 
    {
        std::swap(vertex_1, vertex_2);
    }

#ifdef LOGS
    std::cout << "Vertxes after sort by y" << "\n";
    std::cout << "vertex_0 x = " << vertex_0.x << " y = " << vertex_0.y << "\n";
    std::cout << "vertex_1 x = " << vertex_1.x << " y = " << vertex_1.y << "\n";
    std::cout << "vertex_2 x = " << vertex_2.x << " y = " << vertex_2.y << "\n";
#endif // LOGS 

    int32_t total_height = vertex_2.y - vertex_0.y;
    for ( int32_t i = 0; i < total_height; i++ ) 
    {
        bool second_half = 
            i > vertex_1.y - vertex_0.y
            || vertex_1.y == vertex_0.y;

        int32_t segment_height = 
            second_half 
            ? vertex_2.y - vertex_1.y 
            : vertex_1.y - vertex_0.y;

        float alpha = 
            static_cast<float>( i )
            / static_cast<float>( total_height );

        float beta =  
            static_cast<float>( i - ( second_half ? vertex_1.y - vertex_0.y : 0 ) )
            / static_cast<float>( segment_height ); 

        Vec2i part_A = 
            vertex_0 + ( vertex_2 - vertex_0 ) * alpha;

        Vec2i part_B = 
            second_half 
            ? vertex_1 + ( vertex_2 - vertex_1 ) * beta 
            : vertex_0 + ( vertex_1 - vertex_0 ) * beta;

        if ( part_A.x > part_B.x ) 
        {
            std::swap( part_A, part_B );        
        }

        for (int32_t j = part_A.x; j <= part_B.x; j++) 
        {
            image.set( j, vertex_0.y + i, 
                      second_half 
                      ? color.at( Colors::Green ) 
                      : color.at( Colors::White ) );    
        }
    }



#ifdef OLD
    for (int y = vertex_0.y; y <= vertex_1.y; y++) {
        int segment_height = vertex_1.y - vertex_0.y + 1;
        float alpha = static_cast<float>(y - vertex_0.y) / static_cast<float>(total_height);
        float beta  = static_cast<float>(y - vertex_0.y) / static_cast<float>(segment_height);
        Vec2i part_A = vertex_0 + (vertex_2 - vertex_0) * alpha;
        Vec2i part_B = vertex_0 + (vertex_1 - vertex_0) * beta;
        if (part_A.x > part_B.x) {
            std::swap(part_A, part_B); 
        }
        // NOTE(akumanory): should use this instead of line drawing
        // because for this simple horizontal line it will be overkill
        for (int j = part_A.x; j <= part_B.x; j++) {
            image.set(j, y, color.at("white")); 
        }
        // NOTE(akumanory): debug info to show where is the ouline of the trinangle
        // image.set(part_A.x, y, color.at("red"));
        // image.set(part_B.x, y, color.at("green"));
    }
    for (int y = vertex_1.y; y <= vertex_2.y; y++) {
        int segment_height = vertex_2.y - vertex_1.y + 1;
        float alpha = static_cast<float>(y - vertex_0.y) / static_cast<float>(total_height);
        float beta  = static_cast<float>(y - vertex_1.y) / static_cast<float>(segment_height);
        Vec2i part_A = vertex_0 + (vertex_2 - vertex_0) * alpha;
        Vec2i part_B = vertex_1 + (vertex_2 - vertex_1) * beta;
        if (part_A.x > part_B.x) {
            std::swap(part_A, part_B);
        }
        // NOTE(akumanory): should use this instead of line drawing
        // because for this simple horizontal line it will be overkill
        for (int j = part_A.x; j <= part_B.x; j++) {
            image.set(j, y, color.at("white")); 
        }
        // NOTE(akumanory): debug info to show where is the ouline of the trinangle
        // image.set(part_A.x, y, color.at("red"));
        // image.set(part_B.x, y, color.at("green"));
    }
#endif // OLD 

    // line(vertex_0, vertex_1, image, color.at("green"));
    // line(vertex_1, vertex_2, image, color.at("white"));
    // line(vertex_2, vertex_0, image, color.at("red"));

}

void line(
    Vec2i point_0, Vec2i point_1,
    TGAImage &image, const TGAColor &color
) {
    line(point_0.x, point_0.y,
         point_1.x, point_1.y,
         image, color);
}

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
