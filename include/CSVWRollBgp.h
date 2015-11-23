#ifndef CSVWROLLBGP_H
#define CSVWROLLBGP_H
#include "Snow.h"
#include "Background.h"
#include <vector>
#include <queue>

class CSVWRollBgp : public Background
{
    private:
        struct SingleBG{
            SDL_Texture* tex;
            int w,h;
            double depth;double depthTrg;int depthLostTime = 0;
            double r1x,r2x;
            Uint8 alpha;double alphaTrg;int alphaLostTime = 0;
            int y;int yTrg;int yLostTime = 0;
        };
        std::vector<SingleBG> m_bgs;

        struct SingleCmd{
            char type;
            int begTime;
            int lenTime;
            int bgNum;
            double trgVal;

            inline bool operator < (const SingleCmd& s) const{
                return s.begTime > s.lenTime;
            }
        };
        std::priority_queue <SingleCmd> m_cmds;
        int m_cnt;

        double m_spd;double m_spdTrg;int m_spdLostTime = 0;
    public:
        void LoadCSV(const std::string& stage,const std::string& bgcsv);
        void OnNext();
        void OnDraw();
        void Clear();
        inline void SetSpeed(double spd){m_spd = spd;}
};

extern CSVWRollBgp background;

#endif // CSVWROLLBGP_H
