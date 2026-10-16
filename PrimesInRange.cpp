vector<ll> PrimesInRange(ll L,ll R) {
	if(L>R||R<2) return {};
	L=max(L,2LL);
	ll len=R-L+1;
	ll lim=(ll)sqrtl((ld)R);
	while((i128)(lim+1)*(lim+1)<=R) lim++;
	while((i128)lim*lim>R) lim--;
	const ll CAP=1e8;
	if(lim>CAP||len<lim/400) {
		vector<ll> res;
		for(ll x=L;x<=R;x++) if(ip(x)) res.push_back(x);
		return res;
	}
	vector<int> base;
	if(lim>=2) {
		base.push_back(2);
		vector<bool> vis(lim/2+1,false);
		for(int i=3;i<=lim;i+=2) {
			if(vis[i>>1]) continue;
			base.push_back(i);
			for(ll j=(ll)i*i;j<=lim;j+=2LL*i) vis[j>>1]=true;
		}
	}
	const int BLOCK=1<<23;
	vector<char> ok(BLOCK);
	vector<ll> res;
	for(ll l=L;l<=R;l+=BLOCK) {
		ll r=min(R,l+BLOCK-1);
		int m=(int)(r-l+1);
		fill(ok.begin(),ok.begin()+m,1);
		for(int p:base) {
			if((ll)p*p>r) break;
			ll st=max((ll)p*p,(l+p-1)/p*p);
			for(ll x=st;x<=r;x+=p) ok[x-l]=0;
		}
		for(ll x=l;x<=r;x++) if(ok[x-l]) res.push_back(x);
	}
	return res;
}