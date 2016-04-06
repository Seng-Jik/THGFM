#include "CSVWRollBgp.h"
#include "Tools.h"
#include "Snow/Debug.h"

using namespace std;
using namespace Snow;

CSVWRollBgp background;

void CSVWRollBgp::LoadCSV(const std::string& stage,const std::string& bgcsv)
{
    Clear();
    string stgPath = "Stage/"+stage+"/";
    CSVReader csv;
    csv.LoadCSV(stgPath+bgcsv);


    SingleBG sbg;
    do{
        string path;
        csv.PopStr(path);
        if(path[0] == '#' || path.empty()) continue;
        else if(path == "START") {break;}
        sbg.tex = LoadPic(stgPath + path);

        csv.PopFloat(sbg.depth);

        int alp;
        csv.PopInt(alp);
        sbg.alpha = alp;

        csv.PopInt(sbg.y);

        SDL_QueryTexture(sbg.tex,nullptr,nullptr,&sbg.w,&sbg.h);
        sbg.r1x = 0;
        sbg.r2x = -sbg.w;

        m_bgs.push_back(sbg);

    }while(csv.NextLine());

    SingleCmd sc;
    while(csv.NextLine()){
        string type;
        csv.PopStr(type);
        if(type[0] == '#' || type.empty()) continue;
        sc.type = type[0];

        csv.PopInt(sc.begTime);
        csv.PopInt(sc.lenTime);
        csv.PopInt(sc.bgNum);
        csv.PopFloat(sc.trgVal);
        m_cmds.push(sc);
        //PNT("WRollBgp::LoadCSV:"<<"CMD "<<sc.type<<sc.begTime);
    }

    m_cnt = 0;
    m_spd = 1.0;

}

void CSVWRollBgp::OnNext()
{
    ++m_cnt;

    /* Speed Cmd Proc */
    if(m_spdLostTime > 0){
        m_spd += (m_spdTrg-m_spd) / m_spdLostTime;
        --m_spdLostTime;
    }

    FOR_EACH(p,m_bgs.begin(),m_bgs.end()){
        /* Alpha Cmd Proc */
        if(p -> alphaLostTime > 0){
            p -> alpha += (p -> alphaTrg-p -> alpha) / p -> alphaLostTime;
            --p -> alphaLostTime;
        }
        /* Depth Cmd Proc */
        if(p -> depthLostTime > 0){
            p -> depth += (p -> depthTrg-p -> depth) / p -> depthLostTime;
            --p -> depthLostTime;
        }

        /* Y Cmd Proc */
        if(p -> yLostTime > 0){
            p -> y += (p -> yTrg-p -> y) / p -> yLostTime;
            --p -> yLostTime;
        }

        /* Normal Proc */
        SDL_SetTextureAlphaMod(p->tex,p->alpha);
        p->r1x -= (1-p->depth)*m_spd;
        p->r2x -= (1-p->depth)*m_spd;
        if(p->r1x+p->w <= WIDTH){
            p->r2x = p->r1x;
            p->r1x = p->w+p->r2x;
        }
    }
	if(!m_cmds.size() != 0)
    while(m_cmds.front().begTime == m_cnt && !m_cmds.empty()){
        //PNT("WRollBgp:"<<m_cmds.front().type<<m_cnt);
        switch(m_cmds.front().type){
        case 'A':
            m_bgs[m_cmds.front().bgNum].alphaTrg = m_cmds.front().trgVal;
            m_bgs[m_cmds.front().bgNum].alphaLostTime = m_cmds.front().lenTime;
            break;
        case 'D':
            m_bgs[m_cmds.front().bgNum].depthTrg = m_cmds.front().trgVal;
            m_bgs[m_cmds.front().bgNum].depthLostTime = m_cmds.front().lenTime;
            break;
        case 'Y':
            m_bgs[m_cmds.front().bgNum].yTrg = m_cmds.front().trgVal;
            m_bgs[m_cmds.front().bgNum].yLostTime = m_cmds.front().lenTime;
            break;
        case 'S':
            m_spdTrg = m_cmds.front().trgVal;
            m_spdLostTime = m_cmds.front().lenTime;
            break;
        }
        m_cmds.pop();
    }
}

void CSVWRollBgp::OnDraw()
{
    FOR_EACH(p,m_bgs.begin(),m_bgs.end()){
        SDL_Rect dst = {
            (int)p->r1x,
            p->y,
            p->w,
            p->h
        };
        SDL_RenderCopy(pRnd,p->tex,nullptr,&dst);
        dst.x = p->r2x;
        SDL_RenderCopy(pRnd,p->tex,nullptr,&dst);
    }
}

void CSVWRollBgp::Clear()
{
    FOR_EACH(p,m_bgs.begin(),m_bgs.end())
        SDL_DestroyTexture(p->tex);
    while(!m_cmds.empty()) m_cmds.pop();
    m_bgs.clear();
}
