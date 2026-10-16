namespace SCC {
	template<typename E>
	int EdgeDest(const E&e) {// E 里哪个是终点
		if constexpr(std::is_constructible_v<E,int>&&std::is_convertible_v<E,int>) return (int)e;
		else return (int)e.first;
	}
	template<typename E>
	pair<int,vector<int>> SccIds(const CSR<E>&G) {
		int N=G.nrows();
		int now_ord=0,K=0;
		vector<int> stk,low(N),ord(N,-1),ids(N);
		stk.reserve(N);
		auto dfs=[&](auto&&self,int u) ->void {
			low[u]=ord[u]=now_ord++;
			stk.push_back(u);
			for(const E&e:G.neighbors(u)) {
				int v=EdgeDest(e);
				if(ord[v]==-1) {
					self(self,v);
					low[u]=min(low[u],low[v]);
				}
				else {
					low[u]=min(low[u],ord[v]);
				}
			}
			if(low[u]==ord[u]) {
				while(true) {
					int x=stk.back();
					stk.pop_back();
					ord[x]=N;
					ids[x]=K;
					if(x==u) break;
				}
				K++;
			}
		};
		rep(i,0,N) if(ord[i]==-1) dfs(dfs,i);
		rep(i,0,N) ids[i]=K-1-ids[i];
		return {K,ids};
	}
	template<typename E>
	RaggedArray<int> Scc(const CSR<E>&G) {
		auto [K,ids]=SccIds(G);
		int N=G.nrows();
		RaggedArray<int> res(K);
		rep(i,0,N) res.offsets[ids[i]+1]++;
		rep(i,1,K+1) res.offsets[i]+=res.offsets[i-1];
		res.data.resize(N);
		vector<int> cnt=res.offsets;
		rep(i,0,N) res.data[cnt[ids[i]]++]=i;
		return res;
	}
}
using namespace SCC;