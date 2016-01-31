<<<<<<< HEAD
#ifndef SEMGR_H
#define SEMGR_H
#include "Snow.h"
enum SeName{
    DEMOSE,
};
class SeMgr
{
    public:
        static void Init();
        void Play(SeName);
        void Pause();
        void Resume();
    protected:
    private:
        static void loadSe(SeName,const std::string& se);
        static Mix_Chunk* m_se[20];
};
extern SeMgr se;

#endif // SEMGR_H
=======
#ifndef SEMGR_H
#define SEMGR_H
#include "Snow.h"
enum SeName{
    DEMOSE,
};
class SeMgr
{
    public:
        static void Init();
        void Play(SeName);
        void Pause();
        void Resume();
    protected:
    private:
        static void loadSe(SeName,const std::string& se);
        static Mix_Chunk* m_se[20];
};
extern SeMgr se;

#endif // SEMGR_H
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
