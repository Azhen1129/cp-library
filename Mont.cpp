struct Mont {// 蒙哥马利乘法
	ull n,inv,r2;
	Mont(ull _n):n(_n),inv(1) {
		rep(i,0,6) inv*=2-n*inv;
		inv=-inv;
		r2=(ull)(((ui128)1<<64)%n);
		r2=(ull)((ui128)r2*r2%n);
	}
	ull mul(ull a,ull b) const {
		ui128 t=(ui128)a*b;
		ull tl=(ull)t,th=(ull)(t>>64);
		ui128 mn=(ui128)(tl*inv)*n;
		ull r=th+(ull)(mn>>64)+(tl!=0);
		if(r<th||r>=n) r-=n;
		return r;
	}
	ull pow(ull a,ull b) const {
		ull r=mul(1,r2),x=mul(a,r2);
		for(;b;b>>=1) {
			if(b&1) r=mul(r,x);
			x=mul(x,x);
		}
		return r;
	}
};