template<typename E>
struct CSR {
	vector<E> data;
	vector<int> offsets;
	CSR():data(),offsets(1,0) {}
	CSR(const CSR&)=default;
	CSR&operator=(const CSR&)=default;
	CSR(CSR&&o) noexcept:data(move(o.data)),offsets(move(o.offsets)) {
		o.data.clear();
		o.offsets.assign(1,0);
	}
	CSR&operator=(CSR&&o) noexcept {
		if(this!=&o) {
			data=move(o.data);
			offsets=move(o.offsets);
			o.data.clear();
			o.offsets.assign(1,0);
		}
		return *this;
	}
	int nrows() const { return (int)offsets.size()-1; }// 顶点数
	int nvals() const { return (int)data.size(); }// 邻接项数（无向 = 2*边数）
	int deg(int u) const { return offsets[u+1]-offsets[u]; }// 出度
	span<const E> neighbors(int u) const { return span<const E>(data.data()+offsets[u],offsets[u+1]-offsets[u]); }
	span<const E> operator[](int u) const { return neighbors(u); }
	const E&operator[](int u,int k) const { return data[offsets[u]+k]; }
	bool empty() const { return offsets.size()==1; }
	bool operator==(const CSR&o) const = default;
	void reserve_data(int nv) { data.reserve(nv); }
	void reserve_rows(int nr) { offsets.reserve(nr+1); }
	void reserve(int nv,int nr) { reserve_data(nv); reserve_rows(nr); }
	void shrink_to_fit() { data.shrink_to_fit(); offsets.shrink_to_fit(); }
	void clear() { data.clear(); offsets.assign(1,0); }
	int push_row() { offsets.push_back((int)data.size()); return nrows()-1; }
	int push_row(span<const E> adj) { data.insert(data.end(),adj.begin(),adj.end()); offsets.push_back((int)data.size()); return nrows()-1; }
	int push_row(initializer_list<E> adj) { return push_row(span<const E>(adj.begin(),adj.size())); }
	void push_back(const E&x) { data.push_back(x); offsets.back()=(int)data.size(); }
	void push_back(E&&x) { data.push_back(move(x)); offsets.back()=(int)data.size(); }
	template<typename... A>
	E& emplace_back(A&&...args) { data.emplace_back(forward<A>(args)...); offsets.back()=(int)data.size(); return data.back(); }
	auto begin() { return data.begin(); }
	auto end() { return data.end(); }
	auto begin() const { return data.begin(); }
	auto end() const { return data.end(); }
	void swap(CSR&o) noexcept { data.swap(o.data); offsets.swap(o.offsets); }
	friend void swap(CSR&a,CSR&b) noexcept { a.swap(b); }
	/*---------- 构造 ----------*/
	static constexpr bool int_like=is_constructible_v<E,int>&&is_constructible_v<int,E>;
	static int head_of(const E&e) {
		if constexpr(int_like) return (int)e;
		else if constexpr(requires{(int)e.first;}) return (int)e.first;
		else { static_assert(!sizeof(E),"head_of：E 要能转 int，或者有可转 int 的 .first（pair 风格）"); return 0; }
	}
	static E rev_of(const E&e,int u) {
		if constexpr(int_like) return E(u);
		else if constexpr(requires{ e.second; E(u,e.second); }) return E(u,e.second);
		else { static_assert(!sizeof(E),"rev_of：E 要能由 int 构造，或者有 .second 且 E(int,.second) 可构造"); return e; }
	}
	template<bool Bidir>
	static CSR from_edges(int N,const vector<pair<int,E>>&edges) {// [0, N)
		static_assert(is_default_constructible_v<E>,"from_edges 需要 E 可默认构造");
		const int M=(int)edges.size();
		CSR res;
		res.offsets.assign(N+1,0);
		rep(i,0,M) {
			res.offsets[edges[i].first+1]++;
			if constexpr(Bidir) res.offsets[head_of(edges[i].second)+1]++;
		}
		rep(i,1,N+1) res.offsets[i]+=res.offsets[i-1];
		res.data.resize(res.offsets.back());// 双向时自然 = 2M，不再出现 2*M 这个乘法
		vector<int> cnt=res.offsets;
		rep(i,0,M) {
			const int u=edges[i].first;
			res.data[cnt[u]++]=edges[i].second;// u -> v
			if constexpr(Bidir) {
				const int v=head_of(edges[i].second);
				res.data[cnt[v]++]=rev_of(edges[i].second,u);// v -> u
			}
		}
		return res;
	}
	template<bool Bidir>
	static CSR from_parent(const vector<int>&P) {
		static_assert(is_constructible_v<E,int>&&is_default_constructible_v<E>,"from_parent/from_children 需要 E 能由 int 构造且能默认构造（res.data.resize 要它）；加权请用 from_edges");
		const int N=(int)P.size();
		CSR res;
		res.offsets.assign(N+1,0);
		rep(i,0,N) if(P[i]!=-1) {
			res.offsets[P[i]+1]++;
			if constexpr(Bidir) res.offsets[i+1]++;
		}
		rep(i,1,N+1) res.offsets[i]+=res.offsets[i-1];
		res.data.resize(res.offsets.back());
		vector<int> cnt=res.offsets;
		rep(i,0,N) if(P[i]!=-1) {
			res.data[cnt[P[i]]++]=E(i);
			if constexpr(Bidir) res.data[cnt[i]++]=E(P[i]);
		}
		return res;
	}
	template<bool Bidir>
	static CSR from_children(const vector<int>&L,const vector<int>&R) {
		static_assert(is_constructible_v<E,int>&&is_default_constructible_v<E>,"from_parent/from_children 需要 E 能由 int 构造且能默认构造（res.data.resize 要它）；加权请用 from_edges");
		const int N=(int)L.size();
		CSR res;
		res.offsets.assign(N+1,0);
		rep(i,0,N) {
			if(L[i]!=-1) { res.offsets[i+1]++; if constexpr(Bidir) res.offsets[L[i]+1]++; }
			if(R[i]!=-1) { res.offsets[i+1]++; if constexpr(Bidir) res.offsets[R[i]+1]++; }
		}
		rep(i,1,N+1) res.offsets[i]+=res.offsets[i-1];
		res.data.resize(res.offsets.back());
		vector<int> cnt=res.offsets;
		rep(i,0,N) {
			if(L[i]!=-1) { res.data[cnt[i]++]=E(L[i]); if constexpr(Bidir) res.data[cnt[L[i]]++]=E(i); }
			if(R[i]!=-1) { res.data[cnt[i]++]=E(R[i]); if constexpr(Bidir) res.data[cnt[R[i]]++]=E(i); }
		}
		return res;
	}
};
template<typename E,bool Bidir>
struct EdgeBag {
	int N;
	vector<pair<int,E>> e;
	explicit EdgeBag(int _N):N(_N) {}
	EdgeBag(int _N,int to_reserve):N(_N) { e.reserve(to_reserve); }
	void reserve(int M) { e.reserve(M); }
	void add(int u,E v) { e.push_back({u,v}); }// u -> v
	template<typename W> requires is_constructible_v<E,int,W>
	void add(int u,int v,W w) { e.push_back({u,E(v,w)}); }
	int size() const { return (int)e.size(); }
	CSR<E> build() const { return CSR<E>::template from_edges<Bidir>(N,e); }// [0, N)
};