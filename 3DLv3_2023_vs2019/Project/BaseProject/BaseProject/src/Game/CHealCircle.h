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
	CHealCircle(float angle, float dist);
	// �f�X�g���N�^
	~CHealCircle();

	// �C���X�^���X�̃|�C���^�̎擾
	static CHealCircle* Instance();

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
	CModel* mpHealCircle;
	// �C���X�^���X
	static CHealCircle* spInstance;
	// �o�ߎ���
	float mElapsedHealTime;

	float mAngle;      // ��]�p�x
	float mDistance;   // �����傩��̋���
	float mBaseScale;  // ������̃x�[�X�X�P�[���l
};