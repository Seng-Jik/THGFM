#include "WSTGame.h"
#include "BulletMgr.h"
#include "CollWorld.h"
#include "EffectMgr.h"
#include "CSVWRollBgp.h"
#include "Tools.h"
#include "PlayerBullet.h"
#include "Snow/Debug.h"
#include "Beater.h"
#include "BgmMgr.h"
#include "PauseActivity.h"
#include "ItemMgr.h"
#include "GameUI.h"
WSTGame* wstg;

WSTGame::WSTGame()
{

    player[0].SetPlayerImage(0);
    //m_p2.SetPlayerImage(0);
    player[0].Birth();
    gameUI.SetLifeAndBomb(3,3);

}

WSTGame::~WSTGame()
{
    //dtor
}

void WSTGame::OnShow(){
    itemMgr.Clear();
    collWorld.ClearEnemy();
    collWorld.ClearEnemyBullet();
    bgm.Play();
}
double angle = 0;
void WSTGame::OnDraw()
{
    SDL_SetRenderTarget(Snow::pRnd,m_pGameGraphic);
    background.OnDraw();
    Boss* boss;
    if((boss = stage.GetBoss())!=nullptr) boss->OnSCBGDraw();
    effMgr.OnDraw();
    playerBulletMgr.OnDraw();
    itemMgr.OnDraw();
    player[0].OnDraw();
    bulletMgr.OnDraw();
    stage.OnDraw();

    SDL_SetRenderTarget(Snow::pRnd,nullptr);
    SDL_Point poi = {WIDTH/2,HEIGHT/2};
    //SDL_SetTextureColorMod(m_pGameGraphic,0,255,0);

    //SDL_SetTextureColorMod(m_pGameGraphic,255,0,0);
    SDL_Rect r = {0,0,WIDTH,HEIGHT};
    if(m_shkTmr){
        --m_shkTmr;
        int shk = m_shkTmr/10;
        switch(m_shkTmr%4){
        case 0:
            r.x-=shk;r.y+=shk;break;
        case 1:
            r.x+=shk;r.y-=shk;break;
        case 2:
            r.x+=shk;r.y+=shk;break;
        case 3:
            r.x-=shk;r.y-=shk;break;
        }
    }
    SDL_RenderCopyEx(Snow::pRnd,m_pGameGraphic,&r,nullptr,angle,&poi,SDL_FLIP_NONE);

    //SDL_SetRenderDrawColor(Snow::pRnd,0,0,0,255*m_blackMask);
    //SDL_SetRenderDrawBlendMode(Snow::pRnd,SDL_BLENDMODE_BLEND);
    //SDL_RenderFillRect(Snow::pRnd,nullptr);

    //collWorld.Debug_DrawEnemy_XRect();
    //angle+=0.1;
    //collWorld.Debug_DrawPlayerBullet();
    //m_p2.OnDraw();
    collWorld.Debug_DrawEnemy();
    //collWorld.Debug_DrawEnemyBullet();
    //collWorld.Debug_DrawPlayer();

    /* Beater Sync Debug */
    /*if(beater.IsBeatFrame()){
        SDL_SetRenderDrawColor(Snow::pRnd,255,0,0,255);
        Snow::pRnd.Clear();
    }*/
    gameUI.OnDraw();
}

void WSTGame::OnNext()
{
    beater.OnNext();
    background.OnNext();
    //PNT("BEGIN PLAYER BULLET");
    playerBulletMgr.OnNext();
    //PNT("END PLAYER BULLET");
    player[0].OnNext();
    //m_p2.OnNext();
    //PNT("BEGIN STAGE");
    stage.OnNext();
    itemMgr.OnNext();
    //PNT("END STAGE");

    bulletMgr.OnNext();
    effMgr.OnNext();

    collWorld.Update_Player_Enemy();
    collWorld.Update_Player_EnemyBullet();
    collWorld.Update_Enemy_PlayerBullet();
    gameUI.OnNext();

    if(m_dbg_doubleSpeed)
    for(int i = 0;i < 2;++i)
    {
        beater.OnNext();
        background.OnNext();
        //PNT("BEGIN PLAYER BULLET");
        playerBulletMgr.OnNext();
        //PNT("END PLAYER BULLET");
        player[0].OnNext();
        //m_p2.OnNext();
        //PNT("BEGIN STAGE");
        stage.OnNext();
        itemMgr.OnNext();
        //PNT("END STAGE");

        bulletMgr.OnNext();
        effMgr.OnNext();

        collWorld.Update_Player_Enemy();
        collWorld.Update_Player_EnemyBullet();
        collWorld.Update_Enemy_PlayerBullet();
        gameUI.OnNext();
    }
}

void WSTGame::OnEvent(int p, Key k, bool b)
{
    if(p == 0) player[0].OnEvent(k,b);
    //if(p == 1) m_p2.OnEvent(k,b);
    if((k == T_ESC || k == T_PAUSE || k == T_ENTER)&&b){
        Pause();
    }
    if(k == T_DBG_DOUBLE_SPEED)
        m_dbg_doubleSpeed = b;
}

void WSTGame::Pause()
{
    bgm.Pause();
    Snow::Call(pause);
}

void WSTGame::OnResume(){
    bgm.Resume();
}

void WSTGame::OnInit(){
    m_pGameGraphic = SDL_CreateTexture(Snow::pRnd,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_TARGET,WIDTH,HEIGHT);
}

void WSTGame::GrpShake(int frame)
{
    m_shkTmr = frame;
}

void WSTGame::GrpColorShake(int ms)
{

}
