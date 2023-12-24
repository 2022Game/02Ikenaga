#pragma once
#include "CSceneBase.h"
class CText;

// �Q�[���I�[�o�[��ʂƃ��j���[�ɖ߂�I��
class CGameOverScene :public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameOverScene();
	// �f�X�g���N�^
	~CGameOverScene();

	// �V�[���̓ǂݍ���
	void Load()override;
	// �X�V
	void Update()override;

private:
	// ���̍��ڂ�I��
	void SelectNextMenu();
	// �O�̍��ڂ�I��
	void SelectPrevMenu();

	CText* mpText;	// ������`��p

	// �I�����
	struct ChoiceInfo
	{
		std::string name;	// �\����
		EScene scene;		// �J�ڐ�̃V�[��
	};
	// �I�����̃��X�g
	std::vector<ChoiceInfo> mChoiceInfoList;

	int mSelectIndex;	// �I�𒆂̃C���f�b�N�X�l
};