#include "CVector.h"
#include <math.h>
#include "CMatrix.h"

//------------------------------
// CVector
//------------------------------

float CVector::Dot(const CVector& v) const
{
	return mX * v.mX + mY * v.mY + mZ * v.mZ;
}

float CVector::Length() const
{
	//sqrt関数で平方根を返す
	return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

CVector CVector::Normalize() const {
	//ベクトルの大きさで割ったベクトルを返す（長さ1のベクトル）
	float length = Length();
	if (length == 0.0f) return CVector(0.0f, 0.0f, 0.0f);
	return *this * (1.0f / length);
}

CVector CVector::Cross(const CVector& v) const
{
	return CVector(mY * v.mZ - mZ * v.mY, mZ * v.mX - mX * v.mZ, mX * v.mY - mY * v.mX);
}

//Set(X座標, Y座標, Z座標)
void CVector::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

float CVector::X() const
{
	return mX;
}

void CVector::X(float x)
{
	mX = x;
}

float CVector::Y() const
{
	return mY;
}

void CVector::Y(float y)
{
	mY = y;
}

float CVector::Z() const
{
	return mZ;
}

void CVector::Z(float z)
{
	mZ = z;
}

CVector::CVector()
	: mX(0.0f), mY(0.0f), mZ(0.0f)
{
}

CVector::CVector(float x, float y, float z)
	: mX(x), mY(y), mZ(z)
{
}

bool CVector::operator==(const CVector& v) const
{
	return mX == v.mX && mY == v.mY && mZ == v.mZ;
}

bool CVector::operator!=(const CVector& v) const
{
	return mX != v.mX || mY != v.mY && mZ != v.mZ;
}

CVector CVector::operator+(const CVector& v) const
{
	return CVector(mX + v.mX, mY + v.mY, mZ + v.mZ);
}

void CVector::operator+=(const CVector& v)
{
	mX += v.mX;
	mY += v.mY;
	mZ += v.mZ;
}

CVector CVector::operator-(const CVector& v) const
{
	return CVector(mX - v.mX, mY - v.mY, mZ - v.mZ);
}

void CVector::operator-=(const CVector& v)
{
	mX -= v.mX;
	mY -= v.mY;
	mZ -= v.mZ;
}

CVector CVector::operator*(const float& f) const
{
	return CVector(mX * f, mY * f, mZ * f);
}

void CVector::operator*=(const float& f)
{
	mX *= f;
	mY *= f;
	mZ *= f;
}

CVector CVector::operator*(const CMatrix& m)
{
	//掛け算の結果をCVector型の値で返す
	return CVector(
		mX * m.M(0, 0) + mY * m.M(1, 0) + mZ * m.M(2, 0) + m.M(3, 0),
		mX * m.M(0, 1) + mY * m.M(1, 1) + mZ * m.M(2, 1) + m.M(3, 1),
		mX * m.M(0, 2) + mY * m.M(1, 2) + mZ * m.M(2, 2) + m.M(3, 2)
	);
}


//------------------------------
// CVector2
//------------------------------

float CVector2::Dot(const CVector2& v) const
{
	return mX * v.mX + mY * v.mY;
}

float CVector2::Length() const
{
	//sqrt関数で平方根を返す
	return sqrtf(mX * mX + mY * mY);
}

CVector2 CVector2::Normalize() const {
	//ベクトルの大きさで割ったベクトルを返す（長さ1のベクトル）
	return *this * (1.0f / Length());
}

float CVector2::Cross(const CVector2& v) const
{
	return mX * v.mY - mY * v.mX;
}

//Set(X座標, Y座標)
void CVector2::Set(float x, float y)
{
	mX = x;
	mY = y;
}

float CVector2::X() const
{
	return mX;
}

void CVector2::X(float x)
{
	mX = x;
}

float CVector2::Y() const
{
	return mY;
}

void CVector2::Y(float y)
{
	mY = y;
}

CVector2::CVector2()
	: mX(0.0f), mY(0.0f)
{
}

CVector2::CVector2(float x, float y)
	: mX(x), mY(y)
{
}

bool CVector2::operator==(const CVector2& v) const
{
	return mX == v.mX && mY == v.mY;
}

bool CVector2::operator!=(const CVector2& v) const
{
	return mX != v.mX || mY != v.mY;
}

CVector2 CVector2::operator+(const CVector2& v) const
{
	return CVector2(mX + v.mX, mY + v.mY);
}

void CVector2::operator+=(const CVector2& v)
{
	mX += v.mX;
	mY += v.mY;
}

CVector2 CVector2::operator-(const CVector2& v) const
{
	return CVector2(mX - v.mX, mY - v.mY);
}

void CVector2::operator-=(const CVector2& v)
{
	mX -= v.mX;
	mY -= v.mY;
}

CVector2 CVector2::operator*(const float& f) const
{
	return CVector2(mX * f, mY * f);
}

void CVector2::operator*=(const float& f)
{
	mX *= f;
	mY *= f;
}


//------------------------------
// CVector4
//------------------------------

//Set(X座標, Y座標, Z座標, W)
void CVector4::Set(float x, float y, float z, float w)
{
	mX = x;
	mY = y;
	mZ = z;
	mW = w;
}

//Set(X座標, Y座標, Z座標)
void CVector4::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

float CVector4::X() const
{
	return mX;
}

void CVector4::X(float x)
{
	mX = x;
}

float CVector4::Y() const
{
	return mY;
}

void CVector4::Y(float y)
{
	mY = y;
}

float CVector4::Z() const
{
	return mZ;
}

void CVector4::Z(float z)
{
	mZ = z;
}

float CVector4::W() const
{
	return mW;
}

void CVector4::W(float w)
{
	mW = w;
}

CVector4::CVector4()
	: CVector4(0.0f, 0.0f, 0.0f, 0.0f)
{
}

CVector4::CVector4(float x, float y, float z, float w)
	: mX(x), mY(y), mZ(z), mW(w)
{
}

CVector4::CVector4(float x, float y, float z)
	: CVector4(x, y, z, 0.0f)
{
}

CVector4::CVector4(const CVector& v)
	: CVector4(v.X(), v.Y(), v.Z())
{
}

void CVector4::operator=(const CVector& v)
{
	mX = v.X();
	mY = v.Y();
	mZ = v.Z();
}

bool CVector4::operator==(const CVector4& v) const
{
	return mX == v.mX && mY == v.mY && mZ == v.mZ && mW != v.mW;
}

bool CVector4::operator!=(const CVector4& v) const
{
	return mX != v.mX || mY != v.mY && mZ != v.mZ && mW != v.mW;
}

CVector4 CVector4::operator+(const CVector4& v) const
{
	return CVector4(mX + v.mX, mY + v.mY, mZ + v.mZ, mW + v.mW);
}

void CVector4::operator+=(const CVector4& v)
{
	mX += v.mX;
	mY += v.mY;
	mZ += v.mZ;
	mW += v.mW;
}

CVector4 CVector4::operator-(const CVector4& v) const
{
	return CVector4(mX - v.mX, mY - v.mY, mZ - v.mZ, mW - v.mW);
}

void CVector4::operator-=(const CVector4& v)
{
	mX -= v.mX;
	mY -= v.mY;
	mZ -= v.mZ;
	mW -= v.mW;
}

CVector4 CVector4::operator*(const float& f) const
{
	return CVector4(mX * f, mY * f, mZ * f, mW * f);
}

void CVector4::operator*=(const float& f)
{
	mX *= f;
	mY *= f;
	mZ *= f;
	mW *= f;
}
