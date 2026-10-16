template<typename T>
struct Matrix {
	int r=0;
	int c=0;
	vector<T> data;
	Matrix()=default;
	Matrix(int nr,int nc):r(nr),c(nc),data((size_t)nr*nc) {}
	Matrix(int nr,int nc,const T&val):r(nr),c(nc),data((size_t)nr*nc,val) {}
	bool operator==(const Matrix&) const = default;
	void assign(int nr,int nc) { r=nr; c=nc; data.assign((size_t)nr*nc,T{}); }
	void assign(int nr,int nc,const T&val) { r=nr; c=nc; data.assign((size_t)nr*nc,val); }
	/*---------- 访问 ----------*/
	T&operator[](int i,int j) { return data[i*c+j]; }
	const T&operator[](int i,int j) const { return data[i*c+j]; }
	span<T> row(int q) { return span<T>(data.data()+q*c,c); }
	span<const T> row(int q) const { return span<const T>(data.data()+q*c,c); }
	span<T> operator[](int i) { return row(i); }
	span<const T> operator[](int i) const { return row(i); }
	int ID(int i,int j) const { return i*c+j; }
	pair<int,int> invID(int q) const { return {q/c,q%c}; }
	int nvals() const { return (int)data.size(); }
	/*---------- 填充 ----------*/
	void fill(const T&val) { ranges::fill(data,val); }
	/*---------- 矩阵运算 ----------*/
	Matrix&operator+=(const Matrix&o) { rep(i,0,nvals()) data[i]+=o.data[i]; return *this; }
	Matrix&operator-=(const Matrix&o) { rep(i,0,nvals()) data[i]-=o.data[i]; return *this; }
	Matrix operator+(const Matrix&o) const { Matrix res(*this); return res+=o; }
	Matrix operator-(const Matrix&o) const { Matrix res(*this); return res-=o; }
	Matrix operator*(const Matrix&o) const {
		Matrix res(r,o.c);
		rep(i,0,r) rep(k,0,c) rep(j,0,o.c) res[i,j]+=(*this)[i,k]*o[k,j];
		return res;
	}
	Matrix&operator*=(const Matrix&o) { return *this=*this*o; }// 先算值再移动赋值，A*=A 也安全
	static Matrix pow(Matrix A,ll exp) {
		const int n=A.r;
		Matrix res(n,n);
		rep(i,0,n) res[i,i]=1;
		while(exp) {
			if(exp&1) res=res*A;
			A*=A; exp>>=1;
		}
		return res;
	}
	Matrix tran() const {
		Matrix res(c,r);
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
};