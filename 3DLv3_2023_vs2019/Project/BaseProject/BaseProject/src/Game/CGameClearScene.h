#pragma once
#include "CTask.h"
#include "CImage.h"

class CTexk;

class CGameClearScene : public CTask
{
public:
	// �C���X�^���X���擾
	static CGameClearScene* Instance();
	// �C���X�^���X���폜
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
	CGameClearScene();
	// �f�X�g���N�^
	~CGameClearScene();

	static CGameClearScene* spInstance;

	CImage* mpBackground;
	CImage* mpBackground2;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	CImage* mpUpUI;
	CImage* mpDownUI;

	CImage* mpEnterKey;

	int mSelectIndex;
	bool mIsOpened;
	CText* mpGameClear;  // �Q�[���N���A
	CText* mpEnd;
};