template<typename T>
struct BIT {
	int N;
	int BM;
	vector<T> data;
	BIT(int _N):N(_N),BM(1),data(_N+1,0) { while(BM<=N) BM<<=1; BM>>=1; }
	void add(int idx,T val) { if(idx<=0) return; while(idx<=N) data[idx]+=val,idx+=idx&-idx; }
	T sum(int idx) { T res=0; while(idx>0) res+=data[idx],idx-=idx&-idx; return res; }
	T sum(int l,int r) { return (l>r)?0:sum(r)-sum(l-1); }
	int kth(T k) {
		int idx=0;
		for(int b=BM;b;b>>=1) {
			int n=idx+b;
			if(n<=N&&data[n]<k) k-=data[n],idx=n;
		}
		return idx+1;
	}
};