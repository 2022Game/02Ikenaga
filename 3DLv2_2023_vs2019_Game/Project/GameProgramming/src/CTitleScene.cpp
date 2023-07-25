#include "CTitleScene.h"
#include "CResourceManager.h"
#include "CStage.h"
#include "main.h"

//�R���X�g���N�^
CTitleScene::CTitleScene()
	:CSceneBase(EScene::eTitle)
{
}

//�f�X�g���N�^
CTitleScene::~CTitleScene()
{
}

//�V�[���ǂݍ���
void CTitleScene::Load()
{
	//�����Ń^�C�g����ʂɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	mBgImage = new CImage("taitoru.png");
	mBgImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	mBgImage->SetPos(0.0f, 0.0f);
	mBgImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	LOAD_MODEL("Stage", "res\\�̈��.obj", "res\\�̈��.mtl");
	LOAD_MODEL("Enemy1", "res\\�G1.obj", "res\\�G1.mtl");
	LOAD_MODEL("Police", "res\\�T�{��|���X.obj", "res\\�T�{��|���X.mtl");
	LOAD_MODEL("Transparent", "res\\�����̕�.obj", "res\\�����̕�.mtl");
}

void CTitleScene::Update()
{
	if (mInput.PushKey(VK_RETURN))
	{
		CSceneManager::Instance()->LoadScene(EScene::eGame);
	}
	else
	{
		if (mBgImage != nullptr)
		{
			mBgImage->Render();
		}
	}
}