#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CObjectBase.h"
#include "Maths.h"

// コンストラクタ
CCollider::CCollider(CObjectBase* owner, ELayer layer, EColliderType type,
	bool isKinematic, float weight)
	: mLayer(layer)
	, mType(type)
	, mpOwner(owner)
	, mIsEnable(true)
	, mIsKinematic(isKinematic)
	, mWeight(weight)
	, mCollisionLayers(~0)
	, mCollisionTags(~0)
{
	// コリジョンリストに追加
	CCollisionManager::Instance()->Add(this);
}

// デストラクタ
CCollider::~CCollider()
{
	// コリジョンリストから削除
	CCollisionManager::Instance()->Remove(this);
}

// 衝突判定レイヤーを取得
ELayer CCollider::Layer() const
{
	return mLayer;
}

// コライダーの種類を取得
EColliderType CCollider::Type() const
{
	return mType;
}

// コライダーの持ち主を返す
CObjectBase* CCollider::Owner() const
{
	return mpOwner;
}

// コライダーの持ち主のタグを取得
ETag CCollider::Tag() const
{
	// 持ち主が存在すれば、持ち主のタグを返す
	if (mpOwner == nullptr) return ETag::eNone;
	return mpOwner->Tag();
}

// コライダーが有効かどうかを設定
void CCollider::SetEnable(bool isEnable)
{
	mIsEnable = isEnable;
}

// コライダーが有効かどうかを取得
bool CCollider::IsEnable() const
{
	return mIsEnable;
}

// 衝突時の押し戻しの影響を受けるかどうかを設定
void CCollider::SetKinematic(bool iskinematic)
{
	mIsKinematic = iskinematic;
}

// 衝突時の押し戻しの影響を受けるかどうか
bool CCollider::IsKinematic() const
{
	return mIsKinematic;
}

// コライダーの重量を設定
void CCollider::SetWeight(float weight)
{
	mWeight = weight;
}

// コライダーの重量を取得
float CCollider::GetWeight() const
{
	return mWeight;
}

// 指定したコライダーと衝突判定を行うかどうかを取得
bool CCollider::IsCollision(CCollider* col) const
{
	// 自分自身もしくは相手のコライダーが有効でなければ、
	// 衝突判定を行わない
	if (col == nullptr) return false;
	if (!col->IsEnable()) return false;
	if (!mIsEnable) return false;

	// 同じコライダーもしくは、持ち主が同じであれば、
	// 衝突判定を行わない
	if (col == this) return false;
	if (col->Owner() != nullptr || mpOwner != nullptr)
	{
		if (col->Owner() == mpOwner) return false;
	}

	// 衝突判定を行うレイヤーか
	if (!IsCollisionLayer(col->Layer())) return false;
	// 衝突判定を行うオブジェクトタグか
	if (!IsCollisionTag(col->Tag())) return false;

	// 全ての条件を満たした場合は衝突判定を行う
	return true;
}

// 衝突判定を行うレイヤーを設定
void CCollider::SetCollisionLayers(Layers layers)
{
	mCollisionLayers = 0;
	for (auto&& layer : layers)
	{
		mCollisionLayers |= 1 << (int)layer;
	}
}

// 指定したレイヤーとの衝突判定を行うかどうかを設定
void CCollider::SetCollisionLayer(ELayer layer, bool isCollision)
{
	if (isCollision) mCollisionLayers |= 1 << (int)layer;
	else mCollisionLayers &= ~(1 << (int)layer);
}

// 指定したレイヤーと衝突判定を行うかどうかを取得
bool CCollider::IsCollisionLayer(ELayer layer) const
{
	return (mCollisionLayers & 1 << (int)layer) != 0;
}

// 衝突判定を行うオブジェクトタグを設定
void CCollider::SetCollisionTags(Tags tags)
{
	mCollisionTags = 0;
	for (auto&& tag : tags)
	{
		mCollisionTags |= 1 << (int)tag;
	}
}

// 指定したオブジェクトタグとの衝突判定を行うかどうかを設定
void CCollider::SetCollisionTag(ETag tag, bool isCollision)
{
	if (isCollision) mCollisionTags |= 1 << (int)tag;
	else mCollisionTags &= ~(1 << (int)tag);
}

