#pragma once

#include <memory>
#include <iostream>
#include <math.h>
class Vec2{
    public:
        Vec2(float x, float y);
        Vec2();
        static Vec2 D2points(Vec2 begin, Vec2 end);
        float Hypotenuse();
        Vec2 GetRotated(float degrees);
        void Rotate(float degrees);
        Vec2 RotateVector(float degrees);
        void RotateAngle(float degrees);
        float Angle();
        float RotateDegree();
        static Vec2 FromAngle(float angleDegrees);
        float x, y;
        Vec2 operator+(const Vec2 &other) const;
        Vec2 operator-(const Vec2 &other) const;
        Vec2 operator*(const float &n) const;
        
    
};