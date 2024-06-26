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

	// �I�[���J�n
	void StartCircle();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	//�J�E���g
	int mCount;
	// ���f���f�[�^�ǂݍ���
	CModel* mpBuffCircle;

	bool mPowerUp;
	float mElapsedPowerUpTime;  // �o�ߎ���
	float mDistance;            // �����傩��̋���
	float mBaseScale;           // ������̃x�[�X�X�P�[���l
};