#include "CSVWRollBgp.h"
#include "Tools.h"

using namespace std;
using namespace Snow;

CSVWRollBgp background;

#include <iostream>

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
        else if(path == "START") break;
        sbg.tex = LoadPic(stgPath + path);
        cout<<"Load "<<stgPath + path<<endl;

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
    }

    m_cnt = 0;
    m_spd = 1.0;

}

void CSVWRollBgp::OnNext()
{
    ++m_cnt;
    FOR_EACH(p,m_bgs.begin(),m_bgs.end()){
        SDL_SetTextureAlphaMod(p->tex,p->alpha);
        p->r1x -= (1-p->depth)*m_spd;
        p->r2x -= (1-p->depth)*m_spd;
        if(p->r1x+p->w <= WIDTH){
            p->r2x = p->r1x;
            p->r1x = p->w;
        }
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
