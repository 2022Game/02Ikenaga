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

protected:
	CharaStatus mCharaMaxStatus;  //最大ステータス
	CharaStatus mCharaStatus;  //現在のステータス
};
