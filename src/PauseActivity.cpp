#include "PauseActivity.h"
#include "WSTGame.h"
#include "Tools.h"
#include "Player.h"
PauseActivity * pause;

PauseActivity::PauseActivity()
{
    //init the value
    for(int i = 0; i < ALLCHOICE; ++i)
    {
        m_btns[i].func = BtnFunc(i);
        m_btns[i].enabled = false;
        m_btns[i].sp.Load("gameUI/pause_" + std::to_string(i) + ".png");
        m_btns[i].sp.SetRollEnable(true);
    }
    m_change_quick_finish = false;
    m_wait_frame_adddec = -1;
}

PauseActivity::~PauseActivity()
{
    for(int i = 0; i < ALLCHOICE; ++i)
        m_btns[i].sp.Clear();
}

void PauseActivity::AddBtns(BtnFunc i_btns[], int i_len)
{
    if(i_len == 0){
        std::cout << "WARNING: the number of buttons was not given" << std::endl;
        i_len = 6;
    }
    //absolutely safe
    for(int i = 0; i < ALLCHOICE; ++i)
        m_btns[i].enabled = false;

    for(int i = 0; i < i_len; ++i)
        for(int j = 0; j < ALLCHOICE; ++j)
            if(i_btns[i] == m_btns[j].func){
                m_btns[j].enabled = true;
                break;
            }
}

void PauseActivity::OnShow()
{
    SetLogicScreenSize(WIDTH,HEIGHT);
    //use the frames to calculate as time(0.8s)
    m_show_time = MENUSHOWTIME;
    m_hide_time = MENUHIDETIME;
    m_wait_time = MENUWAITTIME;

    SDL_Rect r = {0,0,Snow::pRnd.GetPhW(),Snow::pRnd.GetPhH()};
    Uint32* pixels = (new Uint32 [Snow::pRnd.GetPhW()*Snow::pRnd.GetPhH()]);
    SDL_RenderReadPixels(Snow::pRnd,
                     &r,
                     SDL_PIXELFORMAT_ARGB8888,
                     (void*)pixels,
                     Snow::pRnd.GetPhW()*4);
    m_bg.Load(SDL_CreateRGBSurfaceFrom((void*)pixels,Snow::pRnd.GetPhW(),Snow::pRnd.GetPhH(),32,Snow::pRnd.GetPhW()*4,
                                       0x00FF0000,0X0000FF00,0x000000FF,0XFF000000));
    m_bgt_o.Load(m_bg);
    m_bgt_o.SetPos(0,0);
    m_bgt_o.SetZoom(WIDTH,HEIGHT);
    Snow::FastBlurARGB8888(m_bg,40);
    m_bgt.Load(m_bg);
    m_bgt.SetPos(0,0);
    m_bgt.SetZoom(WIDTH,HEIGHT);
    m_bgt.SetAlpha(0);

    delete pixels;
    m_state = SHOWING;
    m_tmr.Reset();

    //display the pointer
    m_ptr.Load("gameUI/pause_ptr.png");
    m_ptr_state = 0;

   //display the socre banner
    int score = player[0].GetScore();
    m_score.Load("gameUI/current_score.png");
    for (m_score_num = 0; score > 0; ++m_score_num){
        m_score_pt[m_score_num] = score % 10;
        score = (score - m_score_pt[m_score_num]) / 10;
    }
    m_score_rect.w = 28;
    m_score_rect.h = 38;
    m_score_rect.y = 50;

    if(m_num_color == 1)
        for (int i = 0; i < 10; ++i)
            m_score_tex[i] = LoadPic("GameUI/Number/red/" + std::to_string(i) + ".PNG");
    else
        for (int i = 0; i < 10; ++i)
            m_score_tex[i] = LoadPic("GameUI/Number/black/" + std::to_string(i) + ".PNG");

    //Init submenu
    m_submenu = false;
    m_submenu_state = SHOWING;
    m_submenu_ask_sp[0].Load("GameUI/pause_sub_0.png");
    m_submenu_ask_sp[1].Load("GameUI/pause_sub_1.png");
    m_submenu_ptr_sp.Load("GameUI/pause_sub_bg.png");


    //TODO:只处理了0号玩家
    player[0].ClearKey();
    BtnFunc tmpBtn[5] = {RESUME, RESTART, SETTINGS, TOTITLE, EXIT};
    AddBtns(tmpBtn, 5);
}

