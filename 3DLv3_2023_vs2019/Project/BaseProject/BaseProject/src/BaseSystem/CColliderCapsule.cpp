#include "CColliderCapsule.h"
#include <glut.h>
#include "Maths.h"
#include "CColor.h"

// コンストラクタ
CColliderCapsule::CColliderCapsule(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, float radius,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eCapsule, isKinematic, weight)
	, mRadius(radius)
{
	// カプセルを構成する線分の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
}

// カプセルコライダーの設定
void CColliderCapsule::Set(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, float radius)
{
	CCollider::Set(owner, layer);

	// カプセルを構成する線分の頂点を設定
	mV[0] = v0;
	mV[1] = v1;

	// 半径を設定
	mRadius = radius;
}

// カプセルを構成する線分の視点と終点を取得
void CColliderCapsule::Get(CVector* v0, CVector* v1) const
{
	*v0 = mWV[0];
	*v1 = mWV[1];
}

// カプセルの半径を取得
float CColliderCapsule::Radius() const
{
	return mRadius;
}

// コライダー描画
void CColliderCapsule::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// 自身の行列を適用
	glMultMatrixf(Matrix().M());
	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	CColor col = CColor::red;
	if (!IsEnable()) col = CColor::gray;
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	glLineWidth(3.0f);
	// 線分を描画
	glBegin(GL_LINES);
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());
	glEnd();
	glLineWidth(1.0f);

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダーの情報を更新
void CColliderCapsule::UpdateCol()
{
	// 頂点のワールド座標を算出
	CMatrix m = Matrix();
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;

	// バウンディングボックスを更新
	mBounds = CBounds::GetCapsuleBounds(mWV[0], mWV[1], mRadius);
}
