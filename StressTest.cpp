// ==================== 数据生成 ====================
//"cout<<"换成"oss<<"
string gen() {
	ostringstream oss;
	ostream& cout=oss;
	//
	
	//
	return oss.str();
}
// ==================== 暴力正解 ====================
//"cin>>"换成"iss>>"，"cout<<"换成"oss<<"
string brute(const string&input) {
	istringstream iss(input);
	ostringstream oss;
	istream& cin=iss;
	ostream& cout=oss;
	//
	
	//
	return oss.str();
}
// ==================== 待测算法 ====================
//"cin>>"换成"iss>>"，"cout<<"换成"oss<<"
string solve(const string&input) {
	istringstream iss(input);
	ostringstream oss;
	istream& cin=iss;
	ostream& cout=oss;
	//
	
	//
	return oss.str();
}
int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int T=500;
	for(int i=1;i<=T;i++) {
		string input=gen();
		string o1=brute(input);
		string o2=solve(input);
		if(o1!=o2) {
			cout<<"WA on test "<<i<<'\n';
			cout<<"input : "<<'\n'<<input;
			cout<<"brute : "<<'\n'<<o1;
			cout<<"solve : "<<'\n'<<o2;
			return 1;
		}
	}
	cout<<"AC"<<'\n';
	return 0;
}