struct DSU {
	vector<int> root;
	DSU(int _N):root(_N+1,-1) {}
	int leader(int u) { return root[u]<0?u:root[u]=leader(root[u]); }
	bool merge(int u,int v) {
		int lu=leader(u);
		int lv=leader(v);
		if(lu==lv) return false;
		if(root[lu]>root[lv]) {// 大的集合做根
			swap(lu,lv);
		}
		root[lu]+=root[lv];
		root[lv]=lu;
		return true;
	}
	bool same(int u,int v) { return leader(u)==leader(v); }
};