template<typename T,typename M=plus<T>>
struct SWAG {
	M merge;
	T iden;
	T back;
	deque<T> data;
	int flen;
	SWAG(T _iden,M _merge=M{}):merge(_merge),iden(_iden),back(_iden),flen(0) {}
	void push(const T&x) { data.push_back(x); back=merge(back,x); }
	void pop() {
		if(!flen) {
			flen=data.size();
			for(int i=flen-2;i>=0;i--) data[i]=merge(data[i],data[i+1]);
			back=iden;
		}
		data.pop_front();
		flen--;
	}
	T query() const {
		if(data.empty()) return iden;
		if(!flen) return back;
		return merge(data.front(),back);
	}
	int size() const { return data.size(); }
	bool empty() const { return data.empty(); }
	/*
		plus<>			+		0	string 是 ""
	 	multiplies<>	*		1
	 	bit_and<>		&		~0
	 	bit_or<>		|		0
	 	bit_xor<>		^		0
	 	logical_and<> 	&&		true
	 	logical_or<> 	||		false
	*/
};