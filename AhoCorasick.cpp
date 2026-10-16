template<int N,int Offset>
struct ACTrie {
	struct Node {
		int nex[N];
		int fail;// 初始都指向 [0]
		bool bad;
		Node():fail(0),bad(false) { fill(nex,nex+N,0); }
		int&operator[](const int idx) { return nex[idx]; }
		int operator[](const int idx) const { return nex[idx]; }
	};
	vector<Node> Nodes;
	ACTrie():Nodes(1) {}
	template<typename T>
	int insert(const T&S) {
		int idx=0;
		for(auto ch:S) {
			int c=(int)ch-Offset;
			if(!Nodes[idx][c]) {
				Nodes[idx][c]=Nodes.size();
				Nodes.emplace_back();
			}
			idx=Nodes[idx][c];
		}
		Nodes[idx].bad=true;
		return idx;
	}
	Node&operator[](const int idx) { return Nodes[idx]; }
	const Node&operator[](const int idx) const { return Nodes[idx]; }
	int size() const { return Nodes.size(); }
};
template<int N,int Offset>
struct AhoCorasick {
	ACTrie<N,Offset> trie;
	AhoCorasick():trie() {}
	template<typename T>
	int insert(const T&S) { return trie.insert(S); }
	void init() {
		queue<int> q;
		rep(c,0,N) { int u=trie[0][c]; if(u) q.push(u); }
		while(!q.empty()) {
			int u=q.front(); q.pop();
			trie[u].bad|=trie[trie[u].fail].bad;
			rep(c,0,N) {
				int v=trie[u][c];
				if(v) {
					trie[v].fail=trie[trie[u].fail][c];
					q.push(v);
				}
				else trie[u][c]=trie[trie[u].fail][c];
			}
		}
	}
	template<typename T>
	int query(const T&S) {// 查询出现违禁词的位置
		int u=0,pos=0;
		for(auto ch:S) {
			int c=ch-Offset;
			u=trie[u][c];
			if(trie[u].bad) return pos;
			pos++;
		}
		return -1;
	}
};