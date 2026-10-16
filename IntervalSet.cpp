template<typename T>
struct IntervalSet {// [ , ) [ , ) [ , ) !!!!!!
	map<T,T> data;
	T total;
	IntervalSet():total(0) {}
	void add(T l,T r) {
		if(l>=r) return;
		auto it=data.lower_bound(l);
		if(it!=data.begin()&&prev(it)->second>=l) it--;
		while(it!=data.end()&&it->first<=r) {
			l=min(l,it->first);
			r=max(r,it->second);
			total-=it->second-it->first;
			it=data.erase(it);
		}
		data[l]=r;
		total+=r-l;
	}
	void remove(T l,T r) {
		if(l>=r) return;
		auto it=data.upper_bound(l);
		if(it!=data.begin()&&prev(it)->second>l) it--;
		while(it!=data.end()&&it->first<r) {
			auto [pl,pr]=*it;
			total-=pr-pl;
			it=data.erase(it);
			if(pl<l) data[pl]=l,total+=l-pl;
			if(pr>r) data[r]=pr,total+=pr-r;
		}
	}
	bool empty() const { return data.empty(); }
	int size() const { return data.size(); }
	T len() const { return total; }
	bool covers(T l,T r) const {// 是否完全覆盖
		if(l>=r) return true;
		auto it=data.upper_bound(l);
		if(it==data.begin()) return false;
		return prev(it)->second>=r;
	}
	bool intersects(T l,T r) const {// 是否有相交
		if(l>=r) return false;
		auto it=data.upper_bound(l);
		if(it!=data.begin()&&prev(it)->second>l) return true;
		return it!=data.end()&&it->first<r;
	}
};