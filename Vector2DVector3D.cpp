template<typename T>
struct Vector2D {
	T x,y;
	Vector2D():x(T{}),y(T{}) {}
	Vector2D(T _x,T _y):x(_x),y(_y) {}
	
	bool operator==(const Vector2D&o) const { return x==o.x&&y==o.y; }
	bool operator!=(const Vector2D&o) const { return !(*this==o); }
	bool operator<(const Vector2D&o) const { return tie(x,y)<tie(o.x,o.y); }
	
	Vector2D&operator+=(const Vector2D&o) { x+=o.x; y+=o.y; return *this; }
	Vector2D&operator-=(const Vector2D&o) { x-=o.x; y-=o.y; return *this; }
	Vector2D operator+(const Vector2D&o) const { return Vector2D(*this)+=o; }
	Vector2D operator-(const Vector2D&o) const { return Vector2D(*this)-=o; }
	Vector2D operator-() const { return Vector2D(-x,-y); }
	
	Vector2D&operator*=(T s) { x*=s; y*=s; return *this; }
	Vector2D&operator/=(T s) { x/=s; y/=s; return *this; }
	Vector2D operator*(T s) const { return Vector2D(*this)*=s; }
	Vector2D operator/(T s) const { return Vector2D(*this)/=s; }
	friend Vector2D operator*(T s,const Vector2D&v) { return v*s; }
	
	T operator*(const Vector2D&o) const { return x*o.x+y*o.y; }
	T operator^(const Vector2D&o) const { return x*o.y-y*o.x; }// 加括号！
	
	T len2() const { return x*x+y*y; }
	T len1() const { return sqrt(len2()); }// 注意sqrt的浮点数误差
	
	//---------- 前置 - 极角序拓展包 ----------
//	int pos() const { return quad[(x>=0)+(x>0)][(y>=0)+(y>0)]; }
//	bool same(const Vector2D&o) const { return pos()==o.pos()&&!((*this)^o); }// 没特判 0 向量
};
template<typename T>
struct Vector3D {
	T x,y,z;
	Vector3D():x(T{}),y(T{}),z(T{}) {}
	Vector3D(T _x,T _y,T _z):x(_x),y(_y),z(_z) {}
	
	bool operator==(const Vector3D&o) const { return x==o.x&&y==o.y&&z==o.z; }
	bool operator!=(const Vector3D&o) const { return !(*this==o); }
	bool operator<(const Vector3D&o) const { return tie(x,y,z)<tie(o.x,o.y,o.z); }
	
	Vector3D&operator+=(const Vector3D&o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
	Vector3D&operator-=(const Vector3D&o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
	Vector3D operator+(const Vector3D&o) const { return Vector3D(*this)+=o; }
	Vector3D operator-(const Vector3D&o) const { return Vector3D(*this)-=o; }
	Vector3D operator-() const { return Vector3D(-x,-y,-z); }
	
	Vector3D&operator*=(T s) { x*=s; y*=s; z*=s; return *this; }
	Vector3D&operator/=(T s) { x/=s; y/=s; z/=s; return *this; }
	Vector3D operator*(T s) const { return Vector3D(*this)*=s; }
	Vector3D operator/(T s) const { return Vector3D(*this)/=s; }
	friend Vector3D operator*(T s,const Vector3D&v) { return v*s; }
	
	T operator*(const Vector3D&o) const { return x*o.x+y*o.y+z*o.z; }
	Vector3D operator^(const Vector3D&o) const {
		return Vector3D{y*o.z-z*o.y,z*o.x-x*o.z,x*o.y-y*o.x}; 
	}// 加括号！
	
	T len2() const { return x*x+y*y+z*z; }
};