#include "CWeapon.h"
#include "CCharaBase.h"

// �R���X�g���N
CWeapon::CWeapon()
	: CObjectBase(ETag::eWeapon, ETaskPriority::eWeapon)
	, mpAttachMtx(nullptr)
	, mOwner(nullptr)
{
}

// �f�X�g���N�^
CWeapon::~CWeapon()
{
}

void CWeapon::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// �U���J�n
void CWeapon::AttackStart()
{
	mAttackHitObjects.clear();
}

// �U���I��
void CWeapon::AttackEnd()
{
}

// ����̏����L�����N�^�[��ݒ�
void CWeapon::SetOwner(CCharaBase* owner)
{
	mOwner = owner;
}

// ����̏����L�����N�^�[���擾
CCharaBase* CWeapon::GetOwner() const
{
	return mOwner;
}

// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
void CWeapon::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
bool CWeapon::IsAttackHitObj(CObjectBase* obj) const
{
	// ���Ƀ��X�g�ɒǉ�����Ă��邩���m�F����
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}