#include <math.h>
#include <iostream>
using namespace std;
namespace MathLineCalc{

//直线方程
struct LineXF{
	float k,b;
};

//给两个点，求直线方程
inline LineXF GetLineXF(float x1,float y1,float x2,float y2){
	LineXF xf;
	if(x1==x2) {
		xf.k=10000;
		xf.b = y1 - xf.k * x1;
	}
	else{
		xf.k = (y1-y2)/(x1-x2);
    	if(y1==y2) xf.k=0.000001;
    	xf.b = y1 - xf.k * x1;
	}
	return xf;
}

//给X求Y
inline float GetY(const LineXF& xf,float x){
		return xf.k*x+xf.b;
}

//给Y求X
inline float GetX(const LineXF& xf,float y){
	return (y-xf.b)/xf.k;
}

//直线上的点是否在线段范围内
inline bool PointOnLimited(float x,float y,float x1,float y1,float x2,float y2){
	return (x>=x1&&x<=x2&&y>=y1&&y<=y2)||(x<=x1&&x>=x2&&y<=y1&&y<=y2);
}

//点到线段距离
inline float PointToLimitedLine(float x, float y, float x1, float y1,
                                        float x2, float y2){
    float dx,dy,a,b,t,tx,ty;
    float distance;
    dx = (x2 - x1); dy = (y2 - y1);
    a = dx*dx + dy*dy;
    b = dx * (x1 - x) + dy * (y1 - y);
    t = -b / a;
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    tx = x1 + dx * t;
    ty = y1 + dy * t;
    distance = sqrt((x - tx)*(x - tx) + (y - ty)*(y - ty));
    return distance;
}

}

#include "PointInXRect.h"
using namespace MathLineCalc;

bool PointInXRect(const XRect& r,int x,int y){
    LineXF pxf[2],nxf[2];
    for(int i=0;i<2;++i){
    	pxf[i]= GetLineXF(r.x[2*i],r.y[2*i],r.x[(2*i+1)%4],r.y[(2*i+1)%4]);
    	nxf[i]= GetLineXF(r.x[(2*i+1)%4],r.y[(2*i+1)%4],r.x[(2*i+2)%4],r.y[(2*i+2)%4]);
    }

    cout<<"------ DEBUG"<<endl;
    for(int i=0;i<2;++i)
        cout<<"y="<<pxf[i].k<<"x+"<<pxf[i].b<<endl;
    for(int i=0;i<2;++i)
        cout<<"y="<<nxf[i].k<<"x+"<<nxf[i].b<<endl;
    cout<<"------ END"<<endl;

    float pxfy[2],nxfy[2];
    for(int i=0;i<2;++i)
    	pxfy[i] = GetY(pxf[i],x);
    for(int i=0;i<2;++i)
    	nxfy[i] = GetY(nxf[i],x);
    bool pIn =
        (y>=pxfy[0]&&y<=pxfy[1])||
        (y<=pxfy[0]&&y>=pxfy[1]);
    bool nIn =
        (y>=nxfy[0]&&y<=nxfy[1])||
        (y<=nxfy[0]&&y>=nxfy[1]);
	return pIn&&nIn;

}

/*
int main(){

	while(1){
		//总测试单元
	    cout<<"------ 输入判定点"<<endl;
    	float px,py;
    	XRect xr;
    	cin>>px;
    	cin>>py;
    	cout<<"-- 逆时针输入判定框"<<endl;
    	for(int i = 0;i<4;++i){
    		cout<<"- 第"<<i<<"个点"<<endl;
    		cin>>xr.x[i];
    		cin>>xr.y[i];
    	}
    	cout<<"判定结果："<<PointInXRect(xr,px,py)<<endl;


    	//求直线方程测试单元
    	cout<<"------"<<endl;
    	float x1,y1,x2,y2;
    	cin>>x1;
    	cin>>y1;
    	cin>>x2;
    	cin>>y2;
    	LineXF x = GetLineXF(x1,y1,x2,y2);
    	cout<<"y="<<x.k<<"x+"<<x.b<<endl;*/

    	//点在线段测试
    	/*float px,py;
    	cout<<"---- 输入点："<<endl;
    	cin>>px;
    	cin>>py;
    	cout<<"---- 输入线"<<endl;
    	float x1,y1,x2,y2;
    	cin>>x1;
    	cin>>y1;
    	cin>>x2;
    	cin>>y2;
    	cout<<"----结果："<<PointOnLimited(px,py,x1,y1,x2,y2)<<endl;

	}



	return 0;
}*/
