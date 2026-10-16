template<typename T>
vector<int> TopoOrder(const T&G) {// 0 - idx，如果返回值不要节点 0 ，将节点 0 自环
	int N=G.nrows();
	vector<int> indeg(N,0);
	rep(u,0,N) for(int v:G[u]) indeg[v]++;
	vector<int> order;
	rep(u,0,N) if(!indeg[u]) order.push_back(u);
	rep(i,0,order.size()) {
		for(int v:G[order[i]]) {
			indeg[v]--;
			if(!indeg[v]) order.push_back(v);
		}
	}
	return order;
}