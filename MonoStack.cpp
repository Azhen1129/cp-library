namespace MonoStack {
	// greater<T> / less<T> 会跳过等值，要算上等值就传 greater_equal<T> / less_equal<T>
	template<typename T,typename C=less<T>>
	vector<int> NextElement(const vector<T>&A,C comp=C{}) {// 0 - idx，向后找，不存在 -1
		int N=A.size();
		vector<int> res(N,-1);
		vector<int> stk;
		rep(i,0,N) {
			while(!stk.empty()&&comp(A[i],A[stk.back()])) {
				res[stk.back()]=i;
				stk.pop_back();
			}
			stk.push_back(i);
		}
		return res;
	}
	template<typename T,typename C=less<T>>
	vector<int> PrevElement(const vector<T>&A,C comp=C{}) {// 0 - idx，向前找，不存在 -1
		int N=A.size();
		vector<int> res(N,-1);
		vector<int> stk;
		rrep(i,N-1,-1) {
			while(!stk.empty()&&comp(A[i],A[stk.back()])) {
				res[stk.back()]=i;
				stk.pop_back();
			}
			stk.push_back(i);
		}
		return res;
	}
	template<typename T>
	ll LargestRectangle(const vector<T>&H) {// 柱状图最大矩形
		int N=H.size();
		auto L=PrevElement(H,less<T>{});
		auto R=NextElement(H,less<T>{});
		ll res=0;
		rep(i,0,N) {
			int l=L[i]+1;
			int r=(R[i]==-1?N:R[i]);
			res=max(res,(ll)H[i]*(r-l));// [l, r)，l = 左边那堵严格更矮的墙的右邻（无墙则 0），r = 右边那堵墙本身（无墙则 N）
		}
		return res;
	}
	namespace Cartesian {
		// 反向遍历：for(int u:order | std::views::reverse) C++20
		template<typename T,typename C=less<T>>
		vector<int> PostOrder(const vector<T>&A,C comp=C{}) {// 后序数组，反向遍历，即父亲先于所有后代
			int N=A.size();
			vector<int> order;
			order.reserve(N);
			vector<int> stk;
			rep(i,0,N) {
				while(!stk.empty()&&!comp(A[stk.back()],A[i])) {
					order.push_back(stk.back());
					stk.pop_back();
				}
				stk.push_back(i);
			}
			rrep(k,(int)stk.size()-1,-1) order.push_back(stk[k]);
			return order;
		}
		namespace Tree {
			template<typename T,typename C=less<T>>
			vector<int> Parent(const vector<T>&A,C comp=C{}) {// 父数组，根的父亲是 -1
				int N=A.size();
				vector<int> P(N,-1);
				vector<int> stk;
				rep(i,0,N) {
					int last=-1;
					while(!stk.empty()&&!comp(A[stk.back()],A[i])) {
						last=stk.back();
						stk.pop_back();
					}
					if(last!=-1) P[last]=i;
					if(!stk.empty()) P[i]=stk.back();
					stk.push_back(i);
				}
				return P;
			}
			template<typename T,typename C=less<T>>
			pair<vector<int>,vector<int>> ParentAndOrder(const vector<T>&A,C comp=C{}) {// {父, 后序}
				int N=A.size();
				vector<int> P(N,-1);
				vector<int> order;
				order.reserve(N);
				vector<int> stk;
				rep(i,0,N) {
					int last=-1;
					while(!stk.empty()&&!comp(A[stk.back()],A[i])) {
						last=stk.back();
						order.push_back(last);
						stk.pop_back();
					}
					if(last!=-1) P[last]=i;
					if(!stk.empty()) P[i]=stk.back();
					stk.push_back(i);
				}
				rrep(k,(int)stk.size()-1,-1) order.push_back(stk[k]);
				return {P,order};
			}
			template<typename T,typename C=less<T>>
			pair<vector<int>,vector<int>> Children(const vector<T>&A,C comp=C{}) {// {左孩子数组, 右孩子数组}，没有孩子为 -1
				int N=A.size();
				vector<int> L(N,-1),R(N,-1);
				vector<int> stk;
				rep(i,0,N) {
					int last=-1;
					while(!stk.empty()&&!comp(A[stk.back()],A[i])) {
						last=stk.back();
						stk.pop_back();
					}
					L[i]=last;
					if(!stk.empty()) R[stk.back()]=i;
					stk.push_back(i);
				}
				return {L,R};
			}
			template<typename T,typename C=less<T>>
			tuple<vector<int>,vector<int>,vector<int>> ChildrenAndOrder(const vector<T>&A,C comp=C{}) {// {左, 右, 后序}，注意这里是后序，不是先序！
				int N=A.size();
				vector<int> L(N,-1),R(N,-1);
				vector<int> order;
				order.reserve(N);
				vector<int> stk;
				rep(i,0,N) {
					int last=-1;
					while(!stk.empty()&&!comp(A[stk.back()],A[i])) {
						last=stk.back();
						order.push_back(last);
						stk.pop_back();
					}
					L[i]=last;
					if(!stk.empty()) R[stk.back()]=i;
					stk.push_back(i);
				}
				rrep(k,(int)stk.size()-1,-1) order.push_back(stk[k]);
				return {L,R,order};
			}
			pair<vector<int>,vector<int>> ParentToChildren(const vector<int>&P) {// {父} -> {左, 右}
				int N=P.size();
				vector<int> L(N,-1),R(N,-1);
				rep(i,0,N) {
					int p=P[i];
					if(p==-1) continue;
					if(i<p) L[p]=i;
					else R[p]=i;
				}
				return {L,R};
			}
			vector<int> ChildrenToParent(const vector<int>&L,const vector<int>&R) {// {左, 右} -> {父}
				int N=L.size();
				vector<int> P(N,-1);
				rep(i,0,N) {
					if(L[i]!=-1) P[L[i]]=i;
					if(R[i]!=-1) P[R[i]]=i;
				}
				return P;
			}
			vector<int> ChildrenToParent(const pair<vector<int>,vector<int>>&A) {// {左, 右} -> {父}
				return ChildrenToParent(A.first,A.second);
			}
		}
		namespace Forest {// 只能传 greater<> / less<>，不能传 greater_equal<> / less_equal<>
			template<typename T,typename C=less<T>>
			vector<int> Parent(const vector<T>&A,C comp=C{},bool preferLeft=true) {// 父数组，根为 -1，preferLeft 传 false 时，返回的不是森林的规范后序
				int N=A.size();
				auto L=PrevElement(A,comp);
				auto R=NextElement(A,comp);
				vector<int> P(N,-1);
				rep(i,0,N) {
					if(L[i]==-1) P[i]=R[i];
					else if(R[i]==-1) P[i]=L[i];
					else if(!comp(A[L[i]],A[R[i]])&&!comp(A[R[i]],A[L[i]])) P[i]=(preferLeft?L[i]:R[i]);
					else P[i]=(comp(A[L[i]],A[R[i]])?R[i]:L[i]);
				}
				return P;
			}
			template<typename T,typename C=less<T>>
			pair<vector<int>,vector<int>> ParentAndOrder(const vector<T>&A,C comp=C{},bool preferLeft=true) {// {父, 后序}，preferLeft 传 false 时，返回的不是森林的规范后序
				return {Parent(A,comp,preferLeft),PostOrder(A,comp)};
			}
		}
	}
}
using namespace MonoStack;
using namespace MonoStack::Cartesian;