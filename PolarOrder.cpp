int quad[3][3]={{6,5,4},{7,0,3},{8,1,2}};
template<typename T>
struct PolarOrder {
	bool operator()(const Vector2D<T>&a,const Vector2D<T>&b) const {
		int pa=a.pos(),pb=b.pos();
		if(pa!=pb) return pa<pb;
		return (a^b)>0;
	}
};