// 指定したオブジェクトタグと衝突判定を行うかどうかを取得
bool CCollider::IsCollisionTag(ETag tag) const
{
	return (mCollisionTags & 1 << (int)tag) != 0;
}

// 行列を取得
CMatrix CCollider::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	if (mpOwner != nullptr)
	{
		m = mpOwner->Matrix() * m;
	}
	return m;
}

// コライダーの設定
void CCollider::Set(CObjectBase* owner, ELayer layer)
{
	mpOwner = owner;
	mLayer = layer;
}

// 三角形と三角形の衝突判定
bool CCollider::CollisionTriangle(	const CVector& t00, const CVector& t01, const CVector& t02,
									const CVector& t10, const CVector& t11, const CVector& t12,
									CHitInfo* hit)
{
	return false;
}

// 三角形と線分の衝突判定
bool CCollider::CollisionTriangleLine(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& ls, const CVector& le, CHitInfo* h)
{
	//面の法線を、外積を正規化して求める
	CVector normal = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = ls - t0;
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = le - t0;
	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f) {
		//衝突してない（調整不要）
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している
	//面と線分の交点を求める
	//交点の計算
	CVector cross = ls + (le - ls) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	//頂点1頂点2ベクトルと頂点1交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((t1 - t0).Cross(cross - t0).Dot(normal) < 0.0f) {
		//衝突してない
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点2頂点3ベクトルと頂点2交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((t2 - t1).Cross(cross - t1).Dot(normal) < 0.0f) {
		//衝突してない
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//課題３２
	//頂点3頂点1ベクトルと頂点3交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((t0 - t2).Cross(cross - t2).Dot(normal) < 0.0f) {
		//衝突してない
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//調整値計算（衝突しない位置まで戻す）
	if (dots < 0.0f) {
		//始点が裏面
		h->adjust = normal * dots;
	}
	else {
		//終点が裏面
		h->adjust = normal * dote;
	}
	return true;
}

// 三角形と球の衝突判定
bool CCollider::CollisionTriangleSphere(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& sp, const float sr, CHitInfo* h)
{
	//CVector v[3], sv, ev;
	////各コライダの頂点をワールド座標へ変換
	//CMatrix tm = t->Matrix();
	//v[0] = t->V(0) * tm;
	//v[1] = t->V(1) * tm;
	//v[2] = t->V(2) * tm;
	////面の法線を、外積を正規化して求める
	//CVector normal = CVector::Cross(v[1] - v[0], v[2] - v[0]).Normalized();
	////線コライダをワールド座標で作成
	//CMatrix sm = s->Matrix();
	//sv = normal * sm * s->Radius();
	//ev = -normal * sm * s->Radius();
	//CColliderLine line(nullptr, ELayer::eNone, sv, ev);
	////三角コライダと線コライダの衝突処理
	//return CollisionTriangleLine(t, &line, h);
	return false;
}

// 球と球の衝突判定
bool CCollider::CollisionSphere(const CVector& sp0, const float sr0,
								const CVector& sp1, const float sr1, CHitInfo* hit)
{
	//中心から中心へのベクトルを求める
	CVector vec = sp0 - sp1;
	float length = vec.Length();
	//中心の距離が半径の合計より小さいと衝突
	float sum = sr0 + sr1;
	if (sum > length) {
		hit->adjust = vec.Normalized() * (sum - length);
		//衝突している
		return  true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	//衝突していない
	return false;
}

// 球と線分の衝突判定
bool CCollider::CollisionSphereLine(const CVector& sp, const float sr,
									const CVector& ls, const CVector& le, CHitInfo* hit)
{
	CVector nearest;
	float length = CalcDistancePointToLine(sp, ls, le, &nearest);
	if (length < sr)
	{
		hit->adjust = (sp - nearest).Normalized() * (sr - length);
		return true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}
// 線分と線分の衝突判定
bool CCollider::CollisionLine(	const CVector& ls0, const CVector& le0,
								const CVector& ls1, const CVector& le1, CHitInfo* hit)
{
	//TODO:調整値の対応
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector S1E1 = le0 - ls0;
	CVector S2E2 = le1 - ls1;
	CVector CD = CVector::Cross(le0 - ls0, le1 - ls1).Normalized();

	CVector S1S2 = ls1 - ls0;
	CVector S1E2 = le1 - ls0;
	CVector S2S1 = ls0 - ls1;
	CVector S2E1 = le0 - ls1;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
	}
	else
	{
		float length1 = CalcDistancePointToLine(ls0, ls1, le1);
		float length2 = CalcDistancePointToLine(le0, ls1, le1);
		float length3 = CalcDistancePointToLine(ls1, ls0, le0);
		float length4 = CalcDistancePointToLine(le1, ls0, le0);
		length = fminf(fminf(length1, length2), fminf(length3, length4));
	}

	if (length < 0.1f + 0.1f)
	{
		return true;
	}

	return false;
}

// メッシュと線分の衝突判定
bool CCollider::CollisionMeshLine(	const std::list<STVertex>& tris,
									const CVector& ls, const CVector& le, CHitInfo* hit)
{
	bool ret = false;
	CVector adjust;
	for (auto& v : tris)
	{
		if (CollisionTriangleLine(v.V[0], v.V[1], v.V[2], ls, le, hit)) 
		{
			hit->tris.push_back(v);
			adjust = hit->adjust;
			ret = true;
		}
	}
	hit->adjust = adjust;
	return ret;
}

// メッシュと球の衝突判定
bool CCollider::CollisionMeshSpehre(const std::list<STVertex>& tris,
									const CVector& sp, const float sr, CHitInfo* hit)
{
	bool ret = false;
	for (auto& v : tris)
	{
		if (CollisionTriangleSphere(v.V[0], v.V[1], v.V[2], sp, sr, hit))
		{
			hit->tris.push_back(v);
			ret = true;
		}
	}
	return ret;
}

// メッシュと三角形の衝突判定
bool CCollider::CollisionMeshTriangle(	const std::list<STVertex>& tris,
										const CVector& t0, const CVector& t1, const CVector& t2,
										CHitInfo* hit)
{
	bool ret = false;
	for (auto& v : tris)
	{
		if (CollisionTriangle(v.V[0], v.V[1], v.V[2], t0, t1, t2, hit))
		{
			hit->tris.push_back(v);
			ret = true;
		}
	}
	return ret;
}

float CCollider::CalcDistancePointToLine(const CVector& point, const CVector& lineS, const CVector& lineE, CVector* nearest)
{
	CVector SE = lineE - lineS;
	float t = (point - lineS).Dot(SE.Normalized());

	CVector C;
	if (t < 0) C = lineS;
	else if (t > SE.Length()) C = lineE;
	else
	{
		C = lineS + SE.Normalized() * t;
	}

	if (nearest != nullptr) *nearest = C;

	return (C - point).Length();
}

bool CCollider::Collision(CCollider* c0, CCollider* c1, CHitInfo* hit)
{
	switch (c0->Type())
	{
		case EColliderType::eLine:
		{
			CColliderLine* line0 = dynamic_cast<CColliderLine*>(c0);
			CVector ls0, le0;
			line0->Get(&ls0, &le0);

			switch (c1->Type())
			{
				case EColliderType::eLine:
				{
					CColliderLine* line1 = dynamic_cast<CColliderLine*>(c1);
					CVector ls1, le1;
					line1->Get(&ls1, &le1);
					return CollisionLine(ls0, le0, ls1, le1, hit);
				}
				case EColliderType::eSphere:
				{
					CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
					CVector sp;
					float sr;
					sphere->Get(&sp, &sr);
					return CollisionSphereLine(sp, sr, ls0, le0, hit);
				}
				case EColliderType::eTriangle:
				{
					CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
					CVector t0, t1, t2;
					triangle->Get(&t0, &t1, &t2);
					return CollisionTriangleLine(t0, t1, t2, ls0, le0, hit);
				}
				case EColliderType::eMesh:
				{
					CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
					std::list<STVertex> tris;
					mesh->Get(&tris);
					return CollisionMeshLine(tris, ls0, le0, hit);
				}
			}
			break;
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere0 = dynamic_cast<CColliderSphere*>(c0);
			CVector sp0;
			float sr0;
			sphere0->Get(&sp0, &sr0);

			switch (c1->Type())
			{
				case EColliderType::eLine:
				{
					CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
					CVector ls, le;
					line->Get(&ls, &le);
					return CollisionSphereLine(sp0, sr0, ls, le, hit);
				}
				case EColliderType::eSphere:
				{
					CColliderSphere* sphere1 = dynamic_cast<CColliderSphere*>(c1);
					CVector sp1;
					float sr1;
					sphere1->Get(&sp1, &sr1);
					return CollisionSphere(sp0, sr0, sp1, sr1, hit);
				}
				case EColliderType::eTriangle:
				{
					CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
					CVector t0, t1, t2;
					triangle->Get(&t0, &t1, &t2);
					return CollisionTriangleSphere(t0, t1, t2, sp0, sr0, hit);
				}
				case EColliderType::eMesh:
				{
					CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
					std::list<STVertex> tris;
					mesh->Get(&tris);
					return CollisionMeshSpehre(tris, sp0, sr0, hit);
				}
			}
			break;
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle0 = dynamic_cast<CColliderTriangle*>(c0);
			CVector t00, t01, t02;
			triangle0->Get(&t00, &t01, &t02);
			switch (c1->Type())
			{
				case EColliderType::eLine:
				{
					CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
					CVector ls, le;
					line->Get(&ls, &le);
					return CollisionTriangleLine(t00, t01, t02, ls, le, hit);
				}
				case EColliderType::eSphere:
				{
					CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
					CVector sp;
					float sr;
					sphere->Get(&sp, &sr);
					return CollisionTriangleSphere(t00, t01, t02, sp, sr, hit);
				}
				case EColliderType::eTriangle:
				{
					CColliderTriangle* triangle1 = dynamic_cast<CColliderTriangle*>(c1);
					CVector t10, t11, t12;
					triangle1->Get(&t10, &t11, &t12);
					return CollisionTriangle(t00, t01, t02, t10, t11, t12, hit);
				}
				case EColliderType::eMesh:
				{
					CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
					std::list<STVertex> tris;
					mesh->Get(&tris);
					return CollisionMeshTriangle(tris, t00, t01, t02, hit);
				}
			}
			break;
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c0);
			std::list<STVertex> tris;
			mesh->Get(&tris);
			switch (c1->Type())
			{
				case EColliderType::eLine:
				{
					CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
					CVector ls, le;
					line->Get(&ls, &le);
					return CollisionMeshLine(tris, ls, le, hit);
				}
				case EColliderType::eSphere:
				{
					CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
					CVector sp;
					float sr;
					sphere->Get(&sp, &sr);
					return CollisionMeshSpehre(tris, sp, sr, hit);
				}
				case EColliderType::eTriangle:
				{
					CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
					CVector t0, t1, t2;
					triangle->Get(&t0, &t1, &t2);
					return CollisionMeshTriangle(tris, t0, t1, t2, hit);
				}
				case EColliderType::eMesh:
				{
					// メッシュとメッシュは衝突判定を行わない
					return false;
				}
			}
			break;
		}
	}
	return false;
}

// 衝突時の押し戻し割合を算出
float CCollider::CalcPushBackRatio(CCollider* self, CCollider* other)
{
	// 自身のコライダーが押し戻しの影響を受けない
	if (self->IsKinematic()) return 0.0f;
	// 相手のコライダーが押し戻しの影響を受けない
	if (other->IsKinematic()) return 1.0f;

	// 両方のコライダーが押し戻しの影響を受ける場合は、
	// 両方のコライダーの重量で押し戻し割合を算出
	float sw = self->GetWeight();
	float ow = self->GetWeight();
	// 同じ重量ならば、50%の影響を受ける
	if (sw == ow) return 0.5f;
	// 自身の重量が0ならば、100%の影響を受ける
	if (sw <= 0.0f) return 1.0f;
	// 相手の重量が0ならば、影響は受けない
	if (ow <= 0.0f) return 0.0f;

	// 重量の割合を計算して返す
	return Math::Clamp01((sw + ow) / sw);
}
