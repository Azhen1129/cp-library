template<typename T>
struct WeightedDSU {
	vector<int> root;
	vector<T> weight;
	WeightedDSU(int _N,T _zero=T{}):root(_N+1,-1),weight(_N+1,_zero) {}
	int leader(int u) {
		if(root[u]<0) return u;
		int v=leader(root[u]);
		weight[u]+=weight[root[u]];
		return root[u]=v;
	}
	T value(int u) { leader(u); return weight[u]; }
	T delta(int u,int v) { return value(v)-value(u); }
	bool merge(int u,int v,T w) {
		int lu=leader(u);
		int lv=leader(v);
		if(lu==lv) {
			return (weight[v]-weight[u]==w);
		}
		if(root[lu]>root[lv]) {// 大的做根
			swap(lu,lv);
			swap(u,v);
			w=-w;
		}
		weight[lv]=w+weight[u]-weight[v];
		root[lu]+=root[lv];
		root[lv]=lu;
		return true;
	}
	bool same(int u,int v) { return leader(u)==leader(v); }
};