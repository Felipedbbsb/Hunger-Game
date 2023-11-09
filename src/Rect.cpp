#include "Rect.h"
#include "Game.h"

Rect::Rect(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h) {}

Rect::Rect()
    : x(0), y(0), w(0), h(0) {}

bool Rect::Contains(float x, float y) {
    return (x >= this->x * Game::resizer) && (x <= (this->x + this->w) * Game::resizer) &&
           (y >= this->y * Game::resizer) && (y <= (this->y + this->h) * Game::resizer);
}

void Rect::DefineCenter(float x, float y) {
    this->x = x - (this->w / 2);
    this->y = y - (this->h / 2);
}

void Rect::DefineCenter(Vec2 box) {
    DefineCenter(box.x, box.y);
}

Vec2 Rect::GetCenter() {
    return Vec2(this->x + (this->w / 2), this->y + (this->h / 2));
}
