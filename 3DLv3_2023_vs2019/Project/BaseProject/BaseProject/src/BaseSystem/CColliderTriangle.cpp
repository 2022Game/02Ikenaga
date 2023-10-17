#include "CColliderTriangle.h"
#include "glut.h"
#include "Maths.h"

// コンストラクタ
CColliderTriangle::CColliderTriangle(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, const CVector& v2)
	: CCollider(owner, layer, EColliderType::eTriangle)
{
	// 三角形の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
}

// コンストラクタ（引数なし版）
CColliderTriangle::CColliderTriangle()
	: CCollider(nullptr, ELayer::eNone, EColliderType::eTriangle)
{
}

// 三角形コライダーの設定
void CColliderTriangle::Set(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, const CVector& v2)
{
	CCollider::Set(owner, layer);

	// 三角形の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
}

// 三角形の頂点を取得
void CColliderTriangle::Get(CVector* v0, CVector* v1, CVector* v2) const
{
	CMatrix m = Matrix();
	*v0 = mV[0] * m;
	*v1 = mV[1] * m;
	*v2 = mV[2] * m;
}

// コライダー描画
void CColliderTriangle::Render()
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
	float c[] = { 1.0f, 0.0f, 0.0f, 0.2f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// 三角形を描画
	glBegin(GL_TRIANGLES);
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}