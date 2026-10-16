using NumType=Mint;
using SegNode=NumType;// SegNodeIden = 0
using SegMerge=plus<NumType>;
using SegLazy=NumType;// SegLazyIden = 1
using SegLazyMerge=multiplies<NumType>;
using SegMapping=multiplies<NumType>;
using SetLazySegTree=LazySegTree<SegNode,SegMerge,SegLazy,SegLazyMerge,SegMapping>;