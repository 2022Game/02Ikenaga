#pragma once
#include "CObjectBase.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"

/*
�o���l�N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CExp : public CBillBoardImage
{
public:
	CExp();
	~CExp();

	// �����ݒ�
	void Setup(const CVector& pos, const CVector& dir);

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	void Update() override;

private:
	CColliderSphere* mpColliderSphere;
	CVector mMoveSpeed;  //�ړ����x

	// ���
	enum class EState
	{
		Drop,       // �o���l�𗎂Ƃ������̏���
		Absorb,     // �o���l���z������鎞�̏���
	};
	EState mState;  // ���݂̏��
	float mElapsedTime;  // �o�ߎ��Ԍv���p

};