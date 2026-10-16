template<typename V>
V ClosestVectorToLine(const V&P,const V&A,const V&B) {
	auto u=A-P;
	auto v=A-B;
	if(v*v==0) return u;
	auto t=(u*v)/(v*v);
	return u-t*v;// 点 P 到直线 AB
}