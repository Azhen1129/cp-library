using NumType=int;
using SegNode=NumType;
//using SegMerge=Min<NumType>;// SegNodeIden = INF
//using SegMerge=Max<NumType>;// SegNodeIden = -INF
using SegLazy=NumType;// SegLazyIden = 0
using SegLazyMerge=plus<NumType>;
using SegMapping=plus<NumType>;
using SetLazySegTree=LazySegTree<SegNode,SegMerge,SegLazy,SegLazyMerge,SegMapping>;