template<typename V>
V ClosestVectorToSegment(const V&P,const V&A,const V&B) {
	auto u=A-P;
	auto v=A-B;
	if(v*v==0) return u;
	auto t=(u*v)/(v*v);
	if(t<0) t=0;
	if(t>1) t=1;
	return u-t*v;// 点 P 到线段 AB
}