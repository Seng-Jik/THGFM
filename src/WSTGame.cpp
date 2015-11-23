#include "WSTGame.h"
#include "BulletMgr.h"
#include "CollWorld.h"
#include "EffectMgr.h"
#include "CSVWRollBgp.h"
#include "Tools.h"

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
    player[0].OnDraw();
    bulletMgr.OnDraw();
    stage.OnDraw();
    //m_p2.OnDraw();
    //collWorld.Debug_DrawEnemy();
    //collWorld.Debug_DrawEnemyBullet();
    //collWorld.Debug_DrawPlayer();
}

void WSTGame::OnNext()
{
    background.OnNext();
    player[0].OnNext();
    //m_p2.OnNext();
    stage.OnNext();
    bulletMgr.OnNext();
    effMgr.OnNext();
}

void WSTGame::OnEvent(int p, Key k, bool b)
{
    if(p == 0) player[0].OnEvent(k,b);
    //if(p == 1) m_p2.OnEvent(k,b);
}
