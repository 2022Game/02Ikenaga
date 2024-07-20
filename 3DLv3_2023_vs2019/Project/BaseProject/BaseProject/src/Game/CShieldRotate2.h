#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 ��]����V�[���h2�N���X
 ����N���X�p��
*/
class CShieldRotate2 : public CWeapon
{
public:
	// �R���X�g���N
	CShieldRotate2(float angle, float dist, float height);
	// �f�X�g���N�^
	~CShieldRotate2();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	// �h��̓A�b�v(�|�[�V��������)
	bool mDefenseUp;
	// ���f���f�[�^�ǂݍ���
	CModel* mpShieldRotate2;
	// �o�ߎ���(�h��̓A�b�v�p)
	float mElapsedDefenseUpTime;

	float mAngle;      // ���̉�]�p�x
	float mDistance;   // �����傩��̋���
	float mHeight;     // ����
};