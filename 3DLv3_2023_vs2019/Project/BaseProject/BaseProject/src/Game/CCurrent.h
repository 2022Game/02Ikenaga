#pragma once
#include "CWeapon.h"
#include "CTexture.h"
#include <functional>

class CLineEffect;
class CColliderLine;

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

	// �q�b�g�������̃R�[���o�b�N�֐���ݒ�
	void SetCollisionCallback(std::function<void()>callback);

	//�U���J�n
	void AttackStart()override;

	//�U���I��
	void AttackEnd()override;

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

	float mElapsedTime;  // �o�ߎ���

	CLineEffect* mpLineEffect;
	TexAnimData mEffectAnimData;
	CColliderLine* mpAttackCol;          // �U���R���C�_�[
	std::function<void()> mColCallback;  // �R�[���o�b�N�֐�
};