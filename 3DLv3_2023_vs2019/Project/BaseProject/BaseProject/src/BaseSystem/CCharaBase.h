#pragma once
#include "CObjectBase.h"
#include "CharaStatus.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio);
	virtual ~CCharaBase();

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	/// <param name="damage">�󂯂鏈��</param>
	virtual void TakeDamage(int damage);

	/// <summary>
	/// �ő�X�e�[�^�X���擾
	/// </summary>
	/// <returns></returns>
	const CharaStatus& MaxStatus() const;
	/// <summary>
	/// ���݂̃X�e�[�^�X���擾
	/// </summary>
	/// <returns></returns>
	const CharaStatus& Status() const;

protected:
	CharaStatus mCharaMaxStatus;  //�ő�X�e�[�^�X
	CharaStatus mCharaStatus;  //���݂̃X�e�[�^�X
};
