#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CModel.h"

// 魔法陣
class CMagicCircle : public CWeapon
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">エフェクトを発生させたオブジェクト</param>
	/// <param name="pos">発生位置</param>
	CMagicCircle(CObjectBase* owner, const CVector& pos);

	// デストラクタ
	~CMagicCircle();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;  // このエフェクトの持ち主
	CModel* mpModel;	   // エフェクトのモデルデータ
	float mElapsedTime;  // 経過時間
};