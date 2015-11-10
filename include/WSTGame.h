#include "KeyMapAct.h"
#include "Player.h"
#include "StageMgr.h"

class WSTGame : public KeyMapAct
{
    public:
        WSTGame();
        void OnDraw();
        void OnNext();
        void OnEvent(int p,Key,bool);
        ~WSTGame();
    protected:
    private:
};
