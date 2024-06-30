#include "CCharaBase.h"

// コンストラク
CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mBaseAttackBuffRatio(1.0f)
	, mBaseDefenseBuffRatio(1.0f)
	, mDefenseUp(false)
	, mElapsedDefenseUpTime(0.0f)
	, mPowerUp(false)
	, mElapsedPowerUpTime(0.0f)
{
	mCharaMaxStatus.Reset();
	mCharaStatus.Reset();
}

// デストラクタ
CCharaBase::~CCharaBase()
{
}

// 被ダメージ処理
void CCharaBase::TakeDamage(int damage, CObjectBase* causedObj)
{
}

// 最大ステータスを取得
const CharaStatus& CCharaBase::MaxStatus() const
{
	return mCharaMaxStatus;
}

// 現在のステータスを取得
const CharaStatus& CCharaBase::Status() const
{
	return mCharaStatus;
}

// 攻撃力の強化割合を取得
float CCharaBase::GetAtkBuff()const
{
	return mBaseAttackBuffRatio;
}

// 防御力の強化割合を取得
float CCharaBase::GetDefBuff(const CVector& attackDir)const
{
	return mBaseDefenseBuffRatio;
}

// 防御力アップ
bool CCharaBase::IsDefenseUp() const
{
	return mDefenseUp;
}

// 防御力アップ中の経過時間
float CCharaBase::GetElapsedDefenseUpTime() const
{
	return mElapsedDefenseUpTime;
}

// 攻撃力アップ
bool CCharaBase::IsPowerUp() const
{
	return mPowerUp;
}

// 攻撃力アップ中の経過時間
float CCharaBase::GetElapsedPowerUpTime() const
{
	return mElapsedPowerUpTime;
}

//　ダメージ量を算出
int CalcDamage(float atkRatio,const CCharaBase* attaker, const CCharaBase* defender)
{
	if (attaker == nullptr || defender == nullptr)return 1;

	// (攻撃側の位置 - 防御側)の正規化したベクトルを取得
	CVector attackDir = (attaker->Position() - defender->Position()).Normalized();

	// 現在の攻撃力×強化割合×比率
	float atk = attaker->Status().power * attaker->GetAtkBuff() * atkRatio;
	// 現在の防御力×強化割合
	float def = defender->Status().defense * defender->GetDefBuff(attackDir);

	// ダメージ量 =（攻撃力÷2）-（防御÷4）
	int damage = (atk * 0.5f) - (def * 0.25f) + 0.5f;
	return std::max(damage, 1);
}
