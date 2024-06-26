#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 �o�t�T�[�N��
 ����N���X�p��
*/
class CBuffCircle : public CWeapon
{
public:
	// �R���X�g���N
	CBuffCircle();
	// �f�X�g���N�^
	~CBuffCircle();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �o�t�T�[�N���J�n
	void StartCircle();
	// �o�t�T�[�N���I��
	void EndCircle();

	// �X�V
	void Update();
	// �`��
	void Render();

private:

	// ���g�̃x�[�X�X�P�[���l���Z�o
	float CalcScale() const;

	// �ҋ@��
	void UpdateIdle();
	// �J�n
	void UpdateStart();
	// �I���҂�
	void UpdateWait();
	// �I��
	void UpdateEnd();

	// ���
	enum EState
	{
		Idle,   // �ҋ@��
		Start,  // �J�n
		Wait,   // �I���҂�
		End,    // �I��
	};
	void ChangeState(EState state);

	EState mState;         // ���݂̏��
	float mElapsedTime;    // �o�ߎ���
	CModel* mpBuffCircle;  // ���f���f�[�^�ǂݍ���
	float mBaseScale;      // ������̃x�[�X�X�P�[���l
};