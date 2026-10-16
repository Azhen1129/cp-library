template<int N,int Offset>
struct Trie {
	struct Node {
		int nex[N];
		Node() { fill(nex,nex+N,0); }
		int&operator[](const int idx) { return nex[idx]; }
		int operator[](const int idx) const { return nex[idx]; }
	};
	vector<Node> Nodes;
	Trie():Nodes(1) {}
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
//		插入信息
		return idx;
	}
	Node&operator[](const int idx) { return Nodes[idx]; }
	const Node&operator[](const int idx) const { return Nodes[idx]; }
	int size() const { return Nodes.size(); }
};