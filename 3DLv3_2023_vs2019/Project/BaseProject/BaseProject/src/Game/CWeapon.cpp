#include "CWeapon.h"

CWeapon::CWeapon()
	: CObjectBase(ETag::eWeapon, ETaskPriority::eWeapon)
	, mpAttachMtx(nullptr)
{
}

CWeapon::~CWeapon()
{
}

void CWeapon::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// 攻撃開始
void CWeapon::AttackStart()
{
	mAttackHitObjects.clear();
}

// 攻撃終了
void CWeapon::AttackEnd()
{
}


// 攻撃がヒットしたオブジェクトを追加
void CWeapon::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// 既に攻撃がヒットしているオブジェクトかどうか
bool CWeapon::IsAttackHitObj(CObjectBase* obj) const
{
	// 既にリストに追加されているかを確認する
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}