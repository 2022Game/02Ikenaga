#pragma once
#include "CObjectBase.h"
#include "CharaStatus.h"


/*
 �L�����N�^�[�̃x�[�X�N���X
*/
class CCharaBase : public CObjectBase
{
public:
	// �R���X�g���N
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// �f�X�g���N�^
	virtual ~CCharaBase();

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	/// <param name="damage">�󂯂鏈��</param>
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

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

	/// <summary>
	/// �U���͂̋����������擾
	/// </summary>
	/// <returns></returns>
	virtual float GetAtkBuff()const;

	/// <summary>
	/// �h��͂̋����������擾
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	virtual float GetDefBuff(const CVector&attackDir)const;

	// �h��̓A�b�v
	bool IsDefenseUp() const;

	// �h��̓A�b�v���̌o�ߎ���
	float GetElapsedDefenseUpTime() const;

protected:
	CharaStatus mCharaMaxStatus;  // �ő�X�e�[�^�X
	CharaStatus mCharaStatus;     // ���݂̃X�e�[�^�X

	float mBaseAttackBuffRatio;   // �U���͂̋������� (1.0�œ��l)
	float mBaseDefenseBuffRatio;  // �h��͂̋������� (1.0�œ��l)

	bool mDefenseUp;              // �h��̓A�b�v(�|�[�V��������)
	float mElapsedDefenseUpTime;  // �o�ߎ���(�h��̓A�b�v�p)
	float mElapsedHealTime;       // �o�ߎ���(�񕜗p)
};

/// <summary>
/// �_���[�W�ʂ��Z�o
/// </summary>
/// <param name="atkRatio">�U���͂̔{��</param>
/// <param name="attaker">�U�����̃L����</param>
/// <param name="defender">�h��͂̃L����</param>
/// <returns>�_���[�W��</returns>
extern int CalcDamage(float atkRatio, const CCharaBase* attaker, const CCharaBase* defender);
