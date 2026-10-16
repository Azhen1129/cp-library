namespace Hash {
	// array<ll,1> mod={998244353LL};// 四选一，单模，可指定模数
	// const array<ll,1> mod={998244353LL};// 四选一，单模，不可指定模数
	// array<ll,3> mod={998244353LL,1000000007LL,1000000009LL};// 四选一，多模，可指定模数
	const array<ll,3> mod={998244353LL,1000000007LL,1000000009LL};// 四选一，多模，不可指定模数
	const int mod_cnt=mod.size();
	const ll base_l=1919810LL;
	const ll base_r=998114514LL;
	// array<ll,mod_cnt> base;// 二选一，指定基
	array<ll,mod_cnt> base=[] {// 二选一，随机基
		array<ll,mod_cnt> b{};
		rep(i,0,mod_cnt) b[i]=rnd.rll(base_l,base_r);
		return b;
	}();
	struct Node {
		array<ll,mod_cnt> hash;
		Node():hash{} {}
		Node(ll val) {
			rep(i,0,mod_cnt) {
				hash[i]=val%mod[i];
				if(hash[i]<0) hash[i]+=mod[i];
			}
		}
		ll& operator[](int idx) { return hash[idx]; }
		ll operator[](int idx) const { return hash[idx]; }
		bool operator==(const Node&H) const { return hash==H.hash; }
		bool operator!=(const Node&H) const { return hash!=H.hash; }
		bool operator<(const Node&H) const { return hash<H.hash; }
	};
	template<typename M=plus<ll>>
	struct Merge {
		using Node=Hash::Node;
		M merge;
		Merge(M _merge=M{}):merge(_merge) {}
		Node operator()(const Node&A,const Node&B) const {
			Node res;
			rep(i,0,mod_cnt) res[i]=merge(A[i],B[i])%mod[i];
			return res;
		}
		// plus<ll>			+
		// multiplies<ll>	*
	};
	struct SequenceNode {
		using Node=Hash::Node;
		Node hash;
		Node power;
		SequenceNode() { rep(i,0,mod_cnt) hash[i]=0LL,power[i]=1LL; }
		SequenceNode(const ll&val) {
			rep(i,0,mod_cnt) {
				hash[i]=val%mod[i];
				if(hash[i]<0) hash[i]+=mod[i];
				power[i]=base[i]%mod[i];
			}
		}
		bool operator==(const SequenceNode&A) const { return hash==A.hash&&power==A.power; }
		bool operator!=(const SequenceNode&A) const { return hash!=A.hash||power!=A.power; }
		bool operator<(const SequenceNode&A) const { return tie(hash,power)<tie(A.hash,A.power); }
	};
	struct SequenceMerge {
		using Node=SequenceNode;
		Node operator()(const Node&A,const Node&B) const {
			Node C;
			auto&[Ch,Cp]=C;
			const auto&[Ah,Ap]=A;
			const auto&[Bh,Bp]=B;
			rep(i,0,mod_cnt) Ch[i]=(Ah[i]*Bp[i]+Bh[i])%mod[i];
			rep(i,0,mod_cnt) Cp[i]=Ap[i]*Bp[i]%mod[i];
			return C;
		}
	};
	struct ReversedSequenceMerge {
		using Node=SequenceNode;
		Node operator()(const Node&A,const Node&B) const {
			return SequenceMerge{}(B,A);
		}
	};
	template<typename T>
	SequenceNode MakeSequenceNode(const T&data) {
		using Node=SequenceNode;
		Node res;
		auto&[hash,power]=res;
		for(const auto&val:data) rep(i,0,mod_cnt) {
			hash[i]=(hash[i]*base[i]+val)%mod[i];
			if(hash[i]<0) hash[i]+=mod[i];
			power[i]=(power[i]*base[i])%mod[i];
		}
		return res;
	}
	template<typename T>
	SequenceNode MakeSequenceNode(const T&data,ll offset,ll base) {
		using Node=SequenceNode;
		Node res;
		auto&[hash,power]=res;
		for(const auto&val:data) rep(i,0,mod_cnt) {
			hash[i]=(hash[i]*base+val-offset)%mod[i];
			if(hash[i]<0) hash[i]+=mod[i];
			power[i]=(power[i]*base)%mod[i];
		}
		return res;
	}
}