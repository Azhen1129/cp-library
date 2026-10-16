namespace TopK {
	template<typename T,typename C=less<T>>
	struct Heap {// TopK 专属的二叉堆
		int K;
		C cmp;
		vector<T> data;
		Heap(int _K,C _cmp=C{}):K(_K),cmp(_cmp),data() {}
		void sift_up(int u) {
			while(u) {
				int p=(u-1)>>1;
				if(!cmp(data[p],data[u])) return;
				swap(data[u],data[p]);
				u=p;
			}
		}
		void sift_down(int u) {
			int N=(int)data.size();
			while(true) {
				int v=u<<1|1;
				if(v>=N) return;
				if(v+1<N&&cmp(data[v],data[v+1])) v++;
				if(!cmp(data[u],data[v])) return;
				swap(data[u],data[v]);
				u=v;
			}
		}
		bool add(const T&x) {// O(logK)，底层是 vector，不要用到 SegTree 之类的
			if((int)data.size()<K) {
				data.push_back(x);
				sift_up((int)data.size()-1);
				return true;
			}
			if(!cmp(x,data[0])) return false;
			data[0]=x;
			sift_down(0);
			return true;
		}
		const T&threshold() const { return data[0]; }// 当前保留集里最差者（第 data.size() 好）
		bool full() const { return size()==K; }
		int size() const { return data.size(); }
		bool empty() const { return data.empty(); }
		void clear() { data.clear(); }
		vector<T> sorted() const {
			vector<T> res=data;
			sort(all(res),[&](const T&a,const T&b) { return cmp(a,b); });
			return res;
		}
	};
	template<typename T,typename C=less<T>>
	struct Array {// 有序数组（运行期 K），底层是 vector，不要用到 SegTree 之类的
		int K;
		C cmp;
		vector<T> data;
		Array(int _K,C _cmp=C{}):K(_K),cmp(_cmp),data() {}
		bool add(const T&x) {// O(K)，插入成功 ? return true : return false
			if((int)data.size()>=K&&!cmp(x,data.back())) return false;
			int pos=(int)(partition_point(all(data),[&](const T&a) { return cmp(a,x); })-data.begin());// 首个“不先于 x”的元素处
			data.insert(data.begin()+pos,x);
			if((int)data.size()>K) data.pop_back();
			return true;
		}
		const T&operator[](int idx) const { return data[idx]; }
		const T&threshold() const { return data.back(); }// 当前保留集里最差者（第 data.size() 好）
		bool full() const { return size()==K; }
		int size() const { return data.size(); }
		bool empty() const { return data.empty(); }
		void clear() { data.clear(); }
		vector<T> sorted() const { return data; }
	};
	template<typename T,int K,typename C=less<T>>
	struct FixedArray {// 有序数组（编译期 K），底层是 array，适合当 SegTree 之类的节点
		int cnt;
		C cmp;
		array<T,K> data;
		FixedArray(C _cmp=C{}):cnt(0),cmp(_cmp),data() {}
		bool add(const T&x) {// O(K)，插入成功 ? return true : return false
			if(cnt>=K&&!cmp(x,data[cnt-1])) return false;
			int pos=0;
			while(pos<cnt&&cmp(data[pos],x)) pos++;
			if(cnt<K) cnt++;
			rrep(i,cnt-1,pos) data[i]=data[i-1];
			data[pos]=x;
			return true;
		}
		const T&operator[](int idx) const { return data[idx]; }
		const T&threshold() const { return data[cnt-1]; }// 当前保留集里最差者（第 cnt 好）
		bool full() const { return cnt==K; }
		int size() const { return cnt; }
		bool empty() const { return cnt==0; }
		void clear() { cnt=0; }
		vector<T> sorted() const { return vector<T>(data.begin(),data.begin()+cnt); }
	};
	template<typename T,typename C=less<T>>
	struct HeapMerge {// 容量取 min(A.K, B.K)
		using Node=Heap<T,C>;
		Node operator()(const Node&A,const Node&B) const {
			Node res(min(A.K,B.K),A.cmp);
			for(const T&x:A.data) res.add(x);
			for(const T&x:B.data) res.add(x);
			return res;
		}
	};
	template<typename T,typename C=less<T>>
	struct ArrayMerge {// 容量取 min(A.K, B.K)
		using Node=Array<T,C>;
		Node operator()(const Node&A,const Node&B) const {
			Node res(min(A.K,B.K),A.cmp);
			int i=0,j=0,na=(int)A.data.size(),nb=(int)B.data.size();
			while((int)res.data.size()<res.K&&(i<na||j<nb)) {
				if(j>=nb||(i<na&&!A.cmp(B.data[j],A.data[i]))) res.data.push_back(A.data[i++]);
				else res.data.push_back(B.data[j++]);
			}
			return res;
		}
	};
	template<typename T,int K,typename C=less<T>>
	struct FixedArrayMerge {
		using Node=FixedArray<T,K,C>;
		Node operator()(const Node&A,const Node&B) const {
			Node res(A.cmp);
			int i=0,j=0;
			while(res.cnt<K&&(i<A.cnt||j<B.cnt)) {
				if(j>=B.cnt||(i<A.cnt&&!A.cmp(B.data[j],A.data[i]))) res.data[res.cnt++]=A.data[i++];
				else res.data[res.cnt++]=B.data[j++];
			}
			return res;
		}
	};
}
using namespace TopK;