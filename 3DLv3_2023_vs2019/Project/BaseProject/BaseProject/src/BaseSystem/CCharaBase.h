#pragma once
#include "CObjectBase.h"
#include "CharaStatus.h"


/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
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

	virtual float GetAtkBuff()const;

	virtual float GetDefBuff(const CVector&attackDir)const;

	bool IsDefenseUp() const;

	float GetElapsedDefenseUpTime() const;

protected:
	CharaStatus mCharaMaxStatus;  // 最大ステータス
	CharaStatus mCharaStatus;     // 現在のステータス

	float mBaseAttackBuffRatio;   // 攻撃力の強化割合 (1.0で等値)
	float mBaseDefenseBuffRatio;  // 防御力の強化割合 (1.0で等値)

	bool mDefenseUp;              // 防御力アップ(ポーション効果)
	float mElapsedDefenseUpTime;  // 経過時間(防御力アップ用)
};

/// <summary>
/// ダメージ量を算出
/// </summary>
/// <param name="atkRatio">攻撃力の倍率</param>
/// <param name="attaker">攻撃側のキャラ</param>
/// <param name="defender">防御力のキャラ</param>
/// <returns>ダメージ量</returns>
extern int CalcDamage(float atkRatio, const CCharaBase* attaker, const CCharaBase* defender);
