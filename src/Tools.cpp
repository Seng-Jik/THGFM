#include "Tools.h"
using namespace Snow;
using namespace std;
SDL_Texture* LoadPic(const std::string& f)
{
    ResFile r;
    r.Load(f);
    auto p = IMG_LoadTexture_RW(pRnd,r,r.Size());
    SDL_SetTextureBlendMode(p,SDL_BLENDMODE_BLEND);
    return p;
}
