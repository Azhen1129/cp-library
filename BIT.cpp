template<typename T>
struct BIT {
	int N;
	int BM;
	vector<T> data;
	BIT(int _N):N(_N),BM(1),data(_N+1,0) { while(BM<=N) BM<<=1; BM>>=1; }
	BIT(const vector<T>&_data):BIT((int)_data.size()) {
		rep(i,0,N) data[i+1]=_data[i];
		rep(i,1,N+1) { int j=i+(i&-i); if(j<=N) data[j]+=data[i]; }
	}
	void add(int idx,T val) { if(idx<0) return; idx++; while(idx<=N) data[idx]+=val,idx+=idx&-idx; }
	T sum(int idx) { T res=0; for(;idx>0;idx-=idx&-idx) res+=data[idx]; return res; }// [0, idx)
	T sum(int l,int r) { return (l>=r)?0:sum(r)-sum(l); }// [l, r)
	int kth(T k) {// 第 k 个(k 从 1 开始)的 0-based 下标；k 超过总和时返回 N
		int idx=0;
		for(int b=BM;b;b>>=1) {
			int n=idx+b;
			if(n<=N&&data[n]<k) k-=data[n],idx=n;
		}
		return idx;
	}
};
// https://github.com/Azhen1129/cp-library/blob/main/BIT.cpp