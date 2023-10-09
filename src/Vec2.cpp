#include "Vec2.h"
#include <math.h>

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2::Vec2() : Vec2(0, 0) {}

Vec2 Vec2::operator+(const Vec2 &other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2 &other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(const float &n) const{
    return Vec2(x * n, y * n);
}

void Vec2::Rotate(float value) {
    // Rotação horária.
    float xline, yline;
    xline = (x * cos(value)) - (y * sin(value));
    yline = (y * cos(value)) + (x * sin(value));
    x = xline;
    y = yline;
}


Vec2 Vec2::RotateVector(float degrees)
{
    // Recebe degrees em radianos
    float x_ = (x * cos(degrees)) - (y * sin(degrees));
    float y_ = (x * sin(degrees)) + (y * cos(degrees));

    x = x_;
    y = y_;
    return Vec2(x_, y_);
}

void Vec2::RotateAngle(float degrees) {
    Rotate(degrees / (180 / 3.14159265359));
}

Vec2 Vec2::GetRotated(float degrees) {
    Vec2 temp(x, y);
    temp.Rotate(degrees);
    return temp;
}

float Vec2::RotateDegree() {
    return atan2(y, x) * (180 / 3.14159265359);
}

float Vec2::Angle(){
    return atan2(y, x) ;
}

float Vec2::Hypotenuse() {
    return static_cast<float>(sqrt(pow(x, 2) + pow(y, 2)));
}

Vec2 Vec2::D2points(Vec2 a, Vec2 b) {
    float x_distance = b.x - a.x;
    float y_distance = b.y - a.y;

    return Vec2(x_distance, y_distance);
}


Vec2 Vec2::FromAngle(float angleDegrees) {
    return Vec2(cos(angleDegrees), sin(angleDegrees));
}