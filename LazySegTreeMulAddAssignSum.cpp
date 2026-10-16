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
template<typename L>
struct MulAddLazyNode {
	L mul;
	L add;
	MulAddLazyNode():mul(1),add(0) {}
	MulAddLazyNode(L _mul,L _add):mul(_mul),add(_add) {}
	bool operator==(const MulAddLazyNode<L>&o) const { return mul==o.mul&&add==o.add; }
};
template<typename L>
struct MulAddLazyMerge {
	using Node=MulAddLazyNode<L>;
	Node operator()(const Node&A,const Node&B) const {
		return {A.mul*B.mul,A.add*B.mul+B.add};
	}
};
template<typename T,typename L>
struct MulAddMapping {
	T operator()(const T&A,const L&B) const {
		return {A.sum*B.mul+B.add*A.len,A.len};
	}
};
using NumType=Mint;
using SegNode=AddNode<NumType>;// SegNodeIden = SegNode{}
using SegMerge=AddMerge<NumType>;
using SegLazy=MulAddLazyNode<NumType>;// SegLazyNode = SegLazy{}
using SegLazyMerge=MulAddLazyMerge<NumType>;
using SegMapping=MulAddMapping<SegNode,SegLazy>;
using SetLazySegTree=LazySegTree<SegNode,SegMerge,SegLazy,SegLazyMerge,SegMapping>;
/*
	区间加	{1, val}
	区间乘	{val, 0}
	区间赋	{0, val}
*/