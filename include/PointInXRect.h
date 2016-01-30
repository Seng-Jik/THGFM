struct XRect{
	int x[4],y[4];
	//规定：点必须逆时针排列。
};
bool PointInXRect(const XRect& r,int x,int y);
