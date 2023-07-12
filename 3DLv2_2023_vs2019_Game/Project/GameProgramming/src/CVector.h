#ifndef CVECTOR_H
#define CVECTOR_H
class CMatrix;

/*
 ベクトルクラス
 ベクトルデータを扱います
*/
class CVector {
public:
	//正規化
	//大きさ1のベクトルを返す
	CVector Normalize() const;
	//外積
	//Cross(ベクトル)
	CVector Cross(const CVector& v) const;

	//内積
	//Dot(ベクトル)
	float Dot(const CVector& v) const;

	//ベクトルの長さを返す
	float Length() const;
	//各軸での値の設定
	//Set(X座標, Y座標, Z座標)
	void Set(float x, float y, float z);
	//Xの値を得る
	float X() const;
	//Xの値を設定
	void X(float x);
	//Yの値を得る
	float Y() const;
	//Yの値を設定
	void Y(float y);
	//Zの値を得る
	float Z() const;
	//Zの値を設定
	void Z(float z);
	//デフォルトコンストラクタ
	CVector();
	//コンストラクタ
	//CVector(X座標, Y座標, Z座標)
	CVector(float x, float y, float z);

	//比較演算子
	//CVector == CVector の結果を返す
	bool operator==(const CVector& v) const;
	//CVector != CVector の結果を返す
	bool operator!=(const CVector& v) const;

	//+演算子のオーバーロード
	//CVector + CVector の演算結果を返す
	CVector operator+(const CVector& v) const;
	//CVector += CVector で演算結果を自身に代入
	void operator+=(const CVector& v);

	//-演算子のオーバーロード
	//CVector - CVector の演算結果を返す
	CVector operator-(const CVector& v) const;
	//CVector -= CVector で演算結果を自身に代入
	void operator-=(const CVector& v);

	//*演算子のオーバーロード
	//CVector * float の演算結果を返す
	CVector operator*(const float& f) const;
	//CVector *= float で演算結果を自身に代入
	void operator*=(const float& f);
	//CVector * CMatrixの結果をCVectorで返す
	CVector operator*(const CMatrix& m);

private:
	//3D各軸での値を設定
	float mX, mY, mZ;
};

//ベクトルクラス（2次元）
class CVector2
{
public:
	//正規化
	//大きさ1のベクトルを返す
	CVector2 Normalize() const;

	//外積
	//Cross(ベクトル)
	float Cross(const CVector2& v) const;

	//内積
	//Dot(ベクトル)
	float Dot(const CVector2& v) const;

	//ベクトルの長さを返す
	float Length() const;
	//各軸での値の設定
	//Set(X座標, Y座標)
	void Set(float x, float y);
	//Xの値を得る
	float X() const;
	//Xの値を設定
	void X(float x);
	//Yの値を得る
	float Y() const;
	//Yの値を設定
	void Y(float y);
	//デフォルトコンストラクタ
	CVector2();
	//コンストラクタ
	//CVector2(X座標, Y座標)
	CVector2(float x, float y);

	//比較演算子
	//CVector2 == CVector2 の結果を返す
	bool operator==(const CVector2& v) const;
	//CVector2 != CVector2 の結果を返す
	bool operator!=(const CVector2& v) const;

	//+演算子のオーバーロード
	//CVector2 + CVector2 の演算結果を返す
	CVector2 operator+(const CVector2& v) const;
	//CVector2 += CVector2 で演算結果を自身に代入
	void operator+=(const CVector2& v);

	//-演算子のオーバーロード
	//CVector2 - CVector2 の演算結果を返す
	CVector2 operator-(const CVector2& v) const;
	//CVector2 -= CVector2 で演算結果を自身に代入
	void operator-=(const CVector2& v);

	//*演算子のオーバーロード
	//CVector2 * float の演算結果を返す
	CVector2 operator*(const float& f) const;
	//CVector2 *= float で演算結果を自身に代入
	void operator*=(const float& f);

private:
	//2D各軸での値を設定
	float mX, mY;
};

//ベクトルクラス（4次元）
class CVector4
{
public:
	//各軸での値の設定
	//Set(X座標, Y座標, Z座標, W)
	void Set(float x, float y, float z, float w);
	//Set(X座標, Y座標, Z座標)
	void Set(float x, float y, float z);
	//Xの値を得る
	float X() const;
	//Xの値を設定
	void X(float x);
	//Yの値を得る
	float Y() const;
	//Yの値を設定
	void Y(float y);
	//Zの値を得る
	float Z() const;
	//Zの値を設定
	void Z(float z);
	//Wの値を得る
	float W() const;
	//Wの値を設定
	void W(float w);

	//デフォルトコンストラクタ
	CVector4();
	//コンストラクタ
	//CVector(X座標, Y座標, Z座標, W)
	CVector4(float x, float y, float z, float w);
	CVector4(float x, float y, float z);
	CVector4(const CVector& v);

	//代入演算子
	//CVector4 = CVector でCVectorの内容を自身に代入
	void operator=(const CVector& v);

	//比較演算子
	//CVector4 == CVector4 の結果を返す
	bool operator==(const CVector4& v) const;
	//CVector4 != CVector4 の結果を返す
	bool operator!=(const CVector4& v) const;

	//+演算子のオーバーロード
	//CVector4 + CVector4 の演算結果を返す
	CVector4 operator+(const CVector4& v) const;
	//CVector4 += CVector4 で演算結果を自身に代入
	void operator+=(const CVector4& v);


	//-演算子のオーバーロード
	//CVector4 - CVector4 の演算結果を返す
	CVector4 operator-(const CVector4& v) const;
	//CVector4 -= CVector4 で演算結果を自身に代入
	void operator-=(const CVector4& v);

	//*演算子のオーバーロード
	//CVector4 * float の演算結果を返す
	CVector4 operator*(const float& f) const;
	//CVector4 *= float で演算結果を自身に代入
	void operator*=(const float& f);

private:
	//4Dでの値を設定
	float mX, mY, mZ, mW;
};
#endif
