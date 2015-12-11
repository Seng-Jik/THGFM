#include "Boss.h"
#include "Snow/Debug.h"
void Boss::LoadRV(const std::string& s)
{
    PNT("Boss::LoadRV:"<<s);
}

void Boss::OnBirth()
{
    PNT("Boss::OnBirth");
}

void Boss::OnDraw()
{
    PNT("Boss::OnDraw");
}

void Boss::OnNext()
{
    PNT("Boss::OnNext");
}
