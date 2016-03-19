#include "Tools.h"
#include "CSVWRollBgp.h"
#include "StageMgr.h"
#include "ShaoNvQiDaoZhong.h"
#include "StageTitle.h"
#include "Snow/Debug.h"
#include "Beater.h"
#include "BgmMgr.h"
using namespace Snow;
using namespace std;

void SoundFinished(int channel){
    PNT(bgm.GetBGMLoop());
    if(channel == 0){
        PNT(bgm.GetBGMLoop());
        if(bgm.GetBGMLoop()) Mix_PlayChannel(0,bgm.GetBGMLoop(),-1);
    }
}

SDL_Texture* LoadPic(const std::string& f)
{
    ResFile r;
    r.Load(f);
    PNT("LoadPic:"<<f);
    auto p = IMG_LoadTexture_RW(pRnd,r,r.Size());
    SDL_assert(p != nullptr);
    SDL_SetTextureBlendMode(p,SDL_BLENDMODE_BLEND);
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
        stage.LoadCSV(path+stgConfig.Str("ESTG"),path);break;
    case LV_N:
        stage.LoadCSV(path+stgConfig.Str("NSTG"),path);break;
    case LV_H:
        stage.LoadCSV(path+stgConfig.Str("HSTG"),path);break;
    case LV_L:
        stage.LoadCSV(path+stgConfig.Str("LSTG"),path);break;
    case LV_EX:
        stage.LoadCSV(path+stgConfig.Str("EXSTG"),path);break;
    }

    //Load Stage Title
    stageTitle.Set(stgConfig.Int("TITLE_BEG"),stgConfig.Int("TITLE_END"),path + stgConfig.Str("TITLE_IMG"));

    //BGM
    //beater.LoadBeats(path + stgConfig.Str("BGM_BEAT"));
    bgm.LoadMusic(path + stgConfig.Str("BGM_FILE"));
    beater.SetBeater(stgConfig.Float("BGM_BPM"));
    ShaoNvQiDaoZhong::EndShaoNv();
}
