struct Combinatorics {
	int N;
	vector<Mint> fact,ifact;
	explicit Combinatorics(int _N):N(_N),fact(_N+1),ifact(_N+1) {// N < MOD
		fact[0]=1;
		rep(i,1,N+1) fact[i]=fact[i-1]*i;
		ifact[N]=fact[N].inv();
		rrep(i,N,0) ifact[i-1]=ifact[i]*i;
	}
	Mint binom(int n,int k) const { return k<0||k>n?(Mint)0:fact[n]*ifact[k]*ifact[n-k]; }// 组合数，n <= N
	Mint perm(int n,int k) const { return k<0||k>n?(Mint)0:fact[n]*ifact[n-k]; }// 排列数，n <= N
	Mint C(int n,int k) const { return binom(n,k); }// 组合数，n <= N
	Mint P(int n,int k) const { return perm(n,k); }// 排列数，n <= N
	Mint compositions(int n,int k) const { return n==0&&k==0?(Mint)1:binom(n-1,k-1); }// 有序拆分数，n - 1 <= N
	Mint weak_compositions(int n,int k) const { return n==0&&k==0?(Mint)1:binom(n+k-1,k-1); }// 弱有序拆分数，n + k - 1 <= N
	Mint catalan(int n) const { return binom(2*n,n)-binom(2*n,n+1); }// 卡特兰数，2 * n <= N
	Mint multinomial(const vector<int>&parts) const {// 多项式系数，p >= 0，sum(parts) <= N
		int n=0;
		for(int p:parts) n+=p;
		Mint res=fact[n];
		for(int p:parts) res*=ifact[p];
		return res;
	}
	/* 
	 * 组合数：从 n 个人中选 k 个人组队，先选后选不算不同
	 *     cb.binom(n, k)
	 * 
	 * 排列数：从 n 个人中选 k 个人组队，先选后选算不同
	 *     cb.perm(n, k)
	 * 
	 * 有序拆分数：n 颗相同的糖分给 k 个人，每人至少 1 颗
	 *     cb.compositions(n, k)
	 * 
	 * 弱有序拆分数：n 颗相同的糖分给 k 个人，但允许有人 0 颗
	 *     cb.weak_compositions(n, k)
	 * 
	 * 卡特兰数：3 对括号的合法匹配序列
	 *     cb.catalan(3)
	 * 
	 * 多项式系数：有重复元素的排列数，3 个 A、 2 个 B、 1 个 C
	 *     cb.multinomial({3, 2, 1})
	 */
};