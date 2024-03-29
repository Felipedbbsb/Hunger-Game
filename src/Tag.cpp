#include "Tag.h"
#include "Text.h"
#include "Game.h"
#include "Reader.h"
#include "Camera.h"

Tag::Tag(GameObject &associated, Tags tag, std::weak_ptr<GameObject> enemyRef, int quantity)
: Component::Component(associated),
enemyRef(enemyRef),
quantity(quantity), 
tag(tag),
readerTag(nullptr),
bigTag(nullptr), 
tagCountNumber(nullptr)
{    
    if(enemyRef.lock()){
        
        new Sprite(associated, GetTagSprite(tag));
        AcivateTag(GetTagSprite(tag));
        
    }
    else{
        associated.RequestDelete();
    } 
} 
 
void Tag::Start() {      
   tagCountRender();
}  
  
Tag::~Tag(){ 
    HideReader();

    if(bigTag != nullptr ){
        bigTag->RequestDelete();
    }

    if (tagCountNumber != nullptr){ 
        tagCountNumber->RequestDelete();
        tagCountNumber = nullptr;   
    }  
} 



void Tag::Update(float dt){  
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    if(auto ref = enemyRef.lock()){
        if(associated.box.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y - Camera::pos.y * Game::resizer)){
            ShowReader();
        } else {
            HideReader();
        }
        
    }
    else{
        associated.RequestDelete();
    }

    

    // If bigtag exists, shrink it size until its equal diminua sua escala
    if (bigTag != nullptr) {
        auto spriteComponent = bigTag->GetComponent("Sprite");
        auto bigTag_spr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
        

        bigTag_spr->SetScale(bigTag_spr->GetScale().x - IMPLOSION_VEL * dt, bigTag_spr->GetScale().y - IMPLOSION_VEL * dt);
        
        auto center = associated.box.GetCenter();
        bigTag->box.DefineCenter(Vec2(center));
 
        // When same size 
        if (bigTag_spr->GetScale().x <= 1) {
            bigTag->RequestDelete();
            bigTag = nullptr;
            
            
        } 
    }       
} 

std::string Tag::GetTagSprite(Tag::Tags tag){
    std::string spriteTag;
        switch (tag) {
            case Tag::Tags::RESILIENCE: spriteTag = TAG_RESILIENCE_SPRITE; break;
            case Tag::Tags::DODGE: spriteTag = TAG_DODGE_SPRITE; break;
            case Tag::Tags::PROVOKE: spriteTag = TAG_PROVOKE_SPRITE; break;
            case Tag::Tags::VULNERABLE: spriteTag = TAG_VULNERABLE_SPRITE; break;
            case Tag::Tags::WEAK: spriteTag = TAG_WEAK_SPRITE; break;
            case Tag::Tags::RAMPAGE: spriteTag = TAG_RAMPAGE_SPRITE; break;
            case Tag::Tags::PROTECTED: spriteTag = TAG_PROTECTED_SPRITE; break;
            case Tag::Tags::EXPOSED: spriteTag = TAG_EXPOSED_SPRITE; break;
            case Tag::Tags::CURSE: spriteTag = TAG_CURSE_SPRITE; break;
        }
    return spriteTag ;   
}
 
void Tag::AcivateTag(std::string sprite){
        if(bigTag == nullptr ){
            bigTag = new GameObject(associated.box.x, associated.box.y);
            Sprite* bigTag_spr = new Sprite(*bigTag, sprite);
            bigTag_spr->SetScale(BIGTAG_SCALE, BIGTAG_SCALE); //  two times bigger
            bigTag_spr->SetAlpha(BIGTAG_APLHA); // APLHA

            auto center = associated.box.GetCenter();
            bigTag->box.DefineCenter(Vec2(center));
            Game::GetInstance().GetCurrentState().AddObject(bigTag);
        }    
} 

void Tag::UpdateQuantity(int newQuantity) { 
    quantity = newQuantity; 
    tagCountRender();
}
 
Tag::Tags Tag::GetTag(){
    return tag;
}

void Tag::ShowReader(){
    if (!readerTag) {
        readerTag = new GameObject(associated.box.x, associated.box.y);
        new Reader(*readerTag, GetTagMessageSprite(tag));
        Game::GetInstance().GetCurrentState().AddObject(readerTag);
    }
}

void Tag::HideReader() {
    if (readerTag!= nullptr) {
        readerTag->RequestDelete();
        readerTag = nullptr;
    } 
}

