#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CColliderTriangle.h"

// コンストラクタ
CCollider::CCollider(CObjectBase* owner, ELayer layer, EColliderType type)
	: mLayer(layer)
	, mType(type)
	, mpOwner(owner)
	, mIsEnable(true)
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

bool CCollider::CollisionTriangleLine(CColliderTriangle* t, CColliderLine* l, CVector* a)
{
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	CMatrix tm = t->Matrix();
	v[0] = t->V(0) * tm;
	v[1] = t->V(1) * tm;
	v[2] = t->V(2) * tm;
	CMatrix lm = l->Matrix();
	sv = l->V(0) * lm;
	ev = l->V(1) * lm;
	//面の法線を、外積を正規化して求める
	CVector normal = CVector::Cross(v[1] - v[0], v[2] - v[0]).Normalized();
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v[0];
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v[0];
	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f) {
		//衝突してない（調整不要）
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している
	//面と線分の交点を求める
	//交点の計算
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	//頂点1頂点2ベクトルと頂点1交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点2頂点3ベクトルと頂点2交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//課題３２
	//頂点3頂点1ベクトルと頂点3交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//調整値計算（衝突しない位置まで戻す）
	if (dots < 0.0f) {
		//始点が裏面
		*a = normal * -dots;
	}
	else {
		//終点が裏面
		*a = normal * -dote;
	}
	return true;
}

bool CCollider::CollisionTriangleSphere(CColliderTriangle* t, CColliderSphere* s, CVector* a)
{
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	CMatrix tm = t->Matrix();
	v[0] = t->V(0) * tm;
	v[1] = t->V(1) * tm;
	v[2] = t->V(2) * tm;
	//面の法線を、外積を正規化して求める
	CVector normal = CVector::Cross(v[1] - v[0], v[2] - v[0]).Normalized();
	//線コライダをワールド座標で作成
	CMatrix sm = s->Matrix();
	sv = normal * sm * s->Radius();
	ev = -normal * sm * s->Radius();
	CColliderLine line(nullptr, ELayer::eNone, sv, ev);
	//三角コライダと線コライダの衝突処理
	return CollisionTriangleLine(t, &line, a);
}

bool CCollider::CollisionSphere(CColliderSphere* sphere1, CColliderSphere* sphere2, CVector* adjust)
{
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×親の変換行列
	CVector pos1 = sphere1->Matrix().Position();
	CVector pos2 = sphere2->Matrix().Position();
	//中心から中心へのベクトルを求める
	CVector vec = pos1 - pos2;
	float length = vec.Length();
	//中心の距離が半径の合計より小さいと衝突
	float sum = sphere1->Radius() + sphere2->Radius();
	if (sum > length) {
		*adjust = pos2 + vec.Normalized() * sum;
		//衝突している
		return  true;
	}

	*adjust = CVector(0.0f, 0.0f, 0.0f);
	//衝突していない
	return false;
}

bool CCollider::CollisionSphereLine(CColliderSphere* sphere, CColliderLine* line, CVector* adjust)
{
	CVector spherePos = sphere->Matrix().Position();
	CMatrix lm = line->Matrix();
	CVector lineStartPos = line->V(0) * lm;
	CVector lineEndPos = line->V(1) * lm;

	CVector nearest;
	float length = CalcDistancePointToLine(spherePos, lineStartPos, lineEndPos, &nearest);
	if (length < sphere->Radius())
	{
		*adjust = nearest + (spherePos - nearest).Normalized() * sphere->Radius();
		return true;
	}

	*adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}

bool CCollider::CollisionLine(CColliderLine* line1, CColliderLine* line2, CVector* adjust)
{
	//TODO:調整値の対応
	*adjust = CVector(0.0f, 0.0f, 0.0f);

	CMatrix lm1 = line1->Matrix();
	CVector S1 = line1->V(0) * lm1;
	CVector E1 = line1->V(1) * lm1;
	CMatrix lm2 = line2->Matrix();
	CVector S2 = line2->V(0) * lm2;
	CVector E2 = line2->V(1) * lm2;

	CVector S1E1 = E1 - S1;
	CVector S2E2 = E2 - S2;
	CVector CD = CVector::Cross(E1 - S1, E2 - S2).Normalized();

	CVector S1S2 = S2 - S1;
	CVector S1E2 = E2 - S1;
	CVector S2S1 = S1 - S2;
	CVector S2E1 = E1 - S2;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
	}
	else
	{
		float length1 = CalcDistancePointToLine(S1, S2, E2);
		float length2 = CalcDistancePointToLine(E1, S2, E2);
		float length3 = CalcDistancePointToLine(S2, S1, E1);
		float length4 = CalcDistancePointToLine(E2, S1, E1);
		length = fminf(fminf(length1, length2), fminf(length3, length4));
	}

	if (length < line1->Radius() + line2->Radius())
	{
		return true;
	}

	return false;
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

bool CCollider::Collision(CCollider* c0, CCollider* c1, CVector* adjust)
{
	switch (c0->Type())
	{
		case EColliderType::eLine:
			switch (c1->Type())
			{
			case EColliderType::eLine:
				return CollisionLine
				(
					dynamic_cast<CColliderLine*>(c0),
					dynamic_cast<CColliderLine*>(c1),
					adjust
				);
			case EColliderType::eSphere:
				return CollisionSphereLine
				(
					dynamic_cast<CColliderSphere*>(c1),
					dynamic_cast<CColliderLine*>(c0),
					adjust
				);
			case EColliderType::eTriangle:
				return CollisionTriangleLine
				(
					dynamic_cast<CColliderTriangle*>(c1),
					dynamic_cast<CColliderLine*>(c0),
					adjust
				);
			}
			break;
		case EColliderType::eSphere:
			switch (c1->Type())
			{
			case EColliderType::eLine:
				return CollisionSphereLine
				(
					dynamic_cast<CColliderSphere*>(c0),
					dynamic_cast<CColliderLine*>(c1),
					adjust
				);
			case EColliderType::eSphere:
				return CollisionSphere
				(
					dynamic_cast<CColliderSphere*>(c1),
					dynamic_cast<CColliderSphere*>(c0),
					adjust
				);
			case EColliderType::eTriangle:
				return CollisionTriangleSphere
				(
					dynamic_cast<CColliderTriangle*>(c1),
					dynamic_cast<CColliderSphere*>(c0),
					adjust
				);
			}
			break;
		case EColliderType::eTriangle:
			switch (c1->Type())
			{
			case EColliderType::eLine:
				return CollisionTriangleLine
				(
					dynamic_cast<CColliderTriangle*>(c0),
					dynamic_cast<CColliderLine*>(c1),
					adjust
				);
			case EColliderType::eSphere:
				return CollisionTriangleSphere
				(
					dynamic_cast<CColliderTriangle*>(c0),
					dynamic_cast<CColliderSphere*>(c1),
					adjust
				);
			case EColliderType::eTriangle:
				//return CollisionTriangle
				//(
				//	dynamic_cast<CColliderTriangle*>(c0),
				//	dynamic_cast<CColliderTriangle*>(c1),
				//	adjust
				//);
				break;
			}
			break;
	}
	return false;
}
