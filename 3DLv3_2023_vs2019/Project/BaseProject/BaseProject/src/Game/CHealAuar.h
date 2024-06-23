#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 �񕜃I�[��
 ����N���X�p��
*/
class CHealAura : public CWeapon
{
public:
	// �R���X�g���N
	CHealAura(float angle, float dist);
	// �f�X�g���N�^
	~CHealAura();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �I�[���J�n
	void StartAura();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	//�J�E���g
	int mCount;
	// ���f���f�[�^�ǂݍ���
	CModel* mpHealAura;
	// �o�ߎ���
	float mElapsedHealTime;

	float mAngle;      // ���̉�]�p�x
	float mDistance;   // �����傩��̋���
	float mBaseScale;  // ������̃x�[�X�X�P�[���l
};