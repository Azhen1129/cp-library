struct PrimeTable {
	int N;
	vector<bool> isc;
	vector<int> primes;
	explicit PrimeTable(int _N):N(_N) {
		isc.assign(N/2+1,false);
		if(N>=2) primes.push_back(2);
		primes.reserve(N/10+10);
		for(int i=3;i<=N;i+=2) {
			if(isc[i>>1]) continue;
			primes.push_back(i);
			if((ll)i*i<=N) for(ll j=(ll)i*i;j<=N;j+=2LL*i) isc[j>>1]=true;
		}
	}
	bool isprime(int x) const {
		if(x<2||x>N) return false;
		if(x==2) return true;
		return (x&1)&&!isc[x>>1];
	}
	bool operator[](int x) const { return isprime(x); }// 查询是否是素数
};