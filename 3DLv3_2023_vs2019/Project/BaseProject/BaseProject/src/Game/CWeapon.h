#pragma once
#include "BaseSystem/CObjectBase.h"
#include "CharaStatus.h"
class CCharaBase;

class CWeapon : public CObjectBase
{
public:
	// �R���X�g���N
	CWeapon();
	// �f�X�g���N�^
	virtual ~CWeapon();

	void SetAttachMtx(const CMatrix* mtx);

	//�U���J�n
	virtual void AttackStart();

	//�U���I��
	virtual void AttackEnd();

	// ����̏����L�����N�^�[��ݒ�
	virtual void SetOwner(CCharaBase* owner);

	// ����̏����L�����N�^�[���擾
	CCharaBase* GetOwner() const;

protected:
	// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
	void AddAttackHitObj(CObjectBase* obj);
	// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
	bool IsAttackHitObj(CObjectBase* obj) const;
	const CMatrix* mpAttachMtx;

	// �U�����q�b�g�ς݂̃I�u�W�F�N�g���X�g
	std::list<CObjectBase*> mAttackHitObjects;

	// ����̏����L�����N�^�[
	CCharaBase* mOwner;
};
