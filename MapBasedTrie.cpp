template<typename K=int>
struct MapBasedTrie {
	struct Node {
		map<K,int> nex;
//		int fail;// 初始都指向 [0]
//		bool bad;
		Node() {}
//		Node():fail(0),bad(false) {}
		void emplace(const K&key,int u) { nex.emplace(key,u); }
		int&operator[](const K&key) { return nex[key]; }
		auto find(const K&key) const { return nex.find(key); }
		auto end() const { return nex.end(); }
	};
	vector<Node> Nodes;
	MapBasedTrie():Nodes(1) {}
	template<typename T>
	int insert(const T&S) {
		int idx=0;
		for(const K&key:S) {
			auto it=Nodes[idx].find(key);
			if(it!=Nodes[idx].end()) {
				idx=it->second;
				continue;
			}
			int v=(int)Nodes.size();
			Nodes.emplace_back();
			Nodes[idx].emplace(key,v);
			idx=v;
		}
//		Nodes[idx].bad=true;
		return idx;
	}
	int add(int u,const K&key) {
		auto it=Nodes[u].find(key);
		if(it!=Nodes[u].end()) return it->second;
		int v=(int)Nodes.size();
		Nodes.emplace_back();
		Nodes[u].emplace(key,v);
		return v;
	}
	int find(int u,const K&key) const {
		auto it=Nodes[u].find(key);
		if(it!=Nodes[u].end()) return it->second;
		return -1;
	}
	int size() const { return Nodes.size(); }
	Node& operator[](int u) { return Nodes[u]; }
	const Node& operator[](int u) const { return Nodes[u]; }
};