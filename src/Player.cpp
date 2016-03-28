#include "Player.h"
#include "Tools.h"
#include "CollWorld.h"
#include "PlayerBullet.h"
#include "ItemMgr.h"
#include "CharData.h"
#include "GameUI.h"
#include "BulletMgr.h"
#include "EffectMgr.h"
#include "StageMgr.h"
#include "SeMgr.h"

using namespace Snow;

SDL_Texture* Player::m_images [2][5];
Player player[1];

void Player::Init()
{
    m_images [1] [0] = LoadPic("Player/Marisa/1.png");
    m_images [1] [1] = LoadPic("Player/Marisa/2.png");
    m_images [1] [2] = LoadPic("Player/Marisa/3.png");
    m_images [1] [3] = LoadPic("Player/Marisa/4.png");
    m_images [1] [4] = LoadPic("Player/Marisa/5.png");
    m_images [0] [0] = LoadPic("Player/Reimu/1.png");
    m_images [0] [1] = LoadPic("Player/Reimu/2.png");
    m_images [0] [2] = LoadPic("Player/Reimu/3.png");
    m_images [0] [3] = LoadPic("Player/Reimu/4.png");
    m_images [0] [4] = LoadPic("Player/Reimu/5.png");
}

Player::Player()
{
    ResetScore();
}

Player::~Player()
{
    //dtor
}

void Player::SetPlayerImage(int n)
{
    m_playerImageNum = n;
    if(n == 1) m_charData = &marisa;
    else if(n == 0) m_charData = &reimu;
}

void Player::SetPlayer(int n){
    m_playerNum = n;
}

#include "Snow/Debug.h"
void Player::OnDraw()
{
    if(m_booming){
        m_charData->BoomOnDraw();
    }
    if(m_invin%2==0 && m_live != Player::DEAD)
        SDL_RenderCopy(pRnd,m_images[m_playerImageNum] [(m_cnt/7)%5],nullptr,&m_r);
    if(m_k[4]){
        SDL_Rect pdRec = {int(m_x)-3,int(m_y)-3,6,6};
        SDL_SetRenderDrawColor(pRnd,255,255,255,255);
        SDL_RenderFillRect(pRnd,&pdRec);
    }
}

void Player::OnNext()
{
    ++m_cnt;
    if(m_deathVS){
        m_deathVS--;
        if(!m_deathVS){
            itemMgr.AddItem(POWER,10,m_x,m_y,5);
            itemMgr.AddItem(SCORE,10,m_x,m_y,5);
            m_charData->InstallDeatAnimation(m_x,m_y);
            m_x = m_y = -200;
            m_live = DEAD;
            gameUI.OnPlayerBeKilled();
            if(gameUI.GetLife() == 1) itemMgr.AddItem(FULLPOWER,10,m_x,m_y,1);
            stage.KillEnemy(nullptr);
            //bulletMgr.Clear();
        }
    }
    double spd = 9;
    if(m_booming){
        m_booming = m_charData->BoomOnNext(this);
        if(!m_booming)
            collWorld.ClearBoom();
        collWorld.Update_Boom();
    }

    if(m_k[4]) spd = 4;
    if((m_k[0] && m_k[2]) ||
       (m_k[0] && m_k[3]) ||
       (m_k[1] && m_k[2]) ||
       (m_k[1] && m_k[3])
    ) spd /= 1.4142135623730950488016887242097;
    if(m_booming) spd *= 0.1;

    if(m_live==LIVING){
        //Player Move
        if(m_k[0]) m_y -= spd;
        if(m_k[1]) m_y += spd;
        if(m_k[2]) m_x -= spd;
        if(m_k[3]) m_x += spd;
        if(m_x<45) m_x = 45;
        else if(m_x >= WIDTH-45) m_x = WIDTH-45;

        if(m_y<45) m_y = 45;
        else if(m_y >= HEIGHT-45) m_y = HEIGHT-45;

        //Player Bullet
        if(m_k[5] && !m_booming){
            m_charData ->BulletInstaller(m_powerValue,m_cnt,m_x,m_y);
        }
    }

    if(m_live == Player::BIRTHING){
        m_x = 2*m_birthTimer++-50;
        m_y = HEIGHT/2;
        if(m_birthTimer==50) m_live = Player::LIVING;
    }

    if(m_invin<=0) collWorld.SetPlayer(m_playerNum,true,m_x-2,m_y-2);

    m_r.x = m_x - m_r.w/2;
    m_r.y = m_y - m_r.h/2;

    if(m_invin>0) --m_invin;

    /* Item Get Border */
    if(m_x >= 1000) itemMgr.GetAll(m_playerNum);
}

void Player::AddPower(double power)
{
    m_powerValue += power;
}


void Player::OnEvent(Key k, bool b)
{
    if(k == T_UP) m_k[0] = b;
    else if(k == T_DOWN) m_k[1] = b;
    else if(k == T_LEFT) m_k[2] = b;
    else if(k == T_RIGHT) m_k[3] = b;
    else if(k == T_SLOW) m_k[4] = b;
    else if(k == T_SHOT) m_k[5] = b;
    else if(k == T_BOOM && b && !m_booming && (m_live == LIVING || m_live == DEATHVS)){
        if(!gameUI.CanUseBomb()) return;
        gameUI.OnUseBomb();
        if(m_deathVS){
            m_live = Player::LIVING;
            m_deathVS = 0;
        }
        m_charData->BoomReset(this);
        m_booming = true;
        Invincible(390);
        itemMgr.GetAll(m_playerNum);
    }
    else if(k == T_DBG_INVIC){
        Invincible(999999999);
    }
}

void Player::Invincible(int frame)
{
    m_invin = frame;
    if(frame) collWorld.SetPlayer(m_playerNum,false);
    else collWorld.SetPlayer(m_playerNum,true);
}

void Player::Kill()
{
    if(m_live != LIVING) return;
    m_live = Player::DEATHVS;
    m_deathVS = 10;
    se.Play(PLDEAD00);
}

void Player::Birth()
{
    m_live = Player::BIRTHING;
    m_birthTimer=0;
    Invincible(180);
}

void Player::ClearKey(){
    for(int i = 0;i < 8;++i)
        m_k[i] = false;
}

std::string Player::GetCharName()
{
    return m_charData -> CharName();
}

