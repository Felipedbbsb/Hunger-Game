#include "hpChangeEffect.h"
#include "Game.h"
#include "Camera.h"
#include "Reader.h"
#include "text.h"
#include <cmath>
#include <cstdlib>

hpChangeEffect::hpChangeEffect(GameObject& associated, std::string number, int lifebarRectY)
    : Component(associated), 
    animationDuration(2.0),
    maxHeightTime(animationDuration * 0.45),
    number(number),
    lifebarRectY(lifebarRectY),
    direction(rand() % 2 == 0 ? 1 : -1), // Randomly choose left or right (1 for right, -1 for left)
    concavity(1), // 1 for upward concavity
    maxXDisplacement(75),
    tMaxHeight(175),
    alpha(1)
{
}

hpChangeEffect::~hpChangeEffect() {
}

void hpChangeEffect::Start() { 
    Text* hpReader_behaviour = new Text(associated, TEXT_hpChangeEffect_FONT, 
                                        TEXT_HP_CHANGE_SIZE, 
                                        Text::SOLID,
                                        number, 
                                        TEXT_hpChangeEffect_FONT_COLOR,
                                        0); 
    associated.AddComponent(std::shared_ptr<Text>(hpReader_behaviour));       
    hpReader_behaviour->SetAlpha(alpha);   

    initialXPosition = associated.box.x;
    initialYPosition = associated.box.y;                                         
}

void hpChangeEffect::Update(float dt) { 
    // Atualize o tempo decorrido
    hpChangeEffectTimer.Update(dt);

    // Calcule a posição baseada na equação da parábola
    float t = hpChangeEffectTimer.Get() / animationDuration;
    float xDisplacement = direction * maxXDisplacement * t;

    // Defina uma aceleração vertical para aumentar a velocidade na queda
    float accelerationY = 400.0; // Ajuste esse valor conforme necessário

    // Calcule o deslocamento vertical com aceleração
    //float tMaxHeight = maxHeightTime / animationDuration;
    float yDisplacement = -concavity * (4 * tMaxHeight * t - 4 * tMaxHeight * t * t)
                        + accelerationY * t * t;

    // Atualize a posição da animação
    associated.box.x = initialXPosition + xDisplacement;
    associated.box.y = initialYPosition + yDisplacement;

    auto textComponent = associated.GetComponent("Text");
    auto textComponentPtr = std::dynamic_pointer_cast<Text>(textComponent);

    // Atualize o valor alpha para aumentar gradualmente, atingir 255 na altura máxima e diminuir depois
    if (t < 0.5) {
        alpha = t * 2 * 255.0;
    } else {
        if(t < 1.0){
          alpha = (1.0 - t)  * 255.0;
            if(alpha < 0){
                alpha = 1;
            }  
        }
        else{
            alpha = 1;
        }
    }


    // Se o tempo total de animação passou, solicite a exclusão da animação
    if (hpChangeEffectTimer.Get() >= animationDuration) {
        associated.RequestDelete();
    }

    // Atualize o valor alpha do texto
    if (textComponentPtr != nullptr) {
        textComponentPtr->SetAlpha(alpha);
    }
}

void hpChangeEffect::Render() {}

bool hpChangeEffect::Is(std::string type) {
    return (type == "hpChangeEffect");
}
