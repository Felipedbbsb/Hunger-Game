#pragma once
#include <iostream>
#include <memory>

#include "Vec2.h"

class Rect {
    public:
      Rect();
      Rect(float x, float y, float w, float h);
      bool Contains(float x, float y);
      float x = 0, y = 0, w = 0, h = 0;
      void DefineCenter(float x, float y);
      void DefineCenter(Vec2 box);
      Vec2 GetCenter();

};           