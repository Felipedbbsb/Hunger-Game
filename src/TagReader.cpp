#include "TagReader.h"
#include "Text.h"
#include "Game.h"
#include "Camera.h"
#include "CameraFollower.h"


TagReader::TagReader(GameObject &associated,
                    Skill::StateProtected ExposedFeatures,
                    std::vector<Tag::Tags> tags, Rect reference, bool isReversed)
: Component::Component(associated),
ExposedFeatures(ExposedFeatures),
tags(tags),
reference(reference),
isReversed(isReversed) 
{   
    
}   
   
void TagReader::Start() {     


    int countTag = 0;

    if(ExposedFeatures != Skill::StateProtected::NOCHANGES){
        GameObject* tagReader = new GameObject();
        Sprite* tagReader_spr;

        if(ExposedFeatures == Skill::StateProtected::PROTECTED){
            tagReader_spr = new Sprite(*tagReader, Tag::GetTagMessageSprite(Tag::Tags::PROTECTED));
        } 
        else if(ExposedFeatures == Skill::StateProtected::EXPOSED){
            tagReader_spr = new Sprite(*tagReader, Tag::GetTagMessageSprite(Tag::Tags::EXPOSED));
        }
        
        tagReader->AddComponent((std::shared_ptr<Sprite>)tagReader_spr); 

        if(isReversed){
            tagReader->box.x = reference.x - tagReader->box.w;
        }
        else{
            tagReader->box.x = reference.x + reference.w;
        }
        
        tagReader->box.y = reference.y + reference.h - tagReader->box.h * (countTag + 1);


        CameraFollower *tagReader_cmfl = new CameraFollower(*tagReader);
        tagReader->AddComponent(std::shared_ptr<CameraFollower>(tagReader_cmfl));

        auto weak_obj = Game::GetInstance().GetCurrentState().AddObject(tagReader);
        readerTags.push_back(weak_obj);

        countTag++;
    }

    for (auto& tag : tags){
        GameObject* tagReader = new GameObject();
        Sprite* tagReader_spr = new Sprite(*tagReader, Tag::GetTagMessageSprite(tag));
        tagReader->AddComponent((std::shared_ptr<Sprite>)tagReader_spr); 

        if(isReversed){
            tagReader->box.x = reference.x - tagReader->box.w;
        }
        else{
            tagReader->box.x = reference.x + reference.w;
        }
        
        tagReader->box.y = reference.y + reference.h - tagReader->box.h * (countTag + 1);


        CameraFollower *tagReader_cmfl = new CameraFollower(*tagReader);
        tagReader->AddComponent(std::shared_ptr<CameraFollower>(tagReader_cmfl));

        auto weak_obj = Game::GetInstance().GetCurrentState().AddObject(tagReader);
        readerTags.push_back(weak_obj);

        countTag++;
    }
}  
 
TagReader::~TagReader(){ 
    for (auto& weak_tag : readerTags) {
        auto tagGameObject = weak_tag.lock();
        if (tagGameObject) {
            tagGameObject->RequestDelete();
        }
    }

    
} 

void TagReader::Update(float dt){  

}

void TagReader::Render(){
} 

bool TagReader::Is(std::string type){
    return (type == "TagReader");
}

