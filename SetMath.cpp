template<typename T1,typename T2,typename C=less<>>
T1 SetAnd(const T1&A,const T2&B,C comp=C{}) {// 返回 A ∩ B（交集）
	T1 res;
	set_intersection(all(A),all(B),inserter(res,res.end()),comp);
	return res;
}
template<typename T1,typename T2,typename C=less<>>
T1 SetOr(const T1&A,const T2&B,C comp=C{}) {// 返回 A ∪ B（并集）
	T1 res;
	set_union(all(A),all(B),inserter(res,res.end()),comp);
	return res;
}
template<typename T1,typename T2,typename C=less<>>
T1 SetDiff(const T1&A,const T2&B,C comp=C{}) {// 返回 A ∖ B（差集：属于 A 但不属于 B）
	T1 res;
	set_difference(all(A),all(B),inserter(res,res.end()),comp);
	return res;
}
template<typename T1,typename T2,typename C=less<>>
T1 SetXor(const T1&A,const T2&B,C comp=C{}) {// 返回 A △ B（对称差：只属于 A 或只属于 B）
	T1 res;
	set_symmetric_difference(all(A),all(B),inserter(res,res.end()),comp);
	return res;
}
template<typename T1,typename T2,typename C=less<>>
bool IsSubset(const T1&A,const T2&B,C comp=C{}) {// 返回 A ⊆ B（A 是 B 的子集；按多重集语义）
	return includes(all(B),all(A),comp);
}
template<typename T1,typename T2,typename C=less<>>
bool IsSuperset(const T1&A,const T2&B,C comp=C{}) {// 返回 A ⊇ B（A 是 B 的超集；按多重集语义）
	return includes(all(A),all(B),comp);
}