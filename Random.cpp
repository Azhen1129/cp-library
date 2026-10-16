struct Random {
	mt19937_64 rng;
	Random() {
		auto now=chrono::high_resolution_clock::now().time_since_epoch().count();
		random_device rd;
		rng.seed(rd()^now);
	}
	int rint(int l,int r) {
		uniform_int_distribution<int> dist(l,r);
		return dist(rng);
	}
	ll rll(ll l,ll r) {
		uniform_int_distribution<ll> dist(l,r);
		return dist(rng);
	}
	ull rull(ull l,ull r) {
		uniform_int_distribution<ull> dist(l,r);
		return dist(rng);
	}
	size_t rsize(size_t l,size_t r) {
		uniform_int_distribution<size_t> dist(l,r);
		return dist(rng);
	}
	double rdouble(double l,double r) {
		uniform_real_distribution<double> dist(l,r);
		return dist(rng);
	}
};
Random rnd;