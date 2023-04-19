#pragma once
#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"

/*
コライダクラス
衝突判定データ
*/

class CCollisionManager;
class CCollider :public CTransform,public CTask {
	friend CCollisionManager;
public:
	//優先度の変更
	//void ChangePriority();
	//優先度の変更
	virtual void ChangePriority();
	//優先度の変更
	void ChangePriority(int priority);
	//CollisionTriangleSphere
	//retryn:true(衝突している)false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere, CVector* adjust);
	int Type();
	//CCollisionTriangleLine(三角コライダ,線分コライダ,調整値)
	//retrun:true(衝突している)false(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);
	//コライダ
	enum EType {
		ESPHERE,//球コライダ
		ETRIANGLE,//三角コライダ
		ELINE,//線分コライダ
	};
	//デフォルトコンストラクタ
	CCollider();
	//衝突処理
	//CCollision(コライダ1,コライダ2)
	//retrun:true(衝突している)false(衝突していない)
	static bool CCollision(CCollider* m, CCollider* o);
	~CCollider();
	//コンストラクタ
	//CCollider(親,新行列,位置,半径)
	CCollider(CCharacter3* parent, CMatrix* matrix, const CVector& position, float radius);
	//親ポインタの取得
	CCharacter3* Parent();
	//描画
	void Render();
protected:
	EType mType;//コライダタイプ
	//頂点
	CVector mV[3];
	CCharacter3* mpParent;//親
	CMatrix* mpMatrix;//新行列
	float mRadius; //半径
};

#endif 
