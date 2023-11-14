#include "Primitive.h"
#include <glut.h>
#include "CMatrix.h"
#include "Maths.h"

// 線分を描画（3D）
void Primitive::DrawLine(const CVector& s, const CVector& e, const CColor& color, float lineWidth)
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	glLineWidth(lineWidth);
	// 線分を描画
	glBegin(GL_LINES);
	glVertex3f(s.X(), s.Y(), s.Z());
	glVertex3f(e.X(), e.Y(), e.Z());
	glEnd();
	glLineWidth(1.0f);

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_BLEND);

	// 描画前の行列に戻す
	glPopMatrix();
}

// 線分を描画（2D）
void Primitive::DrawLine2D(const CVector2& s, const CVector2& e, const CColor& color, float lineWidth)
{
}

// 球を描画
void Primitive::DrawSphere(const CMatrix& m, float rad, const CColor& color)
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// 描画行列を反映
	glMultMatrixf(m.M());

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	float col[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glColor4fv(col);

	const int cut = 32;
	CVector* vertex = new CVector[cut * cut * 4];
	float* s = new float[cut + 1];
	float* c = new float[cut + 1];
	for (int i = 0; i <= cut; ++i)
	{
		s[i] = sinf((float)M_PI * 2 * i / cut) * rad;
		c[i] = cosf((float)M_PI * 2 * i / cut) * rad;
	}

	int idx = 0;
	for (int i = 0; i < cut; ++i)
	{
		const float& is1 = s[i];
		const float& is2 = s[i + 1];
		const float& ic1 = c[i];
		const float& ic2 = c[i + 1];
		for (int j = 0; j < cut; ++j)
		{
			const float& js1 = s[j];
			const float& js2 = s[j + 1];
			const float& jc1 = c[j];
			const float& jc2 = c[j + 1];

			vertex[idx] = CVector(js1 * is1, ic1, jc1 * is1);
			vertex[idx + 1] = CVector(js2 * is1, ic1, jc2 * is1);
			vertex[idx + 2] = CVector(js2 * is2, ic2, jc2 * is2);
			vertex[idx + 3] = CVector(js1 * is2, ic2, jc1 * is2);
			idx += 4;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_QUADS, 0, cut * cut * 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	delete[] vertex;
	delete[] s;
	delete[] c;

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_BLEND);

	// 描画前の行列に戻す
	glPopMatrix();
}

// ワイヤーフレームの球を描画
void Primitive::DrawWireSphere(const CMatrix& m, float rad, const CColor& color)
{
	glPolygonMode(GL_FRONT, GL_LINE);
	DrawSphere(m, rad, color);
	glPolygonMode(GL_FRONT, GL_FILL);
}

// 三角形を描画
void Primitive::DrawTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& color)
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// 三角形を描画
	glBegin(GL_TRIANGLES);
	glVertex3f(v0.X(), v0.Y(), v0.Z());
	glVertex3f(v1.X(), v1.Y(), v1.Z());
	glVertex3f(v2.X(), v2.Y(), v2.Z());
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_BLEND);

	// 描画前の行列に戻す
	glPopMatrix();
}

// 板ポリゴンを描画
void Primitive::DrawQuad(const CMatrix& m, const CVector2& size, const CColor& color)
{
	// 板ポリゴンの4頂点
	static const CVector v[4] =
	{
		{ -0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f,  0.5f },
		{  0.5f, 0.0f,  0.5f },
		{  0.5f, 0.0f, -0.5f },
	};

	// 現在の行列を退避しておく
	glPushMatrix();

	// 描画行列反映
	glMultMatrixf(m.M());

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// 板ポリゴンを描画
	glBegin(GL_QUADS);
	glVertex3f(v[0].X() * size.X(), v[0].Y(), v[0].Z() * size.Y());
	glVertex3f(v[1].X() * size.X(), v[1].Y(), v[1].Z() * size.Y());
	glVertex3f(v[2].X() * size.X(), v[2].Y(), v[2].Z() * size.Y());
	glVertex3f(v[3].X() * size.X(), v[3].Y(), v[3].Z() * size.Y());
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_BLEND);

	// 描画前の行列に戻す
	glPopMatrix();
}
