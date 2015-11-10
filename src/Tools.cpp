#include "Tools.h"
using namespace Snow;
using namespace std;
SDL_Texture* LoadPic(const std::string& f)
{
    ResFile r;
    r.Load(f);
    return IMG_LoadTexture_RW(pRnd,r,r.Size());
}
