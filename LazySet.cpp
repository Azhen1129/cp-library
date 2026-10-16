template<typename K>
struct LazySet {
	vector<K> data;
	LazySet() {}
	void insert(const K&key) { data.push_back(key); }
	void init() { sort(all(data)); data.erase(unique(all(data)),data.end()); }
	auto begin() const { return data.begin(); }
	auto rbegin() const { return data.rbegin(); }
	auto end() const { return data.end(); }
	auto rend() const { return data.rend(); }
	auto lower_bound(const K&key) const { return std::lower_bound(all(data),key); }
	auto upper_bound(const K&key) const { return std::upper_bound(all(data),key); }
	auto find(const K&key) const {
		auto it=lower_bound(key);
		if(it==data.end()||*it==key) return it;
		else return data.end();
	}
	int lower_bound_ID(const K&key) const { return lower_bound(key)-data.begin(); }
	int upper_bound_ID(const K&key) const { return upper_bound(key)-data.begin(); }
	int find_ID(const K&key) const { return find(key)-data.begin(); }
	int size() const { return data.size(); }
	bool count(const K&key) const { return find(key)!=data.end(); }
	const K& kth(int idx) const { return data[idx]; }
};