#include "Camera.h"
#include "TileMap.h"

TileMap::TileMap(GameObject &associated, std::string file, TileSet *tileSet) : Component::Component(associated) {
    this->tileSet = tileSet;       
    this->parallax = 0;                                                                     
    TileMap::Load(file.c_str());
}

void TileMap::Load(std::string file){
    int tile;
    char empty;
    std::ifstream file_object;
    file_object.open(file.c_str());
    // Confere se foi aberto
    if (file_object) {
        file_object >> mapWidth >> empty >> mapHeight >> empty >> mapDepth >> empty;
        //std::cout << "TileMap: " << std::endl;
        //std::cout << "tileMap width: " << mapWidth << std::endl;  
        //std::cout << "tileMap height: " << mapHeight << std::endl; 
        //std::cout << "tileMap depth: " << mapDepth << std::endl;
    }
    else{
        std::cout << "Failed to open TileMap " << std::endl;
    }

    for (int i = 0; i < (mapWidth * mapHeight * mapDepth); i++){
        file_object >> tile >> empty;
        tileMatrix.push_back(tile-1);
    }
}

int& TileMap::At(int x, int y, int z){
    int index = x + (y * mapWidth) + (z * mapWidth * mapHeight);
    int& result = tileMatrix[index];
    return result;
}


void TileMap::SetTileSet(TileSet* tileSet){
    this->tileSet = tileSet;
}


void TileMap::RenderLayer(int layer, int cameraX, int cameraY){
    for(int x = 0; x < mapHeight; x++){
		for(int y = 0; y < mapWidth; y++){
			tileSet->RenderTile(At(x, y, layer),
                    (float)((x + cameraX * layer * this-> parallax) * tileSet->GetTileWidth()),
                    (float)((y + cameraY * layer * this-> parallax) * tileSet->GetTileHeight()));}
	}
}



int TileMap::GetWidth(){
    return mapWidth;
}

int TileMap::GetHeight(){
    return mapHeight;
}

int TileMap::GetDepth(){
    return mapDepth;
}

bool TileMap::Is(std::string type){
    if (type == "TileMap"){return true;}
    else{return false;}
}

void TileMap::Update(float dt) {}

void TileMap::Render(){
    for (int i = 0; i < mapDepth; i++)
    {
        // std::cout << "TileMap::Render: Indice da layer " << i << std::endl;
        RenderLayer(i, Camera::pos.x + (int)Camera::pos.x*parallax*i, Camera::pos.y + (int)Camera::pos.y*parallax*i);
    }
}

void TileMap::SetParallax(float parallax) {
	this->parallax = parallax;
}