struct IsPrime {
	static const ull B7[7];
	bool isprime(ull n) const {
		if(n<2) return false;
		if(n%2==0) return n==2;
		if(n%3==0) return n==3;
		if(n%5==0) return n==5;
		if(n%7==0) return n==7;
		ull d=n-1;
		int s=__builtin_ctzll(d);
		d>>=s;
		Mont mt(n);
		ull one=mt.mul(1,mt.r2),mone=n-one;
		static const ull s7[7]={2,3,5,7,11,13,17};
		const ull *B=B7;
		int k=7;
		if(n<3215031751ULL) { B=s7; k=4; }
		else if(n<3474749660383ULL) { B=s7; k=6; }
		else if(n<341550071728321ULL) B=s7;
		rep(i,0,k) {
			ull a=B[i]%n;
			if(a==0) continue;
			ull x=mt.pow(a,d);
			if(x==one||x==mone) continue;
			bool ok=false;
			rep(j,1,s) {
				x=mt.mul(x,x);
				if(x==mone) { ok=true; break; }
			}
			if(!ok) return false;
		}
		return true;
	}
	bool isprime(ll x) const { return x>=0&&isprime((ull)x); }// O(logN)
	bool isprime(int x) const { return x>=0&&isprime((ull)x); }// O(logN)
	bool operator()(ull x) const { return isprime(x); }// O(logN)
	bool operator()(ll x) const { return isprime(x); }// O(logN)
	bool operator()(int x) const { return isprime(x); }// O(logN)
};
const ull IsPrime::B7[7]={2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
IsPrime ip;