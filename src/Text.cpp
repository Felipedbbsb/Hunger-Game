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
    ProcessTextWithTags(); // Process the tags when defining the text
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
        int yOffset = 0; // Control vertical offset between lines
        for (size_t i = 0; i < lines.size(); ++i) {
            SDL_Rect clipRect = {0, 0, lines[i].width, lines[i].height};
            SDL_Rect dst = {static_cast<int>(associated.box.x + Camera::pos.x),
                            static_cast<int>(associated.box.y + Camera::pos.y + yOffset),
                            lines[i].width,
                            lines[i].height};

            int RENDER_FAIL;
            RENDER_FAIL = SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), lines[i].texture, &clipRect, &dst, associated.angleDeg, nullptr, SDL_FLIP_NONE);
            if (RENDER_FAIL != 0) {
                std::cout << "Text: fail to render " << SDL_GetError() << std::endl;
            }
            yOffset += lines[i].height; // Adjust vertical offset for the next line
        }
    }
}

void Text::SetText(std::string text) {
    this->text = text;
    ProcessTextWithTags(); // Process the tags when defining the text
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

    if (font == nullptr) {
        font = Resources::GetFont(fontFile, fontSize);
    }

    // Split the text into multiple lines based on the maximum allowed width
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
        }
        SDL_QueryTexture(textLine.texture, nullptr, nullptr, &textLine.width, &textLine.height);
        lines.push_back(textLine);
    }

    // Calculate the total height of the rendered lines and set the associated box height
    int totalHeight = 0;
    int maxWidth = 0; // Maximum width among the lines
    for (const auto& line : lines) {
        totalHeight += line.height;
        if (line.width > maxWidth) {
            maxWidth = line.width;
        }
    }
    associated.box.h = totalHeight;
    associated.box.w = maxWidth;
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

bool Text::Is(std::string type) {
    return (type == "Text");
}

void Text::ProcessTextWithTags() {
    std::string processedText = text;
    std::string startTag = "<color=";
    std::string endTag = "</color>";
    std::size_t startPos = 0;

    // Vetor para armazenar cores processadas
    std::vector<SDL_Color> processedColors;

    while ((startPos = processedText.find(startTag, startPos)) != std::string::npos) {
        std::size_t endPos = processedText.find(endTag, startPos);
        if (endPos != std::string::npos) {
            std::string tagContent = processedText.substr(startPos + startTag.length(), endPos - (startPos + startTag.length()));

            // Parse the color from the tag content
            SDL_Color tagColor = ParseColorTag(tagContent);

            // Adicione a cor processada ao vetor
            processedColors.push_back(tagColor);

            // Move a posição passada para a próxima tag
            startPos = endPos + endTag.length();
        } else {
            // Handle error or log a message indicating that there's a mismatched tag
            std::cerr << "Error: Mismatched color tags in the text." << std::endl;
            break;
        }
    }

    // Use as cores processadas ao renderizar o texto
    for (size_t i = 0; i < lines.size() && i < processedColors.size(); ++i) {
        SDL_SetTextureColorMod(lines[i].texture, processedColors[i].r, processedColors[i].g, processedColors[i].b);
    }

    // Set the processed text
    text = processedText;
}



SDL_Color Text::ParseColorTag(const std::string& tagContent) {
    SDL_Color parsedColor = color; // Inicializa com a cor atual

    // Verifique se a tagContent começa com "#" e possui 6 caracteres para representar a cor hexadecimal
    if (tagContent.size() == 6 && tagContent[0] == '#') {
        // Extrai os valores RGB da tagContent
        std::istringstream stream(tagContent.substr(1)); // Ignora o "#" inicial
        unsigned int r, g, b;
        stream >> std::hex >> r >> g >> b;

        // Se a extração for bem-sucedida, configure os componentes de cor
        if (!stream.fail()) {
            parsedColor.r = r;
            parsedColor.g = g;
            parsedColor.b = b;
        }
    }

    return parsedColor;
}



