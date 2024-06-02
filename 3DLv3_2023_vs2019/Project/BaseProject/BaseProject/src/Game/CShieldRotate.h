#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 ��]����V�[���h�N���X
 ����N���X�p��
*/
class CShieldRotate : public CWeapon
{
public:
	// �R���X�g���N
	CShieldRotate(const CVector& pos, const CColor& col);
	// �f�X�g���N�^
	~CShieldRotate();

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
	// �ړ����x
	//CVector mMoveSpeed;
};
