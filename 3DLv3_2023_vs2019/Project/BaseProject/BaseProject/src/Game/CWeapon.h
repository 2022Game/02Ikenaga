#pragma once
#include "BaseSystem/CObjectBase.h"
#include "CharaStatus.h"
class CCharaBase;

class CWeapon : public CObjectBase
{
public:
	CWeapon();
	virtual ~CWeapon();

	void SetAttachMtx(const CMatrix* mtx);

	//攻撃開始
	virtual void AttackStart();

	//攻撃終了
	virtual void AttackEnd();

	// 武器の所持キャラクターを設定
	void SetOwner(CCharaBase* owner);

	// 武器の所持キャラクターを取得
	CCharaBase* GetOwner() const;

	const CharaStatus& MaxStatus() const;
	const CharaStatus& Status() const;

protected:
	// 攻撃がヒットしたオブジェクトを追加
	void AddAttackHitObj(CObjectBase* obj);
	// 既に攻撃がヒットしているオブジェクトかどうか
	bool IsAttackHitObj(CObjectBase* obj) const;
	const CMatrix* mpAttachMtx;

	// 攻撃がヒット済みのオブジェクトリスト
	std::list<CObjectBase*> mAttackHitObjects;

	// 武器の所持キャラクター
	CCharaBase* mOwner;
	CharaStatus mCharaMaxStatus;  //最大ステータス
	CharaStatus mCharaStatus;
};
