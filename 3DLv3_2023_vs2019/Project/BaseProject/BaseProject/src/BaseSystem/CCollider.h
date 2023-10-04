#pragma once
#include "CTransform.h"
#include "CollisionLayer.h"
#include "ColliderType.h"
#include "CObjectBase.h"

class CCollisionManager;
class CColliderLine;
class CColliderSphere;
class CColliderTriangle;

/// <summary>
/// コライダーのベースクラス
/// </summary>
class CCollider : public CTransform
{
	friend CCollisionManager;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用のレイヤー</param>
	/// <param name="type">コライダーの種類</param>
	CCollider(CObjectBase* owner, ELayer layer, EColliderType type);
	// デストラクタ
	virtual ~CCollider();

	// 衝突判定レイヤーを取得
	ELayer Layer() const;
	// コライダーの種類を取得
	EColliderType Type() const;
	// コライダーの持ち主を取得
	CObjectBase* Owner() const;
	// コライダーの持ち主のタグを取得
	ETag Tag() const;

	/// <summary>
	/// コライダーが有効かどうかを設定
	/// </summary>
	/// <param name="isEnable">有効かどうか</param>
	void SetEnable(bool isEnable);
	/// <summary>
	/// コライダーが有効かどうかを取得
	/// </summary>
	/// <returns>trueならば有効</returns>
	bool IsEnable() const;

	/// <summary>
	/// 指定したコライダーと衝突判定を行うかどうかを取得
	/// </summary>
	/// <param name="col">衝突判定を行うか確認するコライダー</param>
	/// <returns>trueならば衝突判定を行う</returns>
	bool IsCollision(CCollider* col) const;

	/// <summary>
	/// 衝突判定を行うレイヤーを設定
	/// （設定前の状態はリセットする）
	/// </summary>
	/// <param name="layers">衝突判定を行うレイヤーのリスト</param>
	void SetCollisionLayers(Layers layers);
	/// <summary>
	/// 指定したレイヤーとの衝突判定を行うかどうかを設定
	/// </summary>
	/// <param name="layer">設定するレイヤー</param>
	/// <param name="isCollision">衝突判定を行うかどうか</param>
	void SetCollisionLayer(ELayer layer, bool isCollision);
	/// <summary>
	/// 指定したレイヤーと衝突判定を行うかどうかを取得
	/// </summary>
	/// <param name="layer">判定するレイヤー</param>
	/// <returns>trueならば衝突判定を行う</returns>
	bool IsCollisionLayer(ELayer layer) const;

	/// <summary>
	/// 衝突判定を行うオブジェクトタグを設定
	/// （設定前の状態はリセットする）
	/// </summary>
	/// <param name="tags">衝突判定を行うオブジェクトタグのリスト</param>
	void SetCollisionTags(Tags tags);
	/// <summary>
	/// 指定したオブジェクトタグとの衝突判定を行うかどうかを設定
	/// </summary>
	/// <param name="tag">設定するオブジェクトタグ</param>
	/// <param name="isCollision">衝突判定を行うかどうか</param>
	void SetCollisionTag(ETag tag, bool isCollision);
	/// <summary>
	/// 指定したオブジェクトタグと衝突判定を行うかどうかを取得
	/// </summary>
	/// <param name="tag">判定するオブジェクトタグ</param>
	/// <returns>trueならば衝突判定を行う</returns>
	bool IsCollisionTag(ETag tag) const;

	// 行列を取得
	CMatrix Matrix() const;

	// コライダー描画
	virtual void Render() = 0;

	//CollisionTriangleLine(三角コライダ, 線分コライダ, 調整値)
	//retrun:true（衝突している）false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleLine(CColliderTriangle* triangle, CColliderLine* line, CVector* adjust);
	//CollisionTriangleSphere(三角コライダ, 球コライダ, 調整値)
	//retrun:true（衝突している）false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleSphere(CColliderTriangle* triangle, CColliderSphere* sphere, CVector* adjust);
	//CollisionSphere(球コライダ, 球コライダ, 調整値)
	//retrun:true（衝突している）false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionSphere(CColliderSphere* sphere1, CColliderSphere* sphere2, CVector* adjust);
	//CollisionSphereLine(球コライダ, 線分コライダ, 調整値)
	//retrun:true（衝突している）false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionSphereLine(CColliderSphere* sphere, CColliderLine* line, CVector* adjust);
	//CollisionLine(線分コライダ, 線分コライダ, 調整値)
	//（内部的にはカプセルとカプセルの衝突判定を行っている)
	//retrun:true（衝突している）false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionLine(CColliderLine* line1, CColliderLine* line2, CVector* adjust);

	static float CalcDistancePointToLine(const CVector& point, const CVector& lineS, const CVector& lineE, CVector* nearest = nullptr);

	//衝突判定
	//Collision(コライダ1, コライダ2)
	//retrun:true（衝突している）false(衝突していない)
	static bool Collision(CCollider* c0, CCollider* c1, CVector* adjust);

protected:
	/// <summary>
	/// コライダーの設定
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	void Set(CObjectBase* owner, ELayer layer);

private:
	ELayer mLayer;			// 衝突判定レイヤー
	EColliderType mType;	// コライダーの種類
	CObjectBase* mpOwner;	// コライダーの持ち主
	bool mIsEnable;			// 有効かどうか
	int mCollisionLayers;	// 衝突判定を行うレイヤーのビットフラグ
	int mCollisionTags;		// 衝突判定を行うオブジェクトタグのビットフラグ
};
