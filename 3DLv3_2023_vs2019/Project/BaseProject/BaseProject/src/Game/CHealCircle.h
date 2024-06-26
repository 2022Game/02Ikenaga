#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 �񕜃T�[�N��
 ����N���X�p��
*/
class CHealCircle : public CWeapon
{
public:
	// �R���X�g���N
	CHealCircle();
	// �f�X�g���N�^
	~CHealCircle();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �I�[���J�n
	void StartCircle();

	// �폜
	void Delete();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	//�J�E���g
	int mCount;
	// ���f���f�[�^�ǂݍ���
	CModel* mpHealCircle;

	float mElapsedHealTime;  // �o�ߎ���
	float mDistance;         // �����傩��̋���
	float mBaseScale;        // ������̃x�[�X�X�P�[���l
};