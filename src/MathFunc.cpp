#include "MathFunc.h"
#include <math.h>
#include <mem.h>
#include <string.h>

using namespace ACGCross;
using namespace std;

float ACGCross::ArcFunc(float x)
{
    if (x >= 1.0) return 1;
    else return sqrt(1 - (x - 1) * (x - 1));
}

float ACGCross::FArcFunc(float x)
{
    if (x >= 1.0) return 0;
    else return ACGCross::ArcFunc(1 - x);
}

float ACGCross::SArcFunc(float x){
    return (x - 1) * (x - 1);
}
