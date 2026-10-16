struct SPF {
	int N;
	vector<int> spf;
	vector<int> primes;
	explicit SPF(int _N):N(_N) {
		spf.assign(N+1,0);
		primes.reserve(N/10+10);
		rep(i,2,N+1) {
			if(spf[i]==0) {
				spf[i]=i;
				primes.push_back(i);
			}
			for(int p:primes) {
				if(p>N/i) break;
				spf[p*i]=p;
				if(p==spf[i]) break;
			}
		}
	}
	int operator[](int x) const { return 0<=x&&x<=N?spf[x]:0; }// 查询最小质因子
	vector<pair<int,int>> factorize(int x) const {// 质因数分解
		vector<pair<int,int>> res;
		if(x<2||x>N) return res;
		while(x>1) {
			int p=spf[x];
			int cnt=0;
			while(x%p==0) {
				x/=p;
				cnt++;
			}
			res.emplace_back(p,cnt);
		}
		return res;
	}
};