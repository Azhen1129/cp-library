struct PollardRho {
	ull pollard(ull n) {// 返回 n 的一个非平凡因子
		for(ull p:{2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL}) {
			if(n%p==0) return p;
		}
		Mont mt(n);
		while(true) {
			ull c=mt.mul(rnd.rull(1,n-1),mt.r2);
			ull x=mt.mul(rnd.rull(1,n-1),mt.r2),y=x,d=1;
			auto f=[&](ull v) {
				ull t=mt.mul(v,v)+c;
				return t<c||t>=n?t-n:t;
			};
			while(d==1) {
				ull q=1,sx=x,sy=y;
				rep(i,0,128) {
					x=f(x); y=f(f(y));
					ull df=x>y?x-y:y-x;
					q=mt.mul(q,df);
				}
				d=gcd(q,n);
				if(d==n) {
					d=1;
					while(d==1) {
						sx=f(sx); sy=f(f(sy));
						ull df=sx>sy?sx-sy:sy-sx;
						d=gcd(df,n);
					}
				}
			}
			if(d!=n) return d;
		}
	}
	void factor_rec(ull n,vector<ull>&res) {
		if(n==1) return;
		if(ip.isprime(n)) {
			res.push_back(n);
			return;
		}
		ull d=pollard(n);
		factor_rec(d,res);
		factor_rec(n/d,res);
	}
	vector<ull> factorize(ull n) {// 质因数分解 O(N^(1/4)*logN)
		vector<ull> res;
		if(n<2) return res;
		factor_rec(n,res);
		sort(all(res));
		return res;
	}
	vector<pair<ull,int>> factorize_count(ull n) {// 质因数分解 O(N^(1/4)*logN)
		auto data=factorize(n);
		vector<pair<ull,int>> res;
		for(ull x:data) {
			if(res.empty()||res.back().first!=x) res.emplace_back(x,1);
			else res.back().second++;
		}
		return res;
	}
};