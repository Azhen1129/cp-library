namespace Dijkstra {
	template<typename Val,typename Edge,typename Better,typename Extend,typename Merge=nullptr_t,typename Settle=nullptr_t>
	vector<Val> Gen(const CSR<Edge>&G,int src,Val empty,Val worst,Better better,Extend extend,Merge merge=nullptr, Settle settle=nullptr) {
		constexpr bool HAS_MERGE=!is_same_v<Merge,nullptr_t>;
		constexpr bool HAS_SETTLE=!is_same_v<Settle,nullptr_t>;
		const int N=G.nrows();
		vector<Val> best(N,worst);
		vector<char> settled(N,false);
		using Item=pair<Val,int>;
		auto worse=[&](const Item&a,const Item&b) { return better(b.first,a.first); };
		priority_queue<Item,vector<Item>,decltype(worse)> pq(worse);
		best[src]=empty;
		pq.push({empty,src});
		while(!pq.empty()) {
			auto [cur,u]=pq.top(); pq.pop();
			if(settled[u]) continue;
			settled[u]=true;
			if constexpr(HAS_SETTLE) settle(u,best[u]);
			for(const Edge&e:G[u]) {
				const int v=CSR<Edge>::head_of(e);
				if(settled[v]) continue;
				Val cand=extend(best[u],e.second);
				/*
				E = int 的老实去 BFS ，别和 Dijkstra 沾边
				实在想要就用这个吧
				Val cand=worst;
				if constexpr(CSR<Edge>::int_like) cand=extend(best[u],1);
				else cand=extend(best[u],e.second);
				*/
				if(better(cand,best[v])) {
					best[v]=cand;
					pq.push({cand,v});
				}
				else if constexpr(HAS_MERGE) {
					if(!better(best[v],cand)) best[v]=merge(best[v],cand);
				}
			}
		}
		return best;
	}
}
using namespace Dijkstra;