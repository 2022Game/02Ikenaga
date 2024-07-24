#pragma once
#include "CTask.h"
#include "CImage.h"
#include "COperationIns.h"
#include "CPlayerStatusUI.h"

class CGameMenu : public CTask
{
public:
	// �Q�[�����j���[�̃C���X�^���X���擾
	static CGameMenu* Instance();
	// �Q�[�����j���[�̃C���X�^���X���폜
	static void ClearInstance();

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
	// �R���X�g���N
	CGameMenu();
	// �f�X�g���N�^
	~CGameMenu();

	static CGameMenu* spInstance;

	CImage* mpBackground;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	CImage* mpTextOperation;
	CImage* mpStatus;
	CImage* mpTextReturn;
	CImage* mpUpUI;
	CImage* mpDownUI;
	
	CImage* mpEnterKey;

	int mSelectIndex;
	bool mIsOpened;

	COperationIns* mpOperationIns;
	CPlayerStatusUI* mpPlayerStatusUI;
};