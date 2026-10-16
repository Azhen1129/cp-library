template<typename T,typename M>
struct SparseTable {// data 的下标就是查询用的下标
	M merge;
	RaggedArray<T> st;
	vector<int> LOG2;
	SparseTable() {}
	explicit SparseTable(const vector<T>&data,M _merge=M{}):merge(_merge),LOG2((int)data.size()+1) {
		int N=(int)data.size();
		int K=0;
		rep(i,1,N+1) {
			if((2<<K)<=i) K++;
			LOG2[i]=K;
		}
		int tot=0;
		rep(k,0,K+1) tot+=N-(1<<k)+1;
		st.reserve(tot,K+1);
		st.push_row(span<const T>(data.data(),N));
		rep(k,1,K+1) {
			int half=1<<(k-1),len=N-(1<<k)+1;
			st.push_row();
			rep(i,0,len) st.push_back(merge(st[k-1,i],st[k-1,i+half]));
		}
	}
	T query(int l,int r) const {// [l, r]，l / r 是传入 vector 的下标
		int k=LOG2[r-l+1];
		return merge(st[k,l],st[k,r-(1<<k)+1]);
	}
	/*
	 	M 必须幂等：x op x == x
	 	可用：min / max / gcd / bit_and<> / bit_or<> / logical_and<> / logical_or<>
	 	不能用：bit_xor<>（x^x==0）、plus<> / multiplies<>（重叠区间算两遍）
	*/
};