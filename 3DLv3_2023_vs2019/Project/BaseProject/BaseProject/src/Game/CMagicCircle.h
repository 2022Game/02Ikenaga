#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CModel.h"

// ���@�w
class CMagicCircle : public CWeapon
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�G�t�F�N�g�𔭐��������I�u�W�F�N�g</param>
	/// <param name="pos">�����ʒu</param>
	CMagicCircle(CObjectBase* owner, const CVector& pos);

	// �f�X�g���N�^
	~CMagicCircle();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;  // ���̃G�t�F�N�g�̎�����
	CModel* mpModel;	   // �G�t�F�N�g�̃��f���f�[�^
};