void PauseActivity::OnDraw()
{
    m_bgt_o.OnDraw();
    m_bgt.OnDraw();
    for (int i = 0; i < ALLCHOICE; ++i)
        if(m_btns[i].enabled)
            m_btns[i].sp.OnDraw();

    for (int i = m_score_num - 1; i >= 0; --i) {
        m_score_rect.x = m_score_rect_x + 240 - 32 * i;
        SDL_RenderCopy(Snow::pRnd, m_score_tex[m_score_pt[i]], nullptr, &m_score_rect);
    }
    m_ptr.OnDraw();
    m_score.OnDraw();
    //sub menu
    if(m_submenu)
    {
        m_submenu_ask_sp[0].OnDraw();
        m_submenu_ask_sp[1].OnDraw();
        m_submenu_ptr_sp.OnDraw();
    }
}

void PauseActivity::OnNext()
{
    float k;
    int k_w, k_h, p_x, p_y;
    if (m_submenu)
    {
        m_btns[m_ptr_state].sp.GetPos(p_x, p_y);
        m_btns[m_ptr_state].sp.GetSize(k_w, k_h);
        switch (m_submenu_state)
        {
        case SHOWING:
            //get position
            k = ACGCross::ArcFunc((MENUSHOWTIME - m_show_time) / (float)MENUSHOWTIME);
            m_submenu_ask_sp[0].SetPos(CHOICEX + k_w + 20, int(p_y + 153 - 170 * k));
            m_submenu_ask_sp[1].SetPos(CHOICEX + k_w + 20, int(p_y + 193 - 170 * k));
            m_submenu_ptr = 0;
            m_submenu_ptr_sp.SetPos(CHOICEX + k_w + 20, int(HEIGHT * (1 - k) + p_y + k_h - 50));
            if(--m_show_time == 0)
                m_submenu_state = WAITING;
            break;
        default:
        case WAITING:
            /*m_wait_time += m_wait_frame_adddec;
            if (m_wait_time <= 0)
                m_wait_frame_adddec = 1;
            if (m_wait_time >= MENUWAITTIME - 1)
                m_wait_frame_adddec = -1;
            if (++m_cycle == 4)
                m_cycle = 0;*/
            break;
        case CHOOSING:
            m_submenu_ptr_sp.SetPos(CHOICEX + k_w + 20, p_y - 17 + 40 * m_submenu_ptr);
            m_submenu_state = WAITING;
            break;
        case FINISHED:

            break;
        case HIDING:
            k = ACGCross::ArcFunc((MENUHIDETIME - m_hide_time) / (float)MENUHIDETIME);
            m_submenu_ask_sp[0].SetPos(CHOICEX + k_w + 20, int(p_y + 100 * k * (6 - m_ptr_state)));
            m_submenu_ask_sp[1].SetPos(CHOICEX + k_w + 20, int(p_y + 70 + 100 * k * (6 - m_ptr_state)));
            m_submenu_ptr_sp.SetPos(CHOICEX + k_w + 20, int(p_y + 90 + 100 * k * (6 - m_ptr_state)));
            if (--m_hide_time <= 0)
                m_submenu = false;
            break;
        }
    }
    switch(m_state)
    {
    case SHOWING:
        //mathfunc animation
        k = ACGCross::ArcFunc((MENUSHOWTIME - m_show_time) / (float)MENUSHOWTIME);
        for(int i = 0; i < ALLCHOICE; ++i){
            m_btns[i].sp.SetAlpha(k * 255);
            m_btns[i].sp.SetPos(CHOICEX, int(HEIGHT - k * (HEIGHT - CHOICEY) + 56 * i));
        }
        m_bgt.SetAlpha(k * 255);
        m_ptr.SetPos(CHOICEX - 70, int(HEIGHT - k * (HEIGHT - CHOICEY + 16)));
        m_score_rect_x = int(WIDTH - k * (WIDTH - 280));
        m_score.SetPos(int(WIDTH - k * (WIDTH - 80)), 50);
        if(--m_show_time == 0)
            m_state = WAITING;
        break;

    default:
    case WAITING:
        //pointer animation
        k = ACGCross::SArcFunc(m_wait_time / (float)MENUWAITTIME);
        std::cout << " the current frame of hding is " << m_wait_time << "and k = " << k << std::endl;
        m_btns[m_ptr_state].sp.GetSize(k_w, k_h);
        for (int i = 0; i < ALLCHOICE; ++i)
            if(i != m_ptr_state){
                m_btns[i].sp.SetPos(CHOICEX,  CHOICEY + 56 * i);
                m_btns[i].sp.SetRollAngle(0);
            }
        m_btns[m_ptr_state].sp.SetAlpha(140 + 90 * k);
        m_ptr.SetPos(CHOICEX - 70, (HEIGHT - CHOICEY - 30) + m_ptr_state * 56);
        m_wait_time += m_wait_frame_adddec;
        if (m_wait_time <= 0)
            m_wait_frame_adddec = 1;
        if (m_wait_time >= MENUWAITTIME - 1)
            m_wait_frame_adddec = -1;
        if (++m_cycle == 4)
            m_cycle = 0;
        //std::cout << " the current frame of waiting is " << m_wait_time << std::endl;
        break;

    case CHOOSING:
        //set the pos of ptr
        k = ACGCross::ArcFunc((MENUCHANGETIME - m_change_time) / (float)MENUCHANGETIME);
            m_ptr.SetPos(CHOICEX - 70, int((HEIGHT - CHOICEY - 30) + m_ptr_state * (56 * k)));
        if (++m_change_time == MENUCHANGETIME || m_change_quick_finish) {
            m_ptr.SetPos(CHOICEX - 70, (HEIGHT - CHOICEY - 30) + m_ptr_state * 56);
            m_ptr_state = WAITING;
            m_change_quick_finish = false;
        }
        std::cout << "Choosing ok" << std::endl;
        break;

    case FINISHED:
        switch(m_ptr_state)
        {
        case 0:
            m_state = HIDING;
            m_tmr.Reset();
            break;

        default:
            break;
        }
        break;

    case HIDING:
        k = ACGCross::ArcFunc((MENUHIDETIME - m_hide_time) / (float)MENUHIDETIME);
        //std::cout << " the current frame of hding is " << m_hide_time << "and k = " << k << std::endl;
        for (int i = 0; i < ALLCHOICE; ++i)
            m_btns[i].sp.SetPos(CHOICEX, int(CHOICEY + k * (HEIGHT - CHOICEY) + 56 * i));
        for (int i = 0; i < 10; ++i)
            SDL_SetTextureAlphaMod(m_score_tex[i], 255 - k * 240);

        m_score_rect_x = int(k * (WIDTH - 280) + 480);
        m_score.SetPos(int(k * (WIDTH - 80) + 80), 50);
        m_score.SetAlpha(255 - k * 240);

        m_ptr.SetPos(CHOICEX - 70, int(CHOICEY - 10 + k * 400));
        m_bgt_o.SetAlpha(255 - k * 240);
        if(--m_hide_time <= 0){
            wstg -> OnResume();
            Snow::Return();
        }
        break;
    }
}

