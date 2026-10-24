template<typename T,typename TM,typename L,typename LM,typename MP>
struct LazySegTree {
	T Tiden;
	L Liden;
	int N;
	TM Tmerge;
	LM Lmerge;
	MP mp;
	vector<T> data;
	vector<L> lazy;
	LazySegTree(T _Tiden,L _Liden,int _N,TM _Tmerge=TM{},LM _Lmerge=LM{},MP _mp=MP{}):
		Tiden(_Tiden),Liden(_Liden),N(_N),Tmerge(_Tmerge),Lmerge(_Lmerge),mp(_mp),
		data((_N<<2)+5,_Tiden),lazy((_N<<2)+5,_Liden) {
		if(N<=0) return;
		auto build=[&](auto&&self,int u,int l,int r) ->void {
			if(l==r) return;
			int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
			self(self,v1,l,mid); self(self,v2,mid+1,r);
			data[u]=Tmerge(data[v1],data[v2]);
		};
		build(build,1,1,N);
	}
	LazySegTree(T _Tiden,L _Liden,const vector<T>&_data,TM _Tmerge=TM{},LM _Lmerge=LM{},MP _mp=MP{}):
		Tiden(_Tiden),Liden(_Liden),N((int)_data.size()),Tmerge(_Tmerge),Lmerge(_Lmerge),mp(_mp),
		data((N<<2)+5,_Tiden),lazy((N<<2)+5,_Liden) {
		if(N<=0) return;
		auto build=[&](auto&&self,int u,int l,int r) ->void {
			if(l==r) { data[u]=_data[l-1]; return; }
			int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
			self(self,v1,l,mid); self(self,v2,mid+1,r);
			data[u]=Tmerge(data[v1],data[v2]);
		};
		build(build,1,1,N);
	}
	void all_apply(int u,const L&val) {
		data[u]=mp(data[u],val);
		lazy[u]=Lmerge(lazy[u],val);
	}
	void push_down(int u) {
		if(lazy[u]==Liden) return;
		all_apply(u*2,lazy[u]);
		all_apply(u*2+1,lazy[u]);
		lazy[u]=Liden;
	}
	void set(int u,int l,int r,int pos,T val) {
		if(l==r) {
			data[u]=val;
			lazy[u]=Liden;
			return;
		}
		int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
		push_down(u);
		if(pos<=mid) set(v1,l,mid,pos,val);
		else set(v2,mid+1,r,pos,val);
		data[u]=Tmerge(data[v1],data[v2]);
	}
	void set(int pos,T val) { set(1,1,N,pos+1,val); }
	void apply(int u,int l,int r,int ql,int qr,const L&val) {
		if(qr<l||r<ql) return;
		if(ql<=l&&r<=qr) {
			all_apply(u,val);
			return;
		}
		int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
		push_down(u);
		apply(v1,l,mid,ql,qr,val);
		apply(v2,mid+1,r,ql,qr,val);
		data[u]=Tmerge(data[v1],data[v2]);
	}
	void apply(int l,int r,const L&val) { if(l<r) apply(1,1,N,l+1,r,val); }// [l, r)
	T prod(int u,int l,int r,int ql,int qr) {
		if(qr<l||r<ql) return Tiden;
		if(ql<=l&&r<=qr) return data[u];
		int v1=u<<1,v2=u<<1|1,mid=(l+r)>>1;
		push_down(u);
		return Tmerge(prod(v1,l,mid,ql,qr),prod(v2,mid+1,r,ql,qr));
	}
	T prod(int l,int r) { return l>=r?Tiden:prod(1,1,N,l+1,r); }// [l, r)
	T top() { return data[1]; }
	T operator[](int pos) {
		pos++;
		int u=1,l=1,r=N;
		while(l<r) {
			push_down(u);
			int mid=(l+r)>>1;
			if(pos<=mid) r=mid,u=u<<1;
			else l=mid+1,u=u<<1|1;
		}
		return data[u];
	}
};
// https://github.com/Azhen1129/cp-library/blob/main/LazySegTree.cpp