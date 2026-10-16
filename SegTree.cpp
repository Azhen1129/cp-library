template<typename T,typename M=plus<T>>
struct SegTree {
	M merge;
	T iden;
	int N;
	vector<T> data;
	vector<int> ID;
	SegTree(T _iden,int _N,M _merge=M{}):
		merge(_merge),iden(_iden),N(_N),data((_N<<2)+5,_iden),ID(_N+1) {
		auto build=[&](auto&&self,int u,int l,int r) ->void {
			if(l==r) { ID[l]=u; return; }
			int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
			self(self,v1,l,mid); self(self,v2,mid+1,r);
		};
		build(build,1,1,N);
	}
	SegTree(T _iden,int _N,const vector<T>&_data,M _merge=M{}):
		merge(_merge),iden(_iden),N(_N),data((_N<<2)+5,_iden),ID(_N+1) {
		auto build=[&](auto&&self,int u,int l,int r) ->void {
			if(l==r) { data[u]=_data[l]; ID[l]=u; return; }
			int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
			self(self,v1,l,mid); self(self,v2,mid+1,r);
			data[u]=merge(data[v1],data[v2]);
		};
		build(build,1,1,N);
	}
	void merge_up(int u) { u>>=1; while(u) { data[u]=merge(data[u<<1],data[u<<1|1]); u>>=1; } }
	void set(int q,T val) { data[ID[q]]=move(val); merge_up(ID[q]); }
	void apply(int q,const T&val) {
		data[ID[q]]=merge(data[ID[q]],val);
		merge_up(ID[q]);
	}
	const T&operator[](int q) const { return data[ID[q]]; }
	T prod(int u,int l,int r,int ql,int qr) const {
		if(r<ql||l>qr) { return iden; }
		if(ql<=l&&r<=qr) { return data[u]; }
		int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
		return merge(prod(v1,l,mid,ql,qr),prod(v2,mid+1,r,ql,qr));
	}
	T prod(int ql,int qr) const { return ql>qr?iden:prod(1,1,N,ql,qr); }
	T top() const { return data[1]; }
	/*
		plus<>			+		0	string 是 ""
	 	multiplies<>	*		1
	 	bit_and<>		&		~0
	 	bit_or<>		|		0
	 	bit_xor<>		^		0
	 	logical_and<> 	&&		true
	 	logical_or<> 	||		false
	*/
};