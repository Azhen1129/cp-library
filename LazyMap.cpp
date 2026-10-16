template<typename K,typename V,typename M=plus<V>>
struct LazyMap {
	M merge;
	vector<pair<K,V>> data;
	LazyMap(M _merge=M{}):merge(_merge) {}
	void insert(const K&key,const V&val) { data.emplace_back(key,val); }
	void init() {
		sort(all(data),[](const auto&p1,const auto&p2) {
			return p1.first<p2.first;
		});
		vector<pair<K,V>> res;
		int idx=0;
		for(const auto&[k,v]:data) {
			while(idx<(int)res.size()&&res[idx].first<k) idx++;
			if(idx==(int)res.size()) res.emplace_back(k,v);
			else res[idx].second=merge(res[idx].second,v);
		}
		data=move(res);
	}
	auto lower_bound(const K&key) const {
		return std::lower_bound(all(data),key,[](const pair<K,V>&p,const K&k) {
			return p.first<k;
		});
	}
	// auto lower_bound(const K&key) {// 不能去修改键！
	// 	return std::lower_bound(all(data),key,[](const pair<K,V>&p,const K&k) {
	// 		return p.first<k;
	// 	});
	// }
	auto upper_bound(const K&key) const {
		return std::upper_bound(all(data),key,[](const K&k,const pair<K,V>&p) {
			return k<p.first;
		});
	}
	// auto upper_bound(const K&key) {// 不能去修改键！
	// 	return std::upper_bound(all(data),key,[](const K&k,const pair<K,V>&p) {
	// 		return k<p.first;
	// 	});
	// }
	auto find(const K&key) const {
		auto it=lower_bound(key);
		if(it==data.end()||it->first==key) return it;
		else return data.end();
	}
	// auto find(const K&key) {// 不能去修改键！
	// 	auto it=lower_bound(key);
	// 	if(it==data.end()||it->first==key) return it;
	// 	else return data.end();
	// }
	int lower_bound_ID(const K&key) const { return lower_bound(key)-data.begin(); }
	int upper_bound_ID(const K&key) const { return upper_bound(key)-data.begin(); }
	int find_ID(const K&key) const { return find(key)-data.begin(); }
	auto begin() const { return data.begin(); }
	auto rbegin() const { return data.rbegin(); }
	auto end() const { return data.end(); }
	auto rend() const { return data.rend(); }
	int size() const { return data.size(); }
	bool count(const K&key) const { return find(key)!=data.end(); }
	const pair<K,V>& kth(int idx) const { return data[idx]; }
};