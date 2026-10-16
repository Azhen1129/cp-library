struct PhiMu {
	int N;
	vector<int> phi;
	vector<int> mu;
	vector<int> primes;
	explicit PhiMu(int _N):N(_N) {
		phi.assign(N+1,0);
		mu.assign(N+1,0);
		if(N>=1) {
			phi[1]=1;
			mu[1]=1;
		}
		primes.reserve(N/10+10);
		rep(i,2,N+1) {
			if(phi[i]==0) {
				primes.push_back(i);
				phi[i]=i-1;
				mu[i]=-1;
			}
			for(int p:primes) {
				if(p>N/i) break;
				int j=p*i;
				if(i%p==0) {
					phi[j]=phi[i]*p;
					mu[j]=0;
					break;
				}
				else {
					phi[j]=phi[i]*(p-1);
					mu[j]=-mu[i];
				}
			}
		}
	}
	int getPhi(int x) const { return 0<=x&&x<=N?phi[x]:0; }// [1, x] 与 x 互质的数的个数 
	int getMu(int x) const { return 0<=x&&x<=N?mu[x]:0; }// 容斥系数
};