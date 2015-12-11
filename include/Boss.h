#ifndef BASEBOSS_H
#define BASEBOSS_H

#include <string>
class Boss
{
    public:
        void LoadRV(const std::string&);
        void OnBirth();
        void OnDraw();
        void OnNext();
        inline bool Live(){return true;};
        int birthTime;
    protected:
    private:
};

#endif // BASEBOSS_H
