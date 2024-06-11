#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CModel.h"

// 雄叫びエフェクト
class CRoar : public CWeapon
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">エフェクトを発生させたオブジェクト</param>
	/// <param name="pos">発生位置</param>
	/// <param name="dir">移動方向</param>
	/// <param name="speed">移動速度</param>
	/// <param name="dist">移動したら消える距離</param>
	CRoar(CObjectBase* owner, const CVector& pos, const CVector& dir, float speed, float dist);

	// デストラクタ
	~CRoar();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;  // このエフェクトの持ち主
	CModel* mpModel;	   // エフェクトのモデルデータ
	CVector mMoveSpeed;	   // 移動速度
	float mKillMoveDist;   // 移動したら消える距離
	float mMovedDist;	   // 現在移動した距離
	float mElapsedTime;	   // 経過時間
};