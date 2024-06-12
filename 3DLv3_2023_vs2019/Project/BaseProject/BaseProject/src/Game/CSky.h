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

	// 更新
	void Update();
	// 描画
	void Render();

	static float mElapsedTime;	  // 経過時間
private:
	// インスタンス
	static CSky* spInstance;
	CModel* mpModel;
};