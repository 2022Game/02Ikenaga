#pragma once
#include "CTask.h"
#include "CImage.h"

class CTexk;

class CGameOverScene : public CTask
{
public:
	// �C���X�^���X���擾
	static CGameOverScene* Instance();
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
	CGameOverScene();
	// �f�X�g���N�^
	~CGameOverScene();

	static CGameOverScene* spInstance;

	CImage* mpBackground;
	CImage* mpBackground2;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	CImage* mpUpUI;
	CImage* mpDownUI;

	CImage* mpEnterKey;

	int mSelectIndex;
	bool mIsOpened;

	CText* mpGameOver;  // �Q�[���I�[�o�[
	CText* mpRevival;   // ����
	CText* mpTitle;     // �^�C�g��
};