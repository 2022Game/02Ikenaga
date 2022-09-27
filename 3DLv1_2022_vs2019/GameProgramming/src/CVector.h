#ifndef CVECTOR_H
#define CVECTOR_H
/*
ベクトルクラス
ベクトルデータを扱います
*/
class CVector {
public:
	//デフォルトコンストラクタ
	CVector();
	//コンストラクタ
	// CVector(X座標,Y座標,Z座標)
	CVector(float x, float y, float z);
	//各軸での値の設定
	//Set(X座標,Y座標,Z座標)
	void Set(float x, float y, float z);
	//Xの値を得る
	float X() const;
	//Yの値を得る
	float Y() const;
	//Zの値を得る
	float Z() const;
private:
	//3D各軸での値の設定
	float mX, mY, mZ;
};
#endif