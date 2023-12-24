#include "CGameOverScene.h"
#include "CText.h"
#include "CInput.h"
#include "CSceneManager.h"

// �R���X�g���N�^
CGameOverScene::CGameOverScene()
	:CSceneBase(EScene::eGameOver)
	, mSelectIndex(0)
{
	// �I����񃊃X�g���쐬
	mChoiceInfoList =
	{
		{ "���Q�[���I�[�o�[", EScene::eNone },
		{ "�ăX�^�[�g",EScene::eGame},
		{ "���j���[���", EScene::eBootMenu },
	};

	// ��Ԑ擪�̍��ڂ��I���ł��Ȃ����ڂł���΁A���̍��ڂ������I���ɕύX
	if (mChoiceInfoList[mSelectIndex].scene == EScene::eNone)
	{
		SelectNextMenu();
	}
}

// �f�X�g���N�^
CGameOverScene::~CGameOverScene()
{
}

// �V�[���ǂݍ���
void CGameOverScene::Load()
{
	// �Q�[���I�[�o�[�̔w�i���F
	System::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// �I���̕\���p�̃e�L�X�g��ǉ�
	mpText = new CText
	(
		nullptr, 50,
		CVector2(400.0f, 250.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor::white
	);
}

// ���̍��ڂ�I��
void CGameOverScene::SelectNextMenu()
{
	// �I���\�ȍ��ڂ�����܂ŁA���֐i�߂�
	// �ieNone�̍��ڂ͑I��s�j
	while (true)
	{
		mSelectIndex = (mSelectIndex + 1) % mChoiceInfoList.size();
		if (mChoiceInfoList[mSelectIndex].scene != EScene::eNone)
		{
			break;
		}
	}
}

// �O�̍��ڂ�I��
void CGameOverScene::SelectPrevMenu()
{
	// �I���\�ȍ��ڂ�����܂ŁA�O�ɖ߂�
	// �ieNone�̍��ڂ͑I��s�j
	while (true)
	{
		mSelectIndex = (mSelectIndex - 1 + mChoiceInfoList.size()) % mChoiceInfoList.size();
		if (mChoiceInfoList[mSelectIndex].scene != EScene::eNone)
		{
			break;
		}
	}
}

// �X�V
void CGameOverScene::Update()
{
	// �u���v�L�[�őO�̍��ڂɖ߂�
	if (CInput::PushKey(VK_UP))
	{
		SelectPrevMenu();
	}
	// �u���v�L�[�Ŏ��̍��ڂ֐i�߂�
	else if (CInput::PushKey(VK_DOWN))
	{
		SelectNextMenu();
	}

	// �\�����郁�j���[�����e�L�X�g�ɐݒ�
	std::string str = "";
	int size = mChoiceInfoList.size();
	for (int i = 0; i < size; i++)
	{
		ChoiceInfo& info = mChoiceInfoList[i];
		str += (i == mSelectIndex) ? ">> " : "   ";
		str += info.name + "\n";
	}
	mpText->SetText(str.c_str());

	// �uEnter�v�L�[�������́A�uSPACE�v�L�[����������A
	// �I���������j���[�̃V�[���ֈڍs
	if (CInput::PushKey(VK_RETURN) || CInput::PushKey(VK_SPACE))
	{
		ChoiceInfo& select = mChoiceInfoList[mSelectIndex];
		CSceneManager::Instance()->LoadScene(select.scene);
	}
}