/* Shared by most REL modules. Reference copy: mg9101 (fn_18_17030). */
#include "types.h"

void operator delete(void*);

struct Vec2 {
    f32 x, y;
    Vec2(f32 x_, f32 y_);
};

struct Vec3 {
    f32 x, y, z;
    Vec3(f32 x_, f32 y_, f32 z_);
};

struct Empty {
    ~Empty();
};

struct Color {
    u8 r, g, b, a;
    Color(u8 r_, u8 g_, u8 b_, u8 a_);
};

Vec2::Vec2(f32 x_, f32 y_) : x(x_), y(y_) {}

Vec3::Vec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}

Empty::~Empty() {}

Color::Color(u8 r_, u8 g_, u8 b_, u8 a_) : r(r_), g(g_), b(b_), a(a_) {}
