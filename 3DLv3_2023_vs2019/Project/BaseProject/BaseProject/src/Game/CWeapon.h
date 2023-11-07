#pragma once
#include "BaseSystem/CObjectBase.h"

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

protected:
	//攻撃がヒットしたオブジェクトを追加
	void AddAttackHitObj(CObjectBase* obj);
	//既に攻撃がヒットしているオブジェクトかどうか
	bool IsAttackHitObj(CObjectBase* obj) const;
	const CMatrix* mpAttachMtx;

	// 攻撃がヒット済みのオブジェクトリスト
	std::list<CObjectBase*> mAttackHitObjects;
};
