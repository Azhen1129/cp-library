template<typename T>// 负无穷
struct Max {
	T operator()(const T&a,const T&b) const { return a>b?a:b; }
};
template<typename T>// 正无穷
struct Min {
	T operator()(const T&a,const T&b) const { return a<b?a:b; }
};
template<typename T>// 0
struct GCD {
	T operator()(const T&a,const T&b) const { return gcd(a,b); }
};
template<typename T>// 1
struct LCM {
	T operator()(const T&a,const T&b) const { return lcm(a,b); }
};