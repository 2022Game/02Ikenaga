#pragma once
#include "CTask.h"
#include "CImage.h"

class COperationIns : public CTask
{
public:
	// �R���X�g���N
	COperationIns();
	// �f�X�g���N�^
	~COperationIns();

	// �I�[�v��
	void Open();
	// �N���[�Y
	void Close();
	// �I�[�v�����邩�ǂ���
	bool IsOpened() const;

	// ���߂�
	void Decide(int select);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CImage* mpBackground;
	// �e�L�X�g�֘A
	CImage* mpTextOperation;
	CImage* mpTextMove;
	CImage* mpTextSpecial;
	CImage* mpTextSP;
	CImage* mpTextMenu;
	CImage* mpTextJump;
	CImage* mpTextAvoid;
	CImage* mpTextMove2;
	CImage* mpTextAttack;
	CImage* mpTextGuard;
	// �L�[�֘A
	CImage* mpWkey;
	CImage* mpAkey;
	CImage* mpSkey;
	CImage* mpDkey;
	CImage* mpQkey;
	CImage* mpEkey;
	CImage* mpMkey;
	CImage* mpSpaceKey;
	CImage* mpShiftKey;
	// �}�E�X�֘A
	CImage* mpMouseLeft;
	CImage* mpMouseMiddle;
	CImage* mpMouseRight;
	// �L���֘A
	CImage* mpPlus;

	bool mIsOpened;
};