#pragma once
#include "CObjectBase.h"
#include "CharaStatus.h"


/*
 キャラクターのベースクラス
*/
class CCharaBase : public CObjectBase
{
public:
	// コンストラク
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	virtual ~CCharaBase();

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">受ける処理</param>
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	/// <summary>
	/// 最大ステータスを取得
	/// </summary>
	/// <returns></returns>
	const CharaStatus& MaxStatus() const;
	/// <summary>
	/// 現在のステータスを取得
	/// </summary>
	/// <returns></returns>
	const CharaStatus& Status() const;

	/// <summary>
	/// 攻撃力の強化割合を取得
	/// </summary>
	/// <returns></returns>
	virtual float GetAtkBuff()const;

	/// <summary>
	/// 防御力の強化割合を取得
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	virtual float GetDefBuff(const CVector&attackDir)const;

	// 防御力アップ
	bool IsDefenseUp() const;

	// 防御力アップ中の経過時間
	float GetElapsedDefenseUpTime() const;

	// 攻撃力アップ
	bool IsPowerUp() const;

	// 攻撃力アップ中の経過時間
	float GetElapsedPowerUpTime() const;

protected:
	CharaStatus mCharaMaxStatus;  // 最大ステータス
	CharaStatus mCharaStatus;     // 現在のステータス

	float mBaseAttackBuffRatio;   // 攻撃力の強化割合 (1.0で等値)
	float mBaseDefenseBuffRatio;  // 防御力の強化割合 (1.0で等値)

	bool mDefenseUp;              // 防御力アップ
	float mElapsedDefenseUpTime;  // 経過時間(防御力アップ用)
	bool mPowerUp;                // 攻撃力アップ
	float mElapsedPowerUpTime;    // 経過時間(攻撃力アップ用)
};

/// <summary>
/// ダメージ量を算出
/// </summary>
/// <param name="atkRatio">攻撃力の倍率</param>
/// <param name="attaker">攻撃側のキャラ</param>
/// <param name="defender">防御力のキャラ</param>
/// <returns>ダメージ量</returns>
extern int CalcDamage(float atkRatio, const CCharaBase* attaker, const CCharaBase* defender);
