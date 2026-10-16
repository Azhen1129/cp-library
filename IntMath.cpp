struct IntMath {
	template<typename T,typename E,typename M>
	static T pow(T acc,T a,E e,M merge) {// e 要是正整数
		while(e) { if(e&1) acc=merge(acc,a); a=merge(a,a); e>>=1; } return acc;
	}
	template<typename T,typename E>
	static T pow(T acc,T a,E e) {// e 要是正整数
		while(e) { if(e&1) acc*=a; a*=a; e>>=1; } return acc;
	}
	template<typename T,typename E>
	static T mod_pow(T acc,T a,E e,T mod) {// e 要是正整数
		while(e) { if(e&1) acc=acc*a%mod; a=a*a%mod; e>>=1; } return acc;
	}
	template<typename T>
	static T sqrt(T n) {// O(1)
		if(n<2) return n;
		if constexpr(sizeof(T)<=8) {
			T x=(T)std::sqrt(double(n));
			while((ui128)x*x>n) x--;
			while((ui128)(x+1)*(x+1)<=n) x++;
			return x;
		}
		else {
			const T TWO64=(T)~0ULL+1;
			auto le=[&](T k) { return k<TWO64&&k*k<=n; };
			T x=(T)std::sqrt((double)n);
			x+=(x>>50)+8;
			T y=(x+n/x)>>1;
			while(le(y+1)) y++;
			while(!le(y)) y--;
			return y;
		}
	}
	template<typename T>
	static bool issquare(T n) {
		if constexpr(std::is_signed_v<T>) if(n<0) return false;
		T r=sqrt(n);
		return r*r==n;
	}
	template<typename T=vector<int>>
	static int mex(const T&A) { 
		const int N=A.size();
		vector<char> has(N+1,false);
		for(auto x:A) if(x>=0&&x<=N) has[x]=true;
		rep(i,0,N+1) if(!has[i]) return i;
		return N+1;// 理论上不可能
	}
};