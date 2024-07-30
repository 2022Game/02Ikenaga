#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 空
*/
class CSky : public CObjectBase
{
public:
	// インスタンスのポインタの取得
	static CSky* Instance();

	// コンストラク
	CSky();
	//　デストラクタ
	~CSky();

	// コライダーを取得
	CCollider* GetCollider() const;

	// 更新
	void Update();
	// 描画
	void Render();

	static float mElapsedTime;	  // 経過時間
private:
	// インスタンス
	static CSky* spInstance;
	// モデル
	CModel* mpModel;
	// コライダーメッシュ
	CColliderMesh* mpColliderMesh;
};