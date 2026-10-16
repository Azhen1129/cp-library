namespace Median {
	template<typename C>
	struct Reverse {// 比较器取反
		C cmp;
		Reverse(C _cmp=C{}):cmp(_cmp) {}
		template<typename T>
		bool operator()(const T&a,const T&b) const { return cmp(b,a); }
	};
	template<typename T,typename C=less<T>>
	struct Heap {// 对顶堆（只增）
        C cmp;
		priority_queue<T,vector<T>,C> left;
		priority_queue<T,vector<T>,Reverse<C>> right;
		Heap(C _cmp=C{}):cmp(_cmp),left(_cmp),right(Reverse<C>(_cmp)) {}
		void push(const T&x) {
			left.push(x);
			right.push(left.top());
			left.pop();
			if(left.size()<right.size()) {
				left.push(right.top()); right.pop();
			}
		}
		const T&median() const { return left.top(); }// 下中位数
		const T&median_upper() const { return left.size()==right.size()?right.top():left.top(); }// 上中位数
		template<typename R,typename F>
		R median_combine(F f) const {
			if(left.size()==right.size()) return (R)f(left.top(),right.top());
			return (R)left.top();
		}
		int size() const { return left.size()+right.size(); }
		bool empty() const { return left.empty(); }
        void clear() {
			left=priority_queue<T,vector<T>,C>(cmp);
			right=priority_queue<T,vector<T>,Reverse<C>>(Reverse<C>(cmp));
		}
	};
	template<typename T,typename C=less<T>>
	struct Tree {// 对顶 multiset（可删）
		C cmp;
		multiset<T,C> left;
		multiset<T,C> right;
		Tree(C _cmp=C{}):cmp(_cmp),left(_cmp),right(_cmp) {}
		void push(const T&x) {
			left.insert(x);
			right.insert(*left.rbegin());
			left.erase(prev(left.end()));
			if(left.size()<right.size()) {
				left.insert(*right.begin());
				right.erase(right.begin());
			}
		}
		bool erase(const T&val) {// 删除失败（不存在）返回 false
			if(!left.empty()&&!cmp(*left.rbegin(),val)) {
				auto it=left.find(val);
				if(it==left.end()) return false;
				left.erase(it);
			}
			else {
				auto it=right.find(val);
				if(it==right.end()) return false;
				right.erase(it);
			}
			if(left.size()<right.size()) {
				left.insert(*right.begin()); right.erase(right.begin());
			}
			else if(right.size()+1<left.size()) {
				right.insert(*left.rbegin()); left.erase(prev(left.end()));
			}
			return true;
		}
		const T&median() const { return *left.rbegin(); }// 下中位数
		const T&median_upper() const { return left.size()==right.size()?*right.begin():*left.rbegin(); }// 上中位数
		template<typename R,typename F>
		R median_combine(F f) const {
			if(left.size()==right.size()) return (R)f(*left.rbegin(),*right.begin());
			return (R)*left.rbegin();
		}
		int size() const { return left.size()+right.size(); }
		bool empty() const { return left.empty(); }
        void clear() { left.clear(); right.clear(); }
	};
}
using namespace Median;