void PauseActivity::OnEvent(int p, Key k, bool b)
{
    if(m_submenu && b && m_submenu_state != HIDING){
        switch (k)
        {
        case T_BOOM : case T_ESC : case T_PAUSE :
            m_show_time = MENUSHOWTIME;
            m_wait_time = MENUWAITTIME;
            m_submenu_state = HIDING;
            break;

        case T_UP  :        case T_DOWN:
            m_submenu_ptr = !m_submenu_ptr;
            m_submenu_state = CHOOSING;
            break;

        case T_ENTER: case T_SHOT:

            if(m_submenu_ptr == 0)
            {
                m_hide_time = MENUHIDETIME;
                m_submenu_state = HIDING;
            }
        }
        return;
    }

    if(!m_submenu && b && m_state != SHOWING)
    {
        //PLAYSE();
        se.Play(DEMOSE);
        m_btns[m_ptr_state].sp.SetAlpha(255);
        switch(k){
        case T_BOOM : case T_ESC : case T_PAUSE :
            m_state = FINISHED;
            m_ptr_state = RESUME;
            break;

        case T_UP:
            if(m_change_time < MENUCHANGETIME)
                m_change_quick_finish = true;
            if(m_ptr_state == 0)
            {
                m_ptr_state = ALLCHOICE - 1;
                while(!m_btns[m_ptr_state].enabled)
                    m_ptr_state--;
            }
            else
            {
                m_ptr_state--;
                while(!m_btns[m_ptr_state].enabled)
                    m_ptr_state--;
            }
            m_state = WAITING;
            m_change_time = 0;
            std::cout << "Now m_ptr_state is " << m_ptr_state << std::endl;
            break;

        case T_DOWN:
            if(m_change_time < MENUCHANGETIME)
                m_change_quick_finish = true;
            if(m_ptr_state == ALLCHOICE - 1)
            {
                m_ptr_state = 0;
                while(!m_btns[m_ptr_state].enabled)
                    m_ptr_state++;
            }
            else
            {
                m_ptr_state++;
                while(!m_btns[m_ptr_state].enabled)
                    m_ptr_state++;
            }
            m_state = WAITING;
            m_change_time = 0;
            //std::cout << "Now m_ptr_state is " << m_ptr_state << std::endl;
            break;

        case T_ENTER:
        case T_SHOT:
            switch (m_ptr_state)
            {
            case 0:
                m_state = FINISHED;
                break;
            case 2:
            case 4:
            case 5:
                m_state = WAITING;
                m_submenu = true;
                m_submenu_state = SHOWING;
                break;
            default:
                break;
            }
            break;
        }
    }
}
