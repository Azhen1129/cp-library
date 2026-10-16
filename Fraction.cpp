struct Fraction {
	ll num;
	ll den;
	// void normalize() {}// 不约分
	void normalize() { if(den<0) num=-num,den=-den; ll gc=gcd(num,den); num/=gc,den/=gc; }
	Fraction(ll _num=0,ll _den=1):num(_num),den(_den) { normalize(); }
	Fraction operator-() const { return Fraction(-num,den); }
	Fraction&operator+=(const Fraction&o) {
		ll lc=lcm(den,o.den);
		num=num*(lc/den)+o.num*(lc/o.den);
		den=lc;
		normalize();
		return *this;
	}
	Fraction&operator-=(const Fraction&o) {
		ll lc=lcm(den,o.den);
		num=num*(lc/den)-o.num*(lc/o.den);
		den=lc;
		normalize();
		return *this;
	}
	Fraction&operator*=(const Fraction&o) {
		num*=o.num;
		den*=o.den;
		normalize();
		return *this;
	}
	Fraction&operator/=(const Fraction&o) {
		num*=o.den;
		den*=o.num;
		normalize();
		return *this;
	}
	friend Fraction operator+(Fraction a,const Fraction&b) { return a+=b; }
	friend Fraction operator-(Fraction a,const Fraction&b) { return a-=b; }
	friend Fraction operator*(Fraction a,const Fraction&b) { return a*=b; }
	friend Fraction operator/(Fraction a,const Fraction&b) { return a/=b; }
	friend bool operator<(const Fraction&a,const Fraction&b) { return a.num*b.den<b.num*a.den; }
	friend bool operator>(const Fraction&a,const Fraction&b) { return a.num*b.den>b.num*a.den; }
	friend bool operator<=(const Fraction&a,const Fraction&b) { return a.num*b.den<=b.num*a.den; }
	friend bool operator>=(const Fraction&a,const Fraction&b) { return a.num*b.den>=b.num*a.den; }
	friend bool operator==(const Fraction&a,const Fraction&b) { return a.num*b.den==b.num*a.den; }
	friend bool operator!=(const Fraction&a,const Fraction&b) { return a.num*b.den!=b.num*a.den; }
};