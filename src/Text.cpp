#include "Text.h"
#include "Game.h"
#include "Camera.h"
#include "Resources.h"  
#include <sstream> 
 
Text::Text(GameObject& associated, std::string fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, int blinkPeriod)
    : Component(associated),
      font(nullptr),
      texture(nullptr),
      text(text),
      style(style), 
      fontFile(fontFile),
      fontSize(fontSize),
      color(color),
      blinkPeriod(blinkPeriod) {
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    RemakeTexture();
}

Text::~Text() {
    if (texture != nullptr) { 
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float dt) {
    if (blinkTimer.Get() < blinkPeriod) {
        blinkTimer.Update(dt);
    } 
    else {
        blinkTimer.Restart();
    }
}

void Text::Render() {
    if (blinkTimer.Get() <= blinkPeriod / 2) {
        int yOffset = 0; // Controla o deslocamento vertical entre as linhas
        for (const auto& line : lines) {
            SDL_Rect clipRect = {0, 0, line.width, line.height};
            SDL_Rect dst = {static_cast<int>(associated.box.x + Camera::pos.x),
                            static_cast<int>(associated.box.y + Camera::pos.y + yOffset),
                            line.width,
                            line.height};

            int RENDER_FAIL;
            RENDER_FAIL = SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), line.texture, &clipRect, &dst, associated.angleDeg, nullptr, SDL_FLIP_NONE);
            if (RENDER_FAIL != 0) {
                std::cout << "Text: fail to render " << SDL_GetError() << std::endl;
            }
            yOffset += line.height; // Ajusta o deslocamento vertical para a próxima linha
        }
    }
}

void Text::SetText(std::string text) {
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
    this->style = style;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::RemakeTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    font = Resources::GetFont(fontFile, fontSize);

    // Divida o texto em várias linhas com base no tamanho máximo permitido
    lines.clear();
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line, '\n')) {
        TextLine textLine;
        switch (style) {
            case SOLID: {
                textLine.texture = RenderSolidText(line, color);
                break;
            }
            case SHADED: {
                textLine.texture = RenderShadedText(line, color);
                break; 
            }
            case BLENDED: {
                textLine.texture = RenderBlendedText(line, color);  
                break; 
            }
            case OUTLINE: {
                textLine.texture = RenderTextWithOutline(line, color, OUTLINE_COLOR);
                break;
            }
            case OUTLINE2: {
                textLine.texture = RenderTextWithOutline(line, color, OUTLINE2_COLOR);
                break;
            }
            case OUTLINE3: {
                textLine.texture = RenderTextWithOutline(line, color, OUTLINE3_COLOR);
                break;
            }
        }
        SDL_QueryTexture(textLine.texture, nullptr, nullptr, &textLine.width, &textLine.height);
        lines.push_back(textLine);
    }

    // Calcule a altura total das linhas renderizadas e defina a altura da caixa associada
    int totalHeight = 0;
    int maxWidth = 0; // Largura máxima entre as linhas
    for (const auto& line : lines) {
        totalHeight += line.height;
        if (line.width > maxWidth) {
            maxWidth = line.width;
        }
    }
    associated.box.h = totalHeight;
    associated.box.w = maxWidth;
}

SDL_Texture* Text::RenderTextWithOutline(const std::string& text, SDL_Color textColor, SDL_Color outlineColor) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);

    // Defina o tamanho da borda (outline) para 5
    TTF_SetFontOutline(font, 2);
    SDL_Surface* outlineSurface = TTF_RenderText_Blended(font, text.c_str(), outlineColor);
 
    if (textSurface == nullptr || outlineSurface == nullptr) {
        return nullptr; 
    }

    // Combine text and outline surfaces
    SDL_Rect rect = {1, 1, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, nullptr, outlineSurface, &rect);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), outlineSurface);
    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(outlineSurface);

    // Restaure o tamanho da borda para 0 após a renderização
    TTF_SetFontOutline(font, 0);

    return texture;
}


SDL_Texture* Text::RenderSolidText(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* Text::RenderShadedText(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Shaded(font, text.c_str(), color, BACKGROUND_COLOR);
    if (surface == nullptr) {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* Text::RenderBlendedText(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (surface == nullptr) {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Text::SetAlpha(Uint8 alpha) {
    this->color.a = alpha; // updates alpha

    // Atualiza a textura do texto com a nova cor
    RemakeTexture();
}

bool Text::Is(std::string type) {
    return (type == "Text");
}
