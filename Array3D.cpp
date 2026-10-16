template<typename T>
struct Array3D {
	int x=0;
	int y=0;
	int z=0;
	vector<T> data;
	Array3D()=default;
	Array3D(int nx,int ny,int nz):x(nx),y(ny),z(nz),data((size_t)nx*ny*nz) {}
	Array3D(int nx,int ny,int nz,const T&val):x(nx),y(ny),z(nz),data((size_t)nx*ny*nz,val) {}
	bool operator==(const Array3D&) const = default;
	void assign(int nx,int ny,int nz) { x=nx; y=ny; z=nz; data.assign((size_t)nx*ny*nz,T{}); }
	void assign(int nx,int ny,int nz,const T&val) { x=nx; y=ny; z=nz; data.assign((size_t)nx*ny*nz,val); }
	T&operator[](int i,int j,int k) { return data[(i*y+j)*z+k]; }
	const T&operator[](int i,int j,int k) const { return data[(i*y+j)*z+k]; }
	span<T> plane(int qi) { return span<T>(data.data()+qi*y*z,y*z); }
	span<const T> plane(int qi) const { return span<const T>(data.data()+qi*y*z,y*z); }
	span<T> row(int qi,int qj) { return span<T>(data.data()+(qi*y+qj)*z,z); }
	span<const T> row(int qi,int qj) const { return span<const T>(data.data()+(qi*y+qj)*z,z); }
	span<T> operator[](int i) { return plane(i); }
	span<const T> operator[](int i) const { return plane(i); }
	span<T> operator[](int i,int j) { return row(i,j); }
	span<const T> operator[](int i,int j) const { return row(i,j); }
	auto line_i(int qj,int qk) { return data|views::drop(qj*z+qk)|views::stride(y*z); }
	auto line_i(int qj,int qk) const { return data|views::drop(qj*z+qk)|views::stride(y*z); }
	auto line_j(int qi,int qk) { return data|views::drop(qi*y*z+qk)|views::stride(z)|views::take(y); }
	auto line_j(int qi,int qk) const { return data|views::drop(qi*y*z+qk)|views::stride(z)|views::take(y); }
	int ID(int i,int j,int k) const { return (i*y+j)*z+k; }
	array<int,3> invID(int q) const { return {q/(y*z),q/z%y,q%z}; }
	/*---------- 填充 ----------*/
	void fill(const T&val) { ranges::fill(data,val); }
	void x_fill(int qi,const T&val) { ranges::fill((*this)[qi],val); }
	void y_fill(int qj,const T&val) { rep(i,0,x) rep(k,0,z) (*this)[i,qj,k]=val; }
	void z_fill(int qk,const T&val) { rep(i,0,x) rep(j,0,y) (*this)[i,j,qk]=val; }
	/*---------- 最值 ----------*/
	T all_max() const { return *ranges::max_element(data); }
	T all_min() const { return *ranges::min_element(data); }
	T x_max(int qi) const { auto p=plane(qi); return *ranges::max_element(p); }
	T x_min(int qi) const { auto p=plane(qi); return *ranges::min_element(p); }
	pair<int,int> x_argmax(int qi) const { auto p=plane(qi); int q=(int)(ranges::max_element(p)-p.begin()); return {q/z,q%z}; }// (j,k) 首个最大
	pair<int,int> x_argmin(int qi) const { auto p=plane(qi); int q=(int)(ranges::min_element(p)-p.begin()); return {q/z,q%z}; }// (j,k) 首个最小
	T y_max(int qj) const { T res=(*this)[0,qj,0]; rep(i,0,x) rep(k,0,z) res=max(res,(*this)[i,qj,k]); return res; }
	T y_min(int qj) const { T res=(*this)[0,qj,0]; rep(i,0,x) rep(k,0,z) res=min(res,(*this)[i,qj,k]); return res; }
	pair<int,int> y_argmax(int qj) const { T best=(*this)[0,qj,0]; int bi=0,bk=0; rep(i,0,x) rep(k,0,z) if((*this)[i,qj,k]>best) { best=(*this)[i,qj,k]; bi=i; bk=k; } return {bi,bk}; }// (i,k) 首个最大
	pair<int,int> y_argmin(int qj) const { T best=(*this)[0,qj,0]; int bi=0,bk=0; rep(i,0,x) rep(k,0,z) if((*this)[i,qj,k]<best) { best=(*this)[i,qj,k]; bi=i; bk=k; } return {bi,bk}; }// (i,k) 首个最小
	T z_max(int qk) const { T res=(*this)[0,0,qk]; rep(i,0,x) rep(j,0,y) res=max(res,(*this)[i,j,qk]); return res; }
	T z_min(int qk) const { T res=(*this)[0,0,qk]; rep(i,0,x) rep(j,0,y) res=min(res,(*this)[i,j,qk]); return res; }
	pair<int,int> z_argmax(int qk) const { T best=(*this)[0,0,qk]; int bi=0,bj=0; rep(i,0,x) rep(j,0,y) if((*this)[i,j,qk]>best) { best=(*this)[i,j,qk]; bi=i; bj=j; } return {bi,bj}; }// (i,j) 首个最大
	pair<int,int> z_argmin(int qk) const { T best=(*this)[0,0,qk]; int bi=0,bj=0; rep(i,0,x) rep(j,0,y) if((*this)[i,j,qk]<best) { best=(*this)[i,j,qk]; bi=i; bj=j; } return {bi,bj}; }// (i,j) 首个最小
	/*---------- 三维前缀（1-idx，原地破坏性） ----------*/
	// 数据须写在 1..x-1 层 / 1..y-1 行 / 1..z-1 列，第 0 层/行/列全为 0
	void build_prefix() {// 只能调一次
		rep(i,1,x) rep(j,1,y) rep(k,1,z) (*this)[i,j,k]+=(*this)[i-1,j,k];
		rep(i,1,x) rep(j,1,y) rep(k,1,z) (*this)[i,j,k]+=(*this)[i,j-1,k];
		rep(i,1,x) rep(j,1,y) rep(k,1,z) (*this)[i,j,k]+=(*this)[i,j,k-1];
	}
	T box_sum(int x1,int x2,int y1,int y2,int z1,int z2) const {// [x1, x2] × [y1, y2] × [z1, z2] 闭区间
		return (*this)[x2,y2,z2]
			-(*this)[x1-1,y2,z2]-(*this)[x2,y1-1,z2]-(*this)[x2,y2,z1-1]
			+(*this)[x1-1,y1-1,z2]+(*this)[x1-1,y2,z1-1]+(*this)[x2,y1-1,z1-1]
			-(*this)[x1-1,y1-1,z1-1];
	}
	void print() const {// 层与层之间空一行
		rep(i,0,x) {
			if(i) cout<<'\n';
			rep(j,0,y) {
				rep(k,0,z) {
					if(k) cout<<' ';
					cout<<(*this)[i,j,k];
				}
				cout<<'\n';
			}
		}
	}
};