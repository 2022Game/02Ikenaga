#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
�U���̓A�b�v�̃|�[�V�����N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CPortionRed : public CObjectBase
{
public:
	CPortionRed();
	~CPortionRed();

	void Update();
	void Render();

private:

	CModel* mpPortionRed;
};