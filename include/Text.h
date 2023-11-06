#ifndef TEXT_H
#define TEXT_H

#define INCLUDE_SDL
#define INCLUDE_SDL_TTF
#define BACKGROUND_COLOR {0, 255, 0, 0}
#define OUTLINE_COLOR {142, 11, 11, 0}
#define OUTLINE2_COLOR {68, 53, 53, 0}

#include <iostream>
#include <vector> 
#include <memory>
#include "SDL_include.h"
#include "Component.h"
#include "GameObject.h"
#include "Timer.h"

class Text : public Component {
public:
    enum TextStyle { SOLID, SHADED, BLENDED, OUTLINE, OUTLINE2 };

    Text(GameObject& associated,
         std::string fontFile,
         int fontSize,
         TextStyle style,
         std::string text,
         SDL_Color color,
         int blinkPeriod = 0);

    ~Text();
    
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void SetText(std::string text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontSize(int fontSize);

    void SetAlpha(Uint8 alpha);

private:
    struct TextLine {
        SDL_Texture* texture;
        int width;  // Largura da linha
        int height; // Altura da linha
    };

    void RemakeTexture();
    SDL_Texture* RenderSolidText(const std::string& text, SDL_Color color);
    SDL_Texture* RenderShadedText(const std::string& text, SDL_Color color);
    SDL_Texture* RenderBlendedText(const std::string& text, SDL_Color color);
    SDL_Texture* RenderTextWithOutline(const std::string& text, SDL_Color textColor, SDL_Color outlineColor);
    
    TTF_Font* font;
    SDL_Texture* texture;
    std::string text;
    TextStyle style;
    std::string fontFile;
    int fontSize;
    SDL_Color color;
    int blinkPeriod;
    Timer blinkTimer;
    std::vector<TextLine> lines;
};

#endif // TEXT_H
