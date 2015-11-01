#include "Snow.h"
#include "KeyMapAct.h"
#include <iostream>
using namespace std;
using namespace Snow;

class test:public KeyMapAct{
public:
    void OnDraw(){};
    void OnEvent(int p,Key k,bool t){
        cout<<p<<" "<<k<<" "<<t<<endl;
    }
};

int main(int argc,char** argv){
    Init();
    pRnd.Create("东方谷丰梦",false,800,600);
    Run(new test);
}
