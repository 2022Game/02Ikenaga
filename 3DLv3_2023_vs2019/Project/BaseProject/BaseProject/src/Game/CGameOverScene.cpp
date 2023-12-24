#include "CGameOverScene.h"
#include "CText.h"
#include "CInput.h"
#include "CSceneManager.h"

// コンストラクタ
CGameOverScene::CGameOverScene()
	:CSceneBase(EScene::eGameOver)
	, mSelectIndex(0)
{
	// 選択情報リストを作成
	mChoiceInfoList =
	{
		{ "■ゲームオーバー", EScene::eNone },
		{ "再スタート",EScene::eGame},
		{ "メニュー画面", EScene::eBootMenu },
	};

	// 一番先頭の項目が選択できない項目であれば、次の項目を初期選択に変更
	if (mChoiceInfoList[mSelectIndex].scene == EScene::eNone)
	{
		SelectNextMenu();
	}
}

// デストラクタ
CGameOverScene::~CGameOverScene()
{
}

// シーン読み込み
void CGameOverScene::Load()
{
	// ゲームオーバーの背景黒色
	System::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 選択の表示用のテキストを追加
	mpText = new CText
	(
		nullptr, 50,
		CVector2(400.0f, 250.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor::white
	);
}

// 次の項目を選択
void CGameOverScene::SelectNextMenu()
{
	// 選択可能な項目が来るまで、次へ進める
	// （eNoneの項目は選択不可）
	while (true)
	{
		mSelectIndex = (mSelectIndex + 1) % mChoiceInfoList.size();
		if (mChoiceInfoList[mSelectIndex].scene != EScene::eNone)
		{
			break;
		}
	}
}

// 前の項目を選択
void CGameOverScene::SelectPrevMenu()
{
	// 選択可能な項目が来るまで、前に戻る
	// （eNoneの項目は選択不可）
	while (true)
	{
		mSelectIndex = (mSelectIndex - 1 + mChoiceInfoList.size()) % mChoiceInfoList.size();
		if (mChoiceInfoList[mSelectIndex].scene != EScene::eNone)
		{
			break;
		}
	}
}

// 更新
void CGameOverScene::Update()
{
	// 「↑」キーで前の項目に戻る
	if (CInput::PushKey(VK_UP))
	{
		SelectPrevMenu();
	}
	// 「↓」キーで次の項目へ進める
	else if (CInput::PushKey(VK_DOWN))
	{
		SelectNextMenu();
	}

	// 表示するメニュー情報をテキストに設定
	std::string str = "";
	int size = mChoiceInfoList.size();
	for (int i = 0; i < size; i++)
	{
		ChoiceInfo& info = mChoiceInfoList[i];
		str += (i == mSelectIndex) ? ">> " : "   ";
		str += info.name + "\n";
	}
	mpText->SetText(str.c_str());

	// 「Enter」キーもしくは、「SPACE」キーを押したら、
	// 選択したメニューのシーンへ移行
	if (CInput::PushKey(VK_RETURN) || CInput::PushKey(VK_SPACE))
	{
		ChoiceInfo& select = mChoiceInfoList[mSelectIndex];
		CSceneManager::Instance()->LoadScene(select.scene);
	}
}