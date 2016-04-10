#include "MathFunc.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

using namespace ACGCross;
using namespace std;

float ACGCross::ArcFunc(float x)
{
    if (x>=1.0) return -1.0;
    else return sqrt(1-(x-1)*(x-1));
}

float ACGCross::FArcFunc(float x)
{
    if (x>=1.0) return -1.0;
    else return 1-ACGCross::ArcFunc(1-x);
}

float ACGCross::SArcFunc(float x){
	auto v = -x * x + 2 * x;
	if (v < 0) {
		return 0;
	}
    return sqrt(v);
}
