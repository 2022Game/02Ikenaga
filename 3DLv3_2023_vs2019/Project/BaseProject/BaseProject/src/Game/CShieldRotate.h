#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 ��]����V�[���h�N���X
 ������傫���ɂȂ�
 ����N���X�p��
*/
class CShieldRotate : public CWeapon
{
public:
	// �R���X�g���N
	CShieldRotate(float angle,float dist);
	// �f�X�g���N�^
	~CShieldRotate();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	int mStateStep;
	// �h��̓A�b�v(�|�[�V��������)
	bool mDefenseUp;
	// ���f���f�[�^�ǂݍ���
	CModel* mpShieldRotate;
	// �o�ߎ���(�h��̓A�b�v�p)
	float mElapsedDefenseUpTime;

	float mAngle;      // ���̉�]�p�x
	float mDistance;   // �����傩��̋���
	float mBaseScale;  // ������̃x�[�X�X�P�[���l
};
