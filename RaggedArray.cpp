template<typename T>
struct RaggedArray {
	vector<T> data;// 所有元素
	vector<int> offsets;// 行起点，长度 = nrows()+1
	RaggedArray():offsets(1,0) {}
	explicit RaggedArray(int N):offsets(N+1,0) {}
	RaggedArray(const RaggedArray&)=default;
	RaggedArray&operator=(const RaggedArray&)=default;
	RaggedArray(RaggedArray&&o) noexcept:data(move(o.data)),offsets(move(o.offsets)) {
		o.data.clear();
		o.offsets.assign(1,0);
	}
	RaggedArray&operator=(RaggedArray&&o) noexcept {
		if(this!=&o) {
			data=move(o.data);
			offsets=move(o.offsets);
			o.data.clear();
			o.offsets.assign(1,0);
		}
		return *this;
	}
	int nrows() const { return (int)offsets.size()-1; }// 行数
	int nvals() const { return (int)data.size(); }// 总元素数
	int row_size(int i) const { return offsets[i+1]-offsets[i]; }// 第 i 行的列数
	bool empty() const { return offsets.size()==1; }// 一行都没有
	bool row_empty(int i) const { return offsets[i]==offsets[i+1]; }// 第 i 行一个元素都没有
	bool operator==(const RaggedArray&o) const = default;
	void reserve_data(int nv) { data.reserve(nv); }
	void reserve_rows(int nr) { offsets.reserve(nr+1); }
	void reserve(int nv,int nr) { reserve_data(nv); reserve_rows(nr); }
	void shrink_to_fit() { data.shrink_to_fit(); offsets.shrink_to_fit(); }
	void clear() { data.clear(); offsets.assign(1,0); }
	span<T> operator[](int i) { return span<T>(data.data()+offsets[i],offsets[i+1]-offsets[i]); }
	span<const T> operator[](int i) const { return span<const T>(data.data()+offsets[i],offsets[i+1]-offsets[i]); }
	T&operator[](int i,int j) { return data[offsets[i]+j]; }
	const T&operator[](int i,int j) const { return data[offsets[i]+j]; }
	int push_row() {// 开一个空行，返回行号
		offsets.push_back((int)data.size());
		return nrows()-1;
	}
	int push_row(span<const T> row) {// 追加一整行，返回行号
		data.insert(data.end(),row.begin(),row.end());
		offsets.push_back((int)data.size());
		return nrows()-1;
	}
	int push_row(initializer_list<T> row) { return push_row(span<const T>(row.begin(),row.size())); }// 追加一整行，返回行号
	void push_back(const T&x) { data.push_back(x); offsets.back()=(int)data.size(); }// nrows()>0
	void push_back(T&&x) { data.push_back(move(x)); offsets.back()=(int)data.size(); }// nrows()>0
	template<typename... A>
	T&emplace_back(A&&...args) {
		data.emplace_back(forward<A>(args)...);
		offsets.back()=(int)data.size();
		return data.back();
	}
	void pop_back() { data.pop_back(); offsets.back()=(int)data.size(); }// 前置：最后一行非空
	void pop_row() { offsets.pop_back(); data.resize(offsets.back()); }// 前置：nrows()>0
	auto begin() { return data.begin(); }
	auto end() { return data.end(); }
	auto begin() const { return data.begin(); }
	auto end() const { return data.end(); }
	template<typename Self,typename Elem>
	struct RowsView {
		Self*a;
		struct Iter {
			Self*a;
			int i;
			span<Elem> operator*() const { return (*a)[i]; }
			Iter&operator++() { i++; return *this; }
			bool operator!=(const Iter&o) const { return i!=o.i; }
		};
		Iter begin() const { return Iter{a,0}; }
		Iter end() const { return Iter{a,a->nrows()}; }
		int size() const { return a->nrows(); }
		span<Elem> operator[](int i) const { return (*a)[i]; }
	};
	RowsView<RaggedArray,T> rows() { return RowsView<RaggedArray,T>{this}; }
	RowsView<const RaggedArray,const T> rows() const { return RowsView<const RaggedArray,const T>{this}; }
	void print() const {
		rep(i,0,nrows()) {
			auto row=(*this)[i];
			rep(j,0,(int)row.size()) {
				if(j) cout<<' ';
				cout<<row[j];
			}
			cout<<'\n';
		}
	}
	void swap(RaggedArray&o) noexcept { data.swap(o.data); offsets.swap(o.offsets); }
	friend void swap(RaggedArray&a,RaggedArray&b) noexcept { a.swap(b); }
	/*---------- 和 vector<vector<T>> 互转 ----------*/
	static RaggedArray from_nested(const vector<vector<T>>&v) {
		RaggedArray res;
		int tot=0;
		for(const auto&row:v) tot+=(int)row.size();
		res.data.reserve(tot);
		res.offsets.reserve((int)v.size()+1);
		for(const auto&row:v) {
			res.data.insert(res.data.end(),all(row));
			res.offsets.push_back((int)res.data.size());
		}
		return res;
	}
	static RaggedArray from_nested(vector<vector<T>>&&v) {// 源：结构保留（行数/行长不变），元素逐个被搬走
		RaggedArray res;
		int tot=0;
		for(const auto&row:v) tot+=(int)row.size();
		res.data.reserve(tot);
		res.offsets.reserve((int)v.size()+1);
		for(auto&row:v) {
			for(auto&x:row) res.data.push_back(move(x));
			res.offsets.push_back((int)res.data.size());
		}
		return res;
	}
	vector<vector<T>> to_nested() const {
		vector<vector<T>> res(nrows());
		rep(i,0,nrows()) res[i].assign(data.begin()+offsets[i],data.begin()+offsets[i+1]);
		return res;
	}
};