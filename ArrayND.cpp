template<typename T,int R>
struct ArrayND {
	static_assert(R>=1);
	array<int,R> dims{};// 每维长度
	vector<T> data;
	ArrayND()=default;
	template<typename...D> requires (sizeof...(D)==R&&(integral<D>&&...))
	ArrayND(D...d):dims{(int)d...} {
		size_t s=1;
		for(int e:dims) s*=(size_t)e;
		data.assign(s,T{});
	}
	bool operator==(const ArrayND&) const = default;
	template<typename...D> requires (sizeof...(D)==R&&(integral<D>&&...))
	void assign(D...d) { dims={(int)d...}; data.assign(count(dims),T{}); }
	/*---------- 访问 ----------
	 * 给满 R 个坐标 → 元素；给前 m 个坐标 → 后面那一段连续内存（span）
	 * 前缀切片规则，和 2D/3D 同一套。R=3 例：a[i,j,k] 元素 | a[i,j] k 线 | a[i] (j,k) 面
	 * 别写 a[i][j]：那是“上一段里的第 j 个值”，和 a[i,j] 不是一回事
	 */
	template<typename...I> requires (sizeof...(I)<=R&&(integral<I>&&...))
	decltype(auto) operator[](I...idx) {
		constexpr int m=sizeof...(I);
		int a[R]{(int)idx...};// 缺的坐标补 0
		int off=flatten(a);
		if constexpr (m==R) return data[off];
		else return span<T>(data.data()+off,block_len(m));
	}
	template<typename...I> requires (sizeof...(I)<=R&&(integral<I>&&...))
	decltype(auto) operator[](I...idx) const {
		constexpr int m=sizeof...(I);
		int a[R]{(int)idx...};
		int off=flatten(a);
		if constexpr (m==R) return data[off];
		else return span<const T>(data.data()+off,block_len(m));
	}
	/*---------- 其他 ----------*/
	template<typename...I> requires (sizeof...(I)==R&&(integral<I>&&...))
	int ID(I...idx) const { int a[R]{(int)idx...}; return flatten(a); }// 坐标 → 扁平下标
	array<int,R> invID(int q) const {// 扁平下标 → 坐标
		array<int,R> res{};
		rrep(r,R-1,-1) { res[r]=q%dims[r]; q/=dims[r]; }
		return res;
	}
	int nvals() const { return (int)data.size(); }
	void fill(const T&val) { ranges::fill(data,val); }
	T all_max() const { return *ranges::max_element(data); }
	T all_min() const { return *ranges::min_element(data); }
	/*---------- R 维前缀和（1-idx + 0 边界层，原地破坏性） ----------
	* dims[d] = n_d+1，数据写在 1..n_d；只能调一次；build 后本对象即前缀表
	* 预处理 O(R・nvals)，查询 2^R 次查表
	*/
	void build_prefix() {
		int sz=(int)data.size();
		if(!sz) return;
		int stride[R];
		stride[R-1]=1;
		rrep(d,R-2,-1) stride[d]=stride[d+1]*dims[d+1];
		rep(d,0,R) {// 先把各轴坐标 0 的层清零：边界层必须是 0（你不用自己保证）
			int step=stride[d],blk=step*dims[d];
			for(int base=0;base<sz;base+=blk) rep(inner,0,step) data[base+inner]=T{};
		}
		rep(d,0,R) {// 沿每根轴做一维前缀和，R 趟
			int step=stride[d],blk=step*dims[d];
			for(int base=0;base<sz;base+=blk) rep(inner,0,step) rep(c,1,dims[d]) data[base+c*step+inner]+=data[base+(c-1)*step+inner];
		}
	}
	template<typename...I> requires (sizeof...(I)==2*R&&(integral<I>&&...))
	T box_sum(I...idx) const {// (lo0,hi0,lo1,hi1,...)，1-idx 闭区间
		int a[2*R]{(int)idx...};
		int stride[R];
		stride[R-1]=1;
		rrep(d,R-2,-1) stride[d]=stride[d+1]*dims[d+1];
		T res{};
		rep(m,0,1<<R) {// 2^R 个角点：bit d = 1 表示该轴取 lo_d-1
			int off=0;
			rep(d,0,R) off+=(m>>d&1?a[2*d]-1:a[2*d+1])*stride[d];
			T term=data[off];
			res+=(popcount((unsigned)m)&1?-term:term);
		}
		return res;
	}
	private:
	static size_t count(const array<int,R>&d) {// 总元素数
		size_t s=1;
		for(int e:d) s*=(size_t)e;
		return s;
	}
	int flatten(const int(&a)[R]) const {// 坐标 → 扁平下标（Horner）
		int off=0;
		rep(r,0,R) off=off*dims[r]+a[r];
		return off;
	}
	int block_len(int m) const {// 从第 m 维起的总长度
		int len=1;
		rep(r,m,R) len*=dims[r];
		return len;
	}
};