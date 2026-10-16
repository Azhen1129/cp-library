template<typename T>
struct AddNode {
	T sum;
	int len;
	AddNode():sum(T{}),len(1) {}
	AddNode(T _sum,int _len):sum(_sum),len(_len) {}
};
template<typename T>
struct AddMerge {
	using Node=AddNode<T>;
	Node operator()(const Node&A,const Node&B) const {
		return {A.sum+B.sum,A.len+B.len};
	}
};
template<typename T,typename L>
struct AddMapping {
	T operator()(const T&A,const L&B) const {
		return {A.sum+B*A.len,A.len};
	}
};
using NumType=ll;
using SegNode=AddNode<NumType>;// SegNodeIden = SegNode{}
using SegMerge=AddMerge<NumType>;
using SegLazy=NumType;// SegLazyIden = 0
using SegLazyMerge=plus<NumType>;
using SegMapping=AddMapping<SegNode,SegLazy>;
using SetLazySegTree=LazySegTree<SegNode,SegMerge,SegLazy,SegLazyMerge,SegMapping>;