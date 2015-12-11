#include "Tools.h"
#include "CSVWRollBgp.h"
#include "StageMgr.h"
#include "StageTitle.h"
using namespace Snow;
using namespace std;
SDL_Texture* LoadPic(const std::string& f)
{
    ResFile r;
    r.Load(f);
    auto p = IMG_LoadTexture_RW(pRnd,r,r.Size());
    SDL_SetTextureBlendMode(p,SDL_BLENDMODE_BLEND);
    SDL_assert(p != nullptr);
    return p;
}

void LoadStage(const std::string& stageName, Level l)
{
    string path = "Stage/"+stageName+"/";
    ResVal stgConfig;
    stgConfig.Load(path + "stage.rv");

    //Load Background Data
    background.LoadCSV(stageName,stgConfig.Str("BGCSV"));
    background.SetSpeed(8.0);

    //Load Stage Data
    stage.Clear();
    switch(l){
    case LV_E:
        stage.LoadCSV(path+stgConfig.Str("ESTG"));break;
    case LV_N:
        stage.LoadCSV(path+stgConfig.Str("NSTG"));break;
    case LV_H:
        stage.LoadCSV(path+stgConfig.Str("HSTG"));break;
    case LV_L:
        stage.LoadCSV(path+stgConfig.Str("LSTG"));break;
    case LV_EX:
        stage.LoadCSV(path+stgConfig.Str("EXSTG"));break;
    }

    //Load Stage Title
    stageTitle.Set(stgConfig.Int("TITLE_BEG"),stgConfig.Int("TITLE_END"),path + stgConfig.Str("TITLE_IMG"));
}
