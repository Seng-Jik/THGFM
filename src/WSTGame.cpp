#include "WSTGame.h"
#include "BulletMgr.h"
#include "CollWorld.h"
#include "EffectMgr.h"
#include "CSVWRollBgp.h"
#include "Tools.h"
#include "PlayerBullet.h"
#include "Snow/Debug.h"

WSTGame::WSTGame()
{
    LoadStage("Demo2",LV_N);
    player[0].SetPlayerImage(1);
    //m_p2.SetPlayerImage(0);
    player[0].Birth();

}

WSTGame::~WSTGame()
{
    //dtor
}

void WSTGame::OnDraw()
{
    background.OnDraw();
    effMgr.OnDraw();
    playerBulletMgr.OnDraw();
    player[0].OnDraw();
    bulletMgr.OnDraw();
    stage.OnDraw();
    collWorld.Debug_DrawPlayerBullet();
    //m_p2.OnDraw();
    collWorld.Debug_DrawEnemy();
    //collWorld.Debug_DrawEnemyBullet();
    //collWorld.Debug_DrawPlayer();

}

void WSTGame::OnNext()
{
    background.OnNext();
    //PNT("BEGIN PLAYER BULLET");
    playerBulletMgr.OnNext();
    //PNT("END PLAYER BULLET");
    player[0].OnNext();
    //m_p2.OnNext();
    //PNT("BEGIN STAGE");
    stage.OnNext();
    //PNT("END STAGE");

    bulletMgr.OnNext();
    effMgr.OnNext();

    collWorld.Update_Player_Enemy();
    collWorld.Update_Player_EnemyBullet();
    collWorld.Update_Enemy_PlayerBullet();
}

void WSTGame::OnEvent(int p, Key k, bool b)
{
    if(p == 0) player[0].OnEvent(k,b);
    //if(p == 1) m_p2.OnEvent(k,b);
}
