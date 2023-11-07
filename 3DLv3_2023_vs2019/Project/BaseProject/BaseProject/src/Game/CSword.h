#pragma once
#include "CObjectBase.h"
#include "CColliderLine.h"
#include "CModel.h"
#include "CWeapon.h"
#include "CharaStatus.h"

/*
���N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CSword : public CWeapon
{
public:
	CSword();
	~CSword();

	void Update();
	void Render();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

	//����̍s��擾
	CMatrix Matrix() const override;

	//�U���J�n
	void AttackStart() override;
	//�U���I��
	void AttackEnd() override;
private:

	CModel* mpSword;
	//�U������p�̃R���C�_�[
	CColliderLine* mpAttackCol;
};

