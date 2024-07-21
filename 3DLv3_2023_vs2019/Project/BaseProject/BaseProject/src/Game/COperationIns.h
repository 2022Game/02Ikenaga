#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

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
	CImage* mpText;
	CImage* mpText2;
	CImage* mpText3;
	CImage* mpTextSP;
	CImage* mpText4;
	CImage* mpText5;
	CImage* mpText6;
	CImage* mpText7;
	// �L�[�֘A
	CImage* mpWkey;
	CImage* mpAkey;
	CImage* mpSkey;
	CImage* mpDkey;
	CImage* mpQkey;
	CImage* mpEkey;
	CImage* mpMkey;
	CImage* mpSpaceKey;
	// �}�E�X�֘A
	CImage* mpMouseLeft;
	CImage* mpMouseMiddle;
	CImage* mpMouseRight;

	bool mIsOpened;
};