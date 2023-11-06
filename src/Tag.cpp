#include "Tag.h"
#include "Text.h"
#include "Game.h"
#include "Reader.h"

Tag::Tag(GameObject &associated, Tags tag, std::weak_ptr<GameObject> enemyRef, int quantity)
: Component::Component(associated),
enemyRef(enemyRef),
quantity(quantity), 
tag(tag),
readerTag(nullptr),
bigTag(nullptr),
tagCountNumber(nullptr)
{   
    if (enemyRef.lock()){
        
        Sprite* tag_spr = new Sprite(associated, GetTagSprite(tag));
        associated.AddComponent(std::shared_ptr<Sprite>(tag_spr));
        //tag_spr->SetScale(0.8, 0.8);
        AcivateTag(GetTagSprite(tag));
        
    }
    else{
        associated.RequestDelete();
    } 
} 
 
void Tag::Start() {     
   
}  
  
Tag::~Tag(){ 
    HideReader();

    if(bigTag != nullptr ){
        bigTag->RequestDelete();
    }
} 



void Tag::Update(float dt){  
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    if(auto ref = enemyRef.lock()){
        if(associated.box.Contains(mousePos.x, mousePos.y)){
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
        }
    return spriteTag ;   
}
 
void Tag::AcivateTag(std::string sprite){
        if(bigTag == nullptr ){
            bigTag = new GameObject(associated.box.x, associated.box.y);
            Sprite* bigTag_spr = new Sprite(*bigTag, sprite);
            bigTag->AddComponent(std::shared_ptr<Sprite>(bigTag_spr)); 
            bigTag_spr->SetScale(BIGTAG_SCALE, BIGTAG_SCALE); //  two times bigger
            bigTag_spr->SetAlpha(BIGTAG_APLHA); // APLHA

            auto center = associated.box.GetCenter();
            bigTag->box.DefineCenter(Vec2(center));
            Game::GetInstance().GetCurrentState().AddObject(bigTag);
        }    
} 

void Tag::UpdateQuantity(int newQuantity) { 
    quantity = newQuantity; 
}
 
Tag::Tags Tag::GetTag(){
    return tag;
}

void Tag::ShowReader(){
    if (!readerTag) {
        readerTag = new GameObject(associated.box.x, associated.box.y);
        Reader* readerTag_behaviour = new Reader(*readerTag, GetTagMessage());
        readerTag->AddComponent(std::shared_ptr<Reader>(readerTag_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(readerTag);
    }
}

void Tag::HideReader() {
    if (readerTag!= nullptr) {
        readerTag->RequestDelete();
        readerTag = nullptr;
    } 
}


std::string Tag::GetTagMessage() {
    std::string tagName = GetTagName();
    std::string message = tagName + " (" + std::to_string(quantity) + " Turns) \n" + GetTagInfo();
    return message; 
}

std::string Tag::GetTagInfo() {
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
    tagCountNumber =  new GameObject(); //posicao foi no olho...
    std::string textNumber = std::to_string(quantity);
    Text* tagCountNumber_behaviour = new Text(*tagCountNumber, TEXT_TAGCOUNT_FONT, 
                                                      TEXT_TAGCOUNT_SIZE,
                                                      Text::BLENDED,
                                                      textNumber, 
                                                      TEXT_TAGCOUNT_FONT_COLOR,
                                                      0);  
 
    //tagCountNumber->box.x += (lifeBarRect.w - hpReader->box.w)/2;                                                
    //tagCountNumber->box.y += (lifeBarRect.h - hpReader->box.h)/2 - 1;     

    tagCountNumber->AddComponent(std::shared_ptr<Text>(tagCountNumber_behaviour));
    Game::GetInstance().GetCurrentState().AddObject(tagCountNumber);

}

void Tag::Render(){
} 

bool Tag::Is(std::string type){
    return (type == "Tag");
}

