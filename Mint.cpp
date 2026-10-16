const ll MOD=998244353;
struct Mint {
	ll x;
	Mint():x(0) {}
	Mint(ll _x):x(_x%MOD) { if(x<0) x+=MOD; }
	template<typename E>
	static Mint pow(Mint a,E e) {// e >= 0
		Mint res=1;
		while(e) { if(e&1) res*=a; a*=a; e>>=1; }
		return res;
	}
	Mint inv() const { return Mint::pow(*this,MOD-2); }// x != 0
	Mint invExgcd() const {// gcd(x, MOD) = 1
		ll a=x,b=MOD,u=1,v=0;
		while(b) {
			ll t=a/b;
			a-=t*b; swap(a,b);
			u-=t*v; swap(u,v);
		}
		return Mint(u);
	}
	Mint&operator+=(const Mint&o) { x+=o.x; if(x>=MOD) x-=MOD; return *this; }
	Mint&operator-=(const Mint&o) { x-=o.x; if(x<0) x+=MOD; return *this; }
	Mint&operator*=(const Mint&o) { x=x*o.x%MOD; return *this; }
	Mint&operator/=(const Mint&o) { return *this*=o.inv(); }
	Mint operator+(const Mint&o) const { return Mint(*this)+=o; }
	Mint operator-(const Mint&o) const { return Mint(*this)-=o; }
	Mint operator*(const Mint&o) const { return Mint(*this)*=o; }
	Mint operator/(const Mint&o) const { return Mint(*this)/=o; }
	Mint operator-() const { return Mint(-x); }
	bool operator==(const Mint&o) const { return x==o.x; }
	bool operator!=(const Mint&o) const { return x!=o.x; }
	bool operator<(const Mint&o) const { return x<o.x; }// 这是给 std::set，std::map 之类的用的
	friend ostream&operator<<(ostream&os,const Mint&m) { os<<m.x; return os; }
	friend istream&operator>>(istream&is,Mint&m) { ll t; is>>t; m=Mint(t); return is; }
};