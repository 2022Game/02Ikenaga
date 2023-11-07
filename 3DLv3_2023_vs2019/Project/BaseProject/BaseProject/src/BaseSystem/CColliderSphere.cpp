#include "CColliderSphere.h"
#include <glut.h>

// コンストラクタ
CColliderSphere::CColliderSphere(CObjectBase* owner, ELayer layer, float radius)
	: CCollider(owner, layer, EColliderType::eSphere)
	, mRadius(radius)
{
}

// 球コライダーの設定
void CColliderSphere::Set(CObjectBase* owner, ELayer layer, float radius)
{
	CCollider::Set(owner, layer);

	// 半径を設定
	mRadius = radius;
}

// 球の座標と半径を取得
void CColliderSphere::Get(CVector* pos, float* rad) const
{
	CMatrix m = Matrix();
	*pos = Position() * m;
	*rad = mRadius * m.VectorX().Length();
}

void CColliderSphere::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// 自身の行列を適用
	CMatrix m;
	m.Translate(Position());
	glMultMatrixf((m * Matrix()).M());
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

	// 球を描画
	glutWireSphere(mRadius, 32, 32);

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}
