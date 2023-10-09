#include "TileSet.h"

TileSet::TileSet(GameObject& associated, int tileWidth, int tileHeight, std::string file) : tileSet(associated, file.c_str()),
                                                                                            tileWidth(tileWidth),
                                                                                            tileHeight(tileHeight)
{
    if (tileSet.IsOpen()){
		columns = tileSet.GetWidth()/tileWidth;
        rows = tileSet.GetHeight()/tileHeight;
        tilesNumber = columns * rows;
	} else {
		std::cout << "Failed to load tileset: " << file.c_str() << std::endl;
		columns = 0; rows = 0;
	}

}

void TileSet::RenderTile(unsigned index, float x, float y){   
    if (0 <= index && index <= (unsigned int)(tilesNumber - 1)){   
            int x_ = tileWidth*(index%columns);
            int y_ = tileHeight*(index/columns);
            
            tileSet.SetClip(x_, y_, tileWidth, tileHeight);
            tileSet.Render((int)x, (int)y);
        }
    
}

int TileSet::GetTileWidth(){return tileWidth;}

int TileSet::GetTileHeight(){return tileHeight;}