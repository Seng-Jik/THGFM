#include "Snow.h"
#include <queue>
#include <stdio.h>
using namespace Snow;

Mix_Music* mus;

class BeatMaker:public Activity{
private:
    int m_fps = 0;
    bool m_white = false;
    bool m_white2 = false;
    std::queue<int> m_beats;
public:
    void OnShow(){
        Mix_PlayMusic(mus,0);
    }
    void OnDraw(){
        if(m_white){
            SDL_SetRenderDrawColor(pRnd,255,255,255,255);
            pRnd.Clear();
        }
    };
    void OnNext(){
        m_fps++;
        m_white = m_white2;
        m_white2 = false;
    };
    void OnEvent(const SDL_Event& e){
        if(e.type == SDL_KEYDOWN && ((e.key.keysym.sym == SDLK_z) || (e.key.keysym.sym == SDLK_x))){
            m_beats.push(m_fps);
            m_white2 = true;
        }
        else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e){
            FILE* f = fopen("beats.txt","w");
            while(!m_beats.empty()){
                fprintf(f,"%d\n",m_beats.front());
                m_beats.pop();
            }
            fclose(f);
            Exit(0);
        }
    };
};

class BeatDebugger:public Activity{
private:
    std::queue<int> m_beats;
    int m_end = 0;
    int m_cnt = 0;
public:
    void OnShow(){
        FILE* f = fopen("beats.txt","r");
        while(!feof(f)){
            int s;
            fscanf(f,"%d\n",&s);
            if(s > 0) m_beats.push(s);
        }
        fclose(f);
        Mix_PlayMusic(mus,0);
    }
    void OnDraw(){
        m_cnt++;
        if(m_cnt == m_beats.front()){
            m_beats.pop();
            SDL_SetRenderDrawColor(pRnd,255,255,255,255);
            pRnd.Clear();
        }
    }
};

int main(int argc,char** argv){
    Init();
    pRnd.Create("BeatMaker",false,800,600);
    mus = Mix_LoadMUS("beats.WAV");
    Run(new BeatMaker);
    return 0;
}
