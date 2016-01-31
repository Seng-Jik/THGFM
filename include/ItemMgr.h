<<<<<<< HEAD
#ifndef ITEMMGR_H
#define ITEMMGR_H
#include <queue>
#include "Snow.h"
enum ItemType{
    POWER = 0,
    SCORE = 1,
    LIFE1UP = 2,
    FULLPOWER = 3,
    BOMB = 4
};

class ItemMgr
{
    public:
        static void Init();
        void OnDraw();
        void OnNext();
        void AddItem(ItemType i,double boom,int x,int y,int num);  //类型,爆炸幅度,x,y,个数
        void Clear();
        void GetAll(int playerNum);
        void GetItem(int num,int playerNum);
        void Kill(int num);
    protected:
    private:
        struct Item{
            bool live = false;
            ItemType type;
            double x,y;
            enum{
                DOWNING,
                GETTING
            }state;
            int boom;
            float gettingSpd = 0;
            double ySpd = 0;
            double boomSpdX = 0;

            int gettingToPlayer;
        }m_items[750];
        int m_searchTop;
        std::priority_queue<int> m_freeList;

        static SDL_Texture* m_imgs [5];
};

extern ItemMgr itemMgr;

#endif // ITEMMGR_H
=======
#ifndef ITEMMGR_H
#define ITEMMGR_H
#include <queue>
#include "Snow.h"
enum ItemType{
    POWER = 0,
    SCORE = 1,
    LIFE1UP = 2,
    FULLPOWER = 3,
    BOMB = 4
};

class ItemMgr
{
    public:
        static void Init();
        void OnDraw();
        void OnNext();
        void AddItem(ItemType i,double boom,int x,int y,int num);  //类型,爆炸幅度,x,y,个数
        void Clear();
        void GetAll(int playerNum);
        void GetItem(int num,int playerNum);
        void Kill(int num);
    protected:
    private:
        struct Item{
            bool live = false;
            ItemType type;
            double x,y;
            enum{
                DOWNING,
                GETTING
            }state;
            int boom;
            float gettingSpd = 0;
            double ySpd = 0;
            double boomSpdX = 0;

            int gettingToPlayer;
        }m_items[750];
        int m_searchTop;
        std::priority_queue<int> m_freeList;

        static SDL_Texture* m_imgs [5];
};

extern ItemMgr itemMgr;

#endif // ITEMMGR_H
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
