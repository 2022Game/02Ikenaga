#pragma once
#include "CObjectBase.h"
#include "CColliderLine.h"
#include "CModel.h"
#include "CWeapon.h"

/*
 ���N���X
 ����N���X�p��
*/
class CSword : public CWeapon
{
public:
	// �R���X�g���N
	CSword();
	// �f�X�g���N�^
	~CSword();

	// �X�V
	void Update();
	// �`��
	void Render();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

	// ����̍s��擾
	CMatrix Matrix() const override;

	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;
	// �U���̓A�b�v
	void PowerUp();

private:
	// ���f���f�[�^�ǂݍ���
	CModel* mpSword;
	// �U���̓A�b�v
	bool mPowerUp;
	// �o�ߎ���(�U���̓A�b�v�p)
	float mElapsedPowerUpTime;
	// �U������p�̃R���C�_�[
	CColliderLine* mpAttackCol;
};

