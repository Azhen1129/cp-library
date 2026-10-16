namespace Tree {
	/*---------- 邻接项适配：E = int（无权）或 pair<int, W>（带权） ----------*/
	template<typename E>
	int EdgeDest(const E&e) {// 邻接项里的邻居
		if constexpr(std::is_constructible_v<E,int>&&std::is_convertible_v<E,int>) return (int)e;
		else return (int)e.first;
	}
	template<typename E>
	auto EdgeWeight(const E&e) {// 邻接项里的边权；E = int 时是编译期常量 1，不占内存
		if constexpr(std::is_constructible_v<E,int>&&std::is_convertible_v<E,int>) return 1;
		else return e.second;
	}
	template<typename E>
	constexpr bool Unweighted=std::is_constructible_v<E,int>&&std::is_convertible_v<E,int>;
	template<typename E>
	constexpr bool Weighted=!Unweighted<E>;
	constexpr int RMQ_MAX_N=300000;// 写 0 强制 倍增表 ，写 无穷大 强制 欧拉序 + 稀疏表
	namespace detail {// 内部原语：三类共用，别当接口用
		template<typename E>
		void DFSCore(const CSR<E>&G,int root,vector<int>&par,vector<int>&pre) {// 生成 父亲表 + 先序表
			const int N=G.nrows();
			par.assign(N,-1); pre.clear(); pre.reserve(N);
			if(!N) return;
			vector<int> stk; stk.reserve(N); stk.push_back(root); par[root]=root;
			while(!stk.empty()) {
				int u=stk.back(); stk.pop_back(); pre.push_back(u);
				auto nb=G.neighbors(u);
				rrep(i,(int)nb.size()-1,-1) {
					int v=EdgeDest(nb[i]);
					if(par[v]!=-1) continue;
					par[v]=u; stk.push_back(v);
				}
			}
			par[root]=-1;
		}
		inline void DepCore(const vector<int>&par,const vector<int>&pre,vector<int>&dep) {// O(N) 生成 深度表
			const int N=(int)par.size(); dep.assign(N,0);
			rep(i,1,N) dep[pre[i]]=dep[par[pre[i]]]+1;
		}
		inline void SzCore(const vector<int>&par,const vector<int>&pre,vector<int>&sz) {// O(N) 生成 子树大小表
			const int N=(int)par.size(); sz.assign(N,1);
			rrep(i,N-1,0) sz[par[pre[i]]]+=sz[pre[i]];
		}
		inline void PosCore(const vector<int>&pre,vector<int>&pos) {// O(N) 生成 逆先序表（pos[v] = 先序下标）
			const int N=(int)pre.size(); pos.assign(N,-1);
			rep(i,0,N) pos[pre[i]]=i;
		}
		template<typename E>
		void PostCore(const CSR<E>&G,int root,const vector<int>&par,vector<int>&post) {// O(N) 生成 后序表
			const int N=G.nrows();
			post.clear(); post.reserve(N);
			if(!N) return;
			vector<int> stk; stk.reserve(N); stk.push_back(root);
			while(!stk.empty()) {
				int u=stk.back(); stk.pop_back();
				if(u<0) { post.push_back(~u); continue; }
				stk.push_back(~u);
				auto nb=G.neighbors(u);
				rrep(i,(int)nb.size()-1,-1) {
					int v=EdgeDest(nb[i]);
					if(par[v]==u) stk.push_back(v);
				}
			}
		}
		template<typename E,typename W>
		void WdCore(const CSR<E>&G,const vector<int>&par,const vector<int>&pre,vector<W>&wd) {// O(N) 生成 父边累加权距表
			const int N=(int)par.size(); wd.assign(N,W{});
			rep(i,1,N) {
				int v=pre[i],p=par[v];
				for(const E&e:G.neighbors(v)) if(EdgeDest(e)==p) {
					wd[v]=wd[p]+EdgeWeight(e); break;
				}
			}
		}
		template<typename E>
		void EulerCore(const CSR<E>&G,int root,const vector<int>&par,vector<int>&euler,vector<int>&first) {// O(N) 生成 欧拉序表 + 第一次出现表
			const int N=G.nrows(); euler.clear(); first.assign(N,-1);
			if(!N) return;
			euler.reserve(2*N-1); vector<int> stk; stk.reserve(2*N-1); stk.push_back(root);
			while(!stk.empty()) {
				int x=stk.back(); stk.pop_back();
				if(x<0) { euler.push_back(~x); continue; }
				if(first[x]==-1) first[x]=(int)euler.size();
				euler.push_back(x);
				auto nb=G.neighbors(x);
				rrep(i,(int)nb.size()-1,-1) {
					int v=EdgeDest(nb[i]);
					if(par[v]==x) { stk.push_back(~x); stk.push_back(v); }
				}
			}
		}
		struct EulerRMQ {
			RaggedArray<int> st;
			vector<int> lg;
			void build(span<const int> ev,const vector<int>&dep) {// O(N log N) 建 稀疏表
				const int L=(int)ev.size(); lg.assign(L+1,0);
				int K=0;
				rep(i,1,L+1) { if((2<<K)<=i) K++; lg[i]=K; }
				int tot=0;
				rep(k,0,K+1) if(L-(1<<k)+1>0) tot+=L-(1<<k)+1;
				st.reserve(tot,K+1);
				if(!L) return;
				st.push_row(ev);
				rep(k,1,K+1) {
					int half=1<<(k-1),len=L-(1<<k)+1;
					if(len<=0) break;
					st.push_row();
					rep(i,0,len) {
						int a=st[k-1,i],b=st[k-1,i+half];
						st.push_back(dep[a]<=dep[b]?a:b);
					}
				}
			}
			int query(int l,int r,const vector<int>&dep) const {// O(1) 查询窗口内深度最小的顶点
				int k=lg[r-l+1]; int a=st[k,l],b=st[k,r-(1<<k)+1];
				return dep[a]<=dep[b]?a:b;
			}
		};
		inline void UpCore(const vector<int>&par,vector<int>&up,int&up_levels) {// O(N log N) 建 倍增表
			const int N=(int)par.size();
			up_levels=1;
			while((1<<up_levels)<N) up_levels++;
			up.assign((size_t)N*up_levels,0);
			rep(v,0,N) up[v*up_levels]=(par[v]==-1?v:par[v]);// 根的祖先指向自己，跳不出去
			rep(j,1,up_levels) rep(v,0,N) up[v*up_levels+j]=up[up[v*up_levels+j-1]*up_levels+j-1];
		}
		struct LCASolver {
			bool use_rmq=false;
			vector<int> first,up;
			int up_levels=0;
			EulerRMQ rmq;
			template<typename E>
			void build(const CSR<E>&G,int root,const vector<int>&par,const vector<int>&dep) {// O(N log N) 按档位建 RMQ 或 LIFT
				use_rmq=G.nrows()<=RMQ_MAX_N;
				if(use_rmq) {
					vector<int>euler;
					EulerCore<E>(G,root,par,euler,first);
					rmq.build(euler,dep);
				}
				else UpCore(par,up,up_levels);
			}
			int query(const vector<int>&dep,int u,int v) const {// RMQ O(1) / LIFT O(log N) 查询最近公共祖先
				if(use_rmq) {
					int l=first[u],r=first[v];
					if(l>r) swap(l,r);
					return rmq.query(l,r,dep);
				}
				if(dep[u]<dep[v]) swap(u,v);
				int k=dep[u]-dep[v];
				rep(j,0,up_levels) if(k>>j&1) u=up[u*up_levels+j];
				if(u==v) return u;
				rrep(j,up_levels-1,-1) if(up[u*up_levels+j]!=up[v*up_levels+j]) u=up[u*up_levels+j],v=up[v*up_levels+j];
				return up[u*up_levels];
			}
		};
	}
	// Unrooted / Rooted 契约：只存 const CSR<E>*（寿命须长于视图），不保存其内部缓冲；
	// 故内部缓冲重分配无害，但视图存活期间不得改内容（clear() + 重填、加行 / 加边） —— 缓存会静默错。
	template<typename E>
	struct Unrooted {// 无根树：只放与根无关的量
		using W=decltype(EdgeWeight(declval<const E&>()));
		const CSR<E>*G;
		int N;
		explicit Unrooted(const CSR<E>&_G):G(&_G),N(_G.nrows()) {}
		Unrooted(CSR<E>&&)=delete;// 防绑临时量
		Unrooted(const Unrooted&)=delete;// 防误拷整套缓存
		Unrooted&operator=(const Unrooted&)=delete;
		Unrooted(Unrooted&&)=default;
		Unrooted&operator=(Unrooted&&)=default;
		pair<int,int> centroids() const { build_cent(); return cent; }// 重心
		W diameter() const { build_dia(); return dia; }// 直径长（要求边权非负）
		pair<int,int> diameter_ends() const { build_dia(); return {dia_a,dia_b}; }// 一条直径的两端
		W dist(int u,int v) const {// 两点距离
			build_lca(); int w=lca_solver.query(dep,u,v);
			if constexpr(Unweighted<E>) { return (W)(dep[u]+dep[v]-2*dep[w]); }
			else { build_wd(); return wd[u]+wd[v]-2*wd[w]; }
		}
	private:
		/*---------- 组 1：私有根 0 的 par / pre（重心、距离共用） ----------*/
		mutable bool ok_build=false;
		mutable vector<int> par,pre;
		void build() const { if(ok_build) return; ok_build=true; detail::DFSCore<E>(*G,0,par,pre); }
		/*---------- 组 2：重心 ----------*/
		mutable bool ok_cent=false;
		mutable vector<int> sz;
		mutable pair<int,int> cent{-1,-1};
		void build_cent() const {
			if(ok_cent) return;
			ok_cent=true; build(); detail::SzCore(par,pre,sz);
			cent={-1,-1};
			rep(v,0,N) {
				int best=N-sz[v];
				for(const E&e:G->neighbors(v)) {
					int c=EdgeDest(e);
					if(par[c]==v) best=max(best,sz[c]);
				}
				if(best*2<=N) {
					if(cent.first==-1) cent.first=v;
					else cent.second=v;
				}
			}
		}
		/*---------- 组 3：直径 ----------*/
		mutable bool ok_dia=false;
		mutable W dia{};
		mutable int dia_a=-1,dia_b=-1;
		void build_dia() const {
			if(ok_dia) return;
			ok_dia=true; dia_a=dia_b=-1; if(!N) return;
			vector<W> d(N,W{});
			auto sweep=[&](int s,int&far)->W {
				W best=W{};
				far=s;
				d[s]=W{};
				vector<pair<int,int>> stk{{s,-1}};
				while(!stk.empty()) {
					auto [u,p]=stk.back();
					stk.pop_back();
					if(d[u]>best) best=d[u],far=u;
					for(const E&e:G->neighbors(u)) {
						int v=EdgeDest(e);
						if(v==p) continue;
						d[v]=d[u]+EdgeWeight(e);
						stk.push_back({v,u});
					}
				}
				return best;
			};
			sweep(0,dia_a);
			dia=sweep(dia_a,dia_b);
		}
		/*---------- 组 4：距离 ----------*/
		mutable bool ok_dep=false,ok_wd=false,ok_lca=false;
		mutable vector<int> dep;
		mutable vector<W> wd;
		mutable detail::LCASolver lca_solver;
		void build_dep() const { if(ok_dep) return; ok_dep=true; build(); detail::DepCore(par,pre,dep); }
		void build_wd() const { if(ok_wd) return; ok_wd=true; build(); detail::WdCore<E,W>(*G,par,pre,wd); }
		void build_lca() const { if(ok_lca) return; ok_lca=true; build_dep(); lca_solver.build<E>(*G,0,par,dep); }
	};
	template<typename E>
	struct Rooted {// 有根树：依赖根的量都在这
		const CSR<E>*G;
		int N;
		int root_id;
		Rooted(const CSR<E>&_G,int _root):G(&_G),N(_G.nrows()),root_id(_root) {}
		Rooted(CSR<E>&&,int)=delete;// 防绑临时量
		Rooted(const Rooted&)=delete;// 防误拷整套缓存
		Rooted&operator=(const Rooted&)=delete;
		Rooted(Rooted&&)=default;
		Rooted&operator=(Rooted&&)=default;
		int root() const { return root_id; }// 根节点
		int parent(int v) const { build(); return par[v]; }// 父亲节点
		const vector<int>&parents() const { build(); return par; }// 父亲表
		const vector<int>&preorder() const { build(); return pre; }// 先序表
		int depth(int v) const { build_dep(); return dep[v]; }// 深度
		const vector<int>&depths() const { build_dep(); return dep; }// 深度表
		int subtree_size(int v) const { build_sz(); return sz[v]; }// 子树点数
		const vector<int>&subtree_sizes() const { build_sz(); return sz; }// 子树大小表
		int tin(int v) const { build_pos(); return pos[v]; }// 先序下标
		const vector<int>&tins() const { build_pos(); return pos; }// 先序下标表
		span<const int> subtree(int v) const { build_sz(); build_pos(); return span<const int>(pre.data()+pos[v],sz[v]); }// 子树点集
		const vector<int>&postorder() const { build_post(); return post; }// 后序表
		const vector<int>&level_order() const { build_lev(); return lev; }// 层序表
		int lca(int u,int v) const { build_lca(); return lca_solver.query(dep,u,v); }// 查询最近公共祖先
	private:
		/*---------- 组 1：par / pre ----------*/
		mutable bool ok_build=false;
		mutable vector<int> par,pre;
		void build() const { if(ok_build) return; ok_build=true; detail::DFSCore<E>(*G,root_id,par,pre); }
		/*---------- 组 2：dep ----------*/
		mutable bool ok_dep=false;
		mutable vector<int> dep;
		void build_dep() const { if(ok_dep) return; ok_dep=true; build(); detail::DepCore(par,pre,dep); }
		/*---------- 组 3：sz / pos（tin(v) 返回 pos[v]） ----------*/
		mutable bool ok_sz=false,ok_pos=false;
		mutable vector<int> sz,pos;
		void build_sz() const { if(ok_sz) return; ok_sz=true; build(); detail::SzCore(par,pre,sz); }
		void build_pos() const { if(ok_pos) return; ok_pos=true; build(); detail::PosCore(pre,pos); }
		/*---------- 组 4：post ----------*/
		mutable bool ok_post=false;
		mutable vector<int> post;
		void build_post() const { if(ok_post) return; ok_post=true; build(); detail::PostCore<E>(*G,root_id,par,post); }
		/*---------- 组 5：lev（BFS；靠 par 认孩子） ----------*/
		mutable bool ok_lev=false;
		mutable vector<int> lev;
		void build_lev() const {
			if(ok_lev) return;
			ok_lev=true; build(); if(!N) return;
			lev.reserve(N); lev.push_back(root_id);
			rep(i,0,(int)lev.size()) {
				int u=lev[i];
				for(const E&e:G->neighbors(u)) {
					int v=EdgeDest(e);
					if(par[v]==u) lev.push_back(v);
				}
			}
		}
		/*---------- 组 6：lca（自己一份构件，与 Unrooted 的不共享） ----------*/
		mutable bool ok_lca=false;
		mutable detail::LCASolver lca_solver;
		void build_lca() const { if(ok_lca) return; ok_lca=true; build_dep(); lca_solver.build<E>(*G,root_id,par,dep); }
	};
	struct Bin {// 二叉树
		span<const int> L,R;
		int N;
		Bin(span<const int> _L,span<const int> _R):L(_L),R(_R),N((int)_L.size()) {}
		Bin(span<const int> _L,span<const int> _R,int _root):L(_L),R(_R),N((int)_L.size()),ok_root(true),root_id(_root) {}
		Bin(const Bin&)=delete;// 防误拷整套缓存
		Bin&operator=(const Bin&)=delete;
		Bin(Bin&&)=default;
		Bin&operator=(Bin&&)=default;
		int root() const { if(!ok_root) build_root(); return root_id; }// 根节点
		const vector<int>&preorder() const { build(); return pre; }// 先序表
		const vector<int>&inorder() const { build(); return in; }// 中序表
		const vector<int>&postorder() const { build(); return post; }// 后序表
		int subtree_size(int v) const { build(); return sz[v]; }// 子树点数
		const vector<int>&level_order() const {// 层序表
			if(ok_lev) return lev;
			ok_lev=true; if(!N||root()==-1) return lev;
			lev.reserve(N); lev.push_back(root_id);
			rep(i,0,(int)lev.size()) {
				int u=lev[i];
				if(L[u]!=-1) lev.push_back(L[u]);
				if(R[u]!=-1) lev.push_back(R[u]);
			}
			return lev;
		}
	private:
		mutable bool ok_root=false;
		mutable int root_id=-1;
		mutable bool ok_build=false,ok_lev=false;
		mutable vector<int> pre,in,post,sz,lev;
		void build_root() const {
			ok_root=true; root_id=-1; vector<int> indeg(N,0);
			rep(v,0,N) {
				if(L[v]!=-1) indeg[L[v]]++;
				if(R[v]!=-1) indeg[R[v]]++;
			}
			bool bad=false;
			rep(v,0,N) {
				if(indeg[v]==0) {
					if(root_id!=-1) bad=true;
					else root_id=v;
				}
				else if(indeg[v]>1) bad=true;
			}
			if(bad) root_id=-1;
		}
		void build() const {
			if(ok_build) return;
			ok_build=true; sz.assign(N,1); if(!N||root()==-1) return;
			vector<pair<int,int>> stk; stk.reserve(N); stk.push_back({root_id,0});
			while(!stk.empty()) {
				auto [u,state]=stk.back();
				stk.pop_back();
				if(state==0) {
					pre.push_back(u);
					stk.push_back({u,1});
					if(L[u]!=-1) stk.push_back({L[u],0});
				}
				else if(state==1) {
					in.push_back(u);
					stk.push_back({u,2});
					if(R[u]!=-1) stk.push_back({R[u],0});
				}
				else {
					post.push_back(u);
					sz[u]=1+(L[u]==-1?0:sz[L[u]])+(R[u]==-1?0:sz[R[u]]);
				}
			}
		}
	};
		/*---------- 已删接口（要用时从这里捞回去实现） ----------*/
	// Unrooted：neighbors(v) 写 G->neighbors(v)；n() / m() 写 G->nrows() / G->nvals()/2；度数 / isLeaf；
	//           is_tree(G)（构造前的本地防线，提交前删调用）；release()；rooted_at(r) 写 Rooted<E> R(*G,r)；
	//           dist_from(u)；path(u,v) / kth_on_path / midpoint；center()（直径中点，1 或 2 个）；
	//           ecc() / radius()（偏心距 / 半径 = 换根 DP，先不搞）
	// Rooted  ：is_ancestor(u,v) 写 lca(u,v)==u；dist_to_root(v)：无权写 depth(v)，带权接 WdCore 加 wd（别转发 Unrooted：临时对象各自建缓存）；
	//           kth_ancestor(v,k)；path_to_root(v)；children(v) / levels()（按层分组：每层一串点）；
	//           Rooted(U,r) 重载：写 Rooted<E>(*U.G,r)
	// Bin     ：left(v) / right(v) 写 L[v] / R[v]；is_valid(L,R)；from_general(g,root,L,R)（左孩子右兄弟；中序 == 原树后根）；
	//           tin / depth / height / is_ancestor（同一趟里顺手能出，先不加）
	// 推后整块：Forest（父数组 / 多根 / 虚拟根）、csrFromParents
	/*---------- 挂起：已评估、未决（别当已定；要用时从这里捞） ----------*/
	// LIFT→HLD（大 N 档）：LCASolver 的 else 换 HLD（重儿子优先），删 UpCore / up；加 hvy / head（8N B；
	//   重儿子靠 sz：Rooted 复用自带 sz，Unrooted 临时 +4N 用完即弃）；查仍 O(logN)；1e6 时 up ≈ 80 MB/份 → 8N B；
	//   也是 jump 的地基——要做 jump 必须先换。
	// valid()：DFSCore 记 vnum 比 N（+4 B/对象）；抓「根错 / 不连通」；抓不出环，单向行多半照过。
	// height()：max(dep) 缓存（+4 B）；一行级，用到再加。
	// 删拷贝：三视图类删拷贝构 / 赋、显式 =default 移动；防 auto x=R / lambda [=] 静默拷整套缓存。（本次如做了就删本行）
	// HLD 坐标系（重儿子优先 dfn）＋ jump / on_path / path_ranges：三件共享同一「坐标二选一」；
	//   jump 链内定位也要坐标；槽位形状自拟，不抄固定版。
	// reset()：clear 不 shrink + 标志复位；T 大 N 小时才值；落地时全库「不 clear」规约要重审。
	// 多根 / Forest：见上「推后整块」行。
	// farthest / path_between：不建对象的自由函数；farthest 可用 max(dist(u,a),dist(u,b)) 顶（a,b = 直径两端，最远点必为直径端点之一）；
	//   path_between 与已删块 path(u,v) 二选一，别做两套。
	// Centroid（点分树，≠ centroids()）、Virtual（虚树）、分块 RMQ（±1 分块：O(N) 建、≈8N B、O(1) 查）。
}
using namespace Tree;