std::string Tag::GetTagMessageSprite(Tags tag){
    std::string tagName;

    switch (tag) {
        case Tags::RESILIENCE:
            tagName = "assets/img/UI/tagReaders/tagReaderResilience.png";
            break;
        case Tags::DODGE:
            tagName = "assets/img/UI/tagReaders/tagReaderDodge.png";
            break;
        case Tags::PROVOKE:
            tagName = "assets/img/UI/tagReaders/tagReaderProvoke.png";
            break;
        case Tags::VULNERABLE:
            tagName = "assets/img/UI/tagReaders/tagReaderVulnerable.png";
            break;
        case Tags::WEAK:
            tagName = "assets/img/UI/tagReaders/tagReaderWeak.png";
            break;
        case Tags::RAMPAGE:
            tagName = "assets/img/UI/tagReaders/tagReaderRampage.png";
            break;
        case Tags::PROTECTED: 
            tagName = "assets/img/UI/tagReaders/tagReaderProtected.png";
            break;
        case Tags::EXPOSED: 
            tagName = "assets/img/UI/tagReaders/tagReaderExposed.png";
            break;
        case Tags::CURSE: 
            tagName = "assets/img/UI/tagReaders/tagReaderCurse.png";
            break;        
        default:
            tagName = "Unknown";
    }

    return tagName;
}

std::string Tag::GetTagMessage(Tags tag) {
    std::string tagName = GetTagName();
    std::string message = tagName + " (" + std::to_string(quantity) + " Turns) \n" + GetTagInfo();
    return message; 
}

std::string Tag::GetTagInfo() {
    std::string tagName;

    switch (tag) {
        case Tags::RESILIENCE:
            tagName = "Reduz o dano recebido em 50%";
            break;
        case Tags::DODGE:
            tagName = "50% de chance de evitar todo o dano do proximo golpe";
            break;
        case Tags::PROVOKE:
            tagName = "Força os inimigos a atacarem este alvo";
            break;
        case Tags::VULNERABLE:
            tagName = "Aumenta o dano recebido em 50%";
            break;
        case Tags::WEAK:
            tagName = "Reduz o seu dano em 25%";
            break;
        case Tags::RAMPAGE:
            tagName = "Aumenta seu dano em 25%";
            break;
        case Tags::PROTECTED: 
            tagName = "Nao pode ser alvejado";
            break;
        case Tags::EXPOSED: 
            tagName = "Pode ser alvejado";
            break;    
        case Tags::CURSE: 
            tagName = "Curse";
            break;        
        default:
            tagName = "Unknown";
    }

    return tagName;
}


std::string Tag::GetTagName() {
    std::string tagName;

    switch (tag) {
        case Tags::RESILIENCE:
            tagName = "Resilience";
            break;
        case Tags::DODGE:
            tagName = "Dodge";
            break;
        case Tags::PROVOKE:
            tagName = "Provoke";
            break;
        case Tags::VULNERABLE:
            tagName = "Vulnerable";
            break;
        case Tags::WEAK:
            tagName = "Weak";
            break;
        case Tags::RAMPAGE:
            tagName = "Rampage";
            break;
        case Tags::PROTECTED: 
            tagName = "Protected";
            break;
        case Tags::EXPOSED: 
            tagName = "Exposed";
            break;

        case Tags::CURSE: 
            tagName = "Curse";
            break;    
        default:
            tagName = "Unknown";
    }

    return tagName;
}

void Tag::tagCountRender() { 
    if (tagCountNumber != nullptr){ 
        tagCountNumber->RequestDelete();
        tagCountNumber = nullptr;     
    }  
    //Creates Reader for hp 
    //position middle of hp bar
    tagCountNumber =  new GameObject(associated.box.x + associated.box.w, associated.box.y + associated.box.h); //posicao foi no olho...
    std::string textNumber = std::to_string(quantity);
    new Text(*tagCountNumber, TEXT_TAGCOUNT_FONT, 
                              TEXT_TAGCOUNT_SIZE,
                              Text::OUTLINE2,
                              textNumber, 
                              TEXT_TAGCOUNT_FONT_COLOR, 
                              0);  
 
    tagCountNumber->box.x -= tagCountNumber->box.w;                                                 
    tagCountNumber->box.y -= tagCountNumber->box.h;     

    Game::GetInstance().GetCurrentState().AddObject(tagCountNumber);

}

void Tag::Render(){
} 

bool Tag::Is(std::string type){
    return (type == "Tag");
}

