#include "CTitleScene.h"
#include "CResourceManager.h"
#include "CStage.h"
#include "main.h"

//コンストラクタ
CTitleScene::CTitleScene()
	:CSceneBase(EScene::eTitle)
{
}

//デストラクタ
CTitleScene::~CTitleScene()
{
}

//シーン読み込み
void CTitleScene::Load()
{
	//ここでタイトル画面に必要な
	//リソースの読み込みやクラスの生成を行う
	mBgImage = new CImage("taitoru.png");
	mBgImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	mBgImage->SetPos(0.0f, 0.0f);
	mBgImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	LOAD_MODEL("Stage", "res\\体育館.obj", "res\\体育館.mtl");
	LOAD_MODEL("Enemy1", "res\\敵1.obj", "res\\敵1.mtl");
	LOAD_MODEL("Police", "res\\サボるポリス.obj", "res\\サボるポリス.mtl");
	LOAD_MODEL("Transparent", "res\\透明の壁.obj", "res\\透明の壁.mtl");
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