#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H


class PlayerBullet
{
    public:
        PlayerBullet();
        void OnDraw();
        void OnNext();
        void Start();
        void Stop();
        void SetY(int y);
        void SetMovePara(bool);

        ~PlayerBullet();
    protected:
    private:
        struct Bullet{
            int x,y,w;
        };
};

#endif // PLAYERBULLET_H
