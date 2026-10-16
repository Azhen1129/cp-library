template<typename T>
struct Array2D {
	int r=0;
	int c=0;
	vector<T> data;
	Array2D()=default;
	Array2D(int nr,int nc):r(nr),c(nc),data((size_t)nr*nc) {}
	Array2D(int nr,int nc,const T&val):r(nr),c(nc),data((size_t)nr*nc,val) {}
	bool operator==(const Array2D&) const = default;
	void assign(int nr,int nc) { r=nr; c=nc; data.assign((size_t)nr*nc,T{}); }
	void assign(int nr,int nc,const T&val) { r=nr; c=nc; data.assign((size_t)nr*nc,val); }
	/*---------- 访问 ----------*/
	T&operator[](int i,int j) { return data[i*c+j]; }
	const T&operator[](int i,int j) const { return data[i*c+j]; }
	span<T> row(int q) { return span<T>(data.data()+q*c,c); }
	span<const T> row(int q) const { return span<const T>(data.data()+q*c,c); }
	span<T> operator[](int i) { return row(i); }
	span<const T> operator[](int i) const { return row(i); }
	auto col(int q) { return data|views::drop(q)|views::stride(c); }
	auto col(int q) const { return data|views::drop(q)|views::stride(c); }
	int ID(int i,int j) const { return i*c+j; }
	pair<int,int> invID(int q) const { return {q/c,q%c}; }
	/*---------- 填充 ----------*/
	void fill(const T&val) { ranges::fill(data,val); }
	void row_fill(int q,const T&val) { ranges::fill((*this)[q],val); }
	void col_fill(int q,const T&val) { ranges::fill(col(q),val); }
	/*---------- 最值 ----------*/
	T all_max() const { return *ranges::max_element(data); }
	T all_min() const { return *ranges::min_element(data); }
	T row_max(int q) const { auto row=(*this)[q]; return *ranges::max_element(row); }
	T row_min(int q) const { auto row=(*this)[q]; return *ranges::min_element(row); }
	int row_argmax(int q) const { auto row=(*this)[q]; return (int)(ranges::max_element(row)-row.begin()); }// 返回列号，首个最大值
	int row_argmin(int q) const { auto row=(*this)[q]; return (int)(ranges::min_element(row)-row.begin()); }// 返回列号，首个最小值
	T col_max(int q) const { auto v=col(q); return *ranges::max_element(v); }
	T col_min(int q) const { auto v=col(q); return *ranges::min_element(v); }
	int col_argmax(int q) const { auto v=col(q); return (int)(ranges::max_element(v)-v.begin()); }// 返回行号，首个最大值
	int col_argmin(int q) const { auto v=col(q); return (int)(ranges::min_element(v)-v.begin()); }// 返回行号，首个最小值
	/*---------- 子矩阵 ----------*/
	Array2D submatrix(int r1,int r2,int c1,int c2) const {// [r1, r2] × [c1, c2] 闭区间
		Array2D res(r2-r1+1,c2-c1+1);
		rep(i,r1,r2+1) rep(j,c1,c2+1) res[i-r1,j-c1]=(*this)[i,j];
		return res;
	}
	/*---------- 旋转 / 转置 ----------*/
	T&rot90_at(int i,int j) { return (*this)[r-j-1,i]; }// 顺时针 90° 
	const T&rot90_at(int i,int j) const { return (*this)[r-j-1,i]; }
	T&rot180_at(int i,int j) { return (*this)[r-i-1,c-j-1]; }
	const T&rot180_at(int i,int j) const { return (*this)[r-i-1,c-j-1]; }
	T&rot270_at(int i,int j) { return (*this)[j,c-i-1]; }// 逆时针 90°
	const T&rot270_at(int i,int j) const { return (*this)[j,c-i-1]; }
	Array2D rot90() const {// 顺时针 90° 
		Array2D res(c,r);
		rep(i,0,r) rep(j,0,c) res[j,r-i-1]=(*this)[i,j];
		return res;
	}
	Array2D rot180() const {
		Array2D res(r,c);
		rep(i,0,r) rep(j,0,c) res[r-i-1,c-j-1]=(*this)[i,j];
		return res;
	}
	Array2D rot270() const {// 逆时针 90°
		Array2D res(c,r);
		rep(i,0,r) rep(j,0,c) res[c-j-1,i]=(*this)[i,j];
		return res;
	}
	Array2D tran() const {// 转置
		Array2D res(c,r);
		rep(i,0,r) rep(j,0,c) res[j,i]=(*this)[i,j];
		return res;
	}
	void print() const {
		rep(i,0,r) {
			auto row=(*this)[i];
			rep(j,0,c) {
				if(j) cout<<' ';
				cout<<row[j];
			}
			cout<<'\n';
		}
	}
	/*---------- 二维前缀（1-idx，原地破坏性） ----------*/
	// 数据须写在 1..r-1 行 / 1..c-1 列，第 0 行与第 0 列全为 0
	void build_prefix() {// 只能调一次
		rep(i,1,r) rep(j,1,c) (*this)[i,j]+=(*this)[i-1,j];
		rep(i,1,r) rep(j,1,c) (*this)[i,j]+=(*this)[i,j-1];
	}
	T rect_sum(int r1,int r2,int c1,int c2) const {// [r1, r2] × [c1, c2] 闭区间，1-idx
		return (*this)[r2,c2]-(*this)[r1-1,c2]-(*this)[r2,c1-1]+(*this)[r1-1,c1-1];
	}
};