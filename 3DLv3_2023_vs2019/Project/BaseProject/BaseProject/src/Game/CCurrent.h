#pragma once
#include "CWeapon.h"
#include "CTexture.h"
#include "CColliderLine.h"

class CLineEffect;

class CCurrent : public CWeapon
{
public:
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�G�t�F�N�g�𔭐��������I�u�W�F�N�g</param>
	/// <param name="pos">�����ʒu</param>
	/// <param name="dir">����</param>
	CCurrent(CObjectBase* owner, const CVector& pos, const CVector& dir);

	//�f�X�g���N�^
	~CCurrent();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;

private:

	CLineEffect* mpLineEffect;
	TexAnimData mEffectAnimData;

	CColliderLine* mpAttackCol;
};