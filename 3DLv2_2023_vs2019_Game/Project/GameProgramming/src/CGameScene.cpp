#include "CGameScene.h"
#include "CResourceManager.h"
#include "CStageManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CStage.h"
#include "CKokki.h"
#include "CTransparent.h"
#include "CFrame.h"
#include "CClear.h"
#include "CSceneManager.h"

//コンストラクタ
CGameScene::CGameScene()
	:CSceneBase(EScene::eGame)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
void CGameScene::Load()
{
	//ここでタイトル画面に必要な
	//リソースの読み込みやクラスの生成を行う
	//LOAD_MODEL("Stage", "res\\体育館.obj", "res\\体育館.mtl");
	LOAD_MODEL("Kokki", "res\\国旗.obj", "res\\国旗.mtl");
	LOAD_MODEL("SP", "res\\SP.obj", "res\\SP.mtl");
	LOAD_MODEL("Frame", "res\\枠緑色.obj", "res\\枠緑色.mtl");
	LOAD_MODEL("Clear", "res\\クリア枠.obj", "res\\クリア枠.mtl");
	LOAD_MODEL("Clear2", "res\\クリア枠.obj", "res\\クリア枠.mtl");
	LOAD_MODEL("Enemy2", "res\\銃持ち敵.obj", "res\\銃持ち敵.mtl");
	LOAD_MODEL("Player", "res\\obama4.obj", "res\\obama4.mtl");
	LOAD_MODEL("Water", "res\\水.obj", "res\\水.mtl");
	LOAD_MODEL("Shark", "res\\サメ.obj", "res\\サメ.mtl");
	LOAD_MODEL("Tiger", "res\\tiger.obj", "res\\tiger.mtl");
	LOAD_MODEL("Meat", "res\\meat.obj", "res\\meat.mtl");
	LOAD_MODEL("Button", "res\\ボタン.obj", "res\\ボタン.mtl");

	//LOAD_MODEL("BG", "res\\sky.obj", "res\\sky.mtl");

	AddTask(new CField());

	AddTask(new CStage(GET_MODEL("Stage"), CVector(30.0f, -0.99f, 0.0f), CVector(), CVector(1.0f, 1.0f, 1.0f)));
	CModel* kokki = GET_MODEL("Kokki");
	//new CKokki(&mKokki, CVector(-10.0f, 1.0f, 0.0f), CVector(0.0f, -90.0f, 0.0f), CVector(5.5f, 5.5f,0.5f));
	AddTask(new CKokki(kokki, CVector(30.0f, 4.0f, -19.65f), CVector(0.0f, -90.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f)));
	//new CKokki(&mKokki, CVector(30.0f, -1.2f, -17.6f), CVector(0.0f, 0.0f, 0.0f), CVector(48.4f, 0.84f, 0.95f));
	AddTask(new CKokki(kokki, CVector(23.55f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));//透明の入口から見て左のステージ袖の階段
	AddTask(new CKokki(kokki, CVector(23.25f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(22.95f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(22.65f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(22.35f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(23.25f, -0.2f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.1f)));//左のステージ袖の階段の前の板

	AddTask(new CKokki(kokki, CVector(36.45f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));//右のステージ袖の階段
	AddTask(new CKokki(kokki, CVector(36.75f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(37.05f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(37.35f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(37.65f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f)));
	AddTask(new CKokki(kokki, CVector(36.77, -0.654f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.3f))); //右のステージ袖の階段の前の板

	CModel* tp = GET_MODEL("Transparent");
	AddTask(new CTransparent(tp, CVector(30.0f, -2.648f, -17.6f), CVector(0.0f, 0.0f, 0.0f), CVector(48.4f, 0.84f, 0.95f)));//-2.649
	AddTask(new CTransparent(tp, CVector(22.15f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(18.0f, 0.84f, 0.8f)));
	AddTask(new CTransparent(tp, CVector(18.0f, -2.648f, -18.5f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 0.84f, 0.6f)));//入口から見て左のステージ左のドア付近上側
	AddTask(new CTransparent(tp, CVector(43.64f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(16.0f, 0.3f, 7.0f)));//透明の壁(右)
	AddTask(new CTransparent(tp, CVector(17.39f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(16.0f, 0.3f, 7.0f))); //左の壁
	AddTask(new CTransparent(tp, CVector(38.1f, 6.0f, 15.83f), CVector(90.0f, 0.0f, 0.0f), CVector(49.0f, 1.27f, 3.0f)));//入口の横の壁(右)
	AddTask(new CTransparent(tp, CVector(45.7f, 5.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(26.0f, 0.5f, 7.0f)));//二階の床の壁(右)
	AddTask(new CTransparent(tp, CVector(44.75f, 2.4f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(5.5f, 0.8f, 7.0f)));//二階の床(右)
	AddTask(new CTransparent(tp, CVector(15.7f, 5.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(26.0f, 0.5f, 7.0f)));//二階の床の壁(左)
	AddTask(new CTransparent(tp, CVector(18.0f, 2.4f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(5.5f, 0.8f, 7.0f)));//二階の床(左)
	AddTask(new CTransparent(tp, CVector(46.75f, 10.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(20.0f, 10.0f, 9.0f)));//屋根
	AddTask(new CTransparent(tp, CVector(21.9f, 6.0f, 15.83f), CVector(90.0f, 0.0f, 0.0f), CVector(49.0f, 1.27f, 3.0f)));//入口の横の壁(左)
	AddTask(new CTransparent(tp, CVector(30.0f, 7.9f, 15.8f), CVector(90.0f, 0.0f, 0.0f), CVector(15.0f, 1.27f, 3.0f)));//入口の横の壁(真ん中)	
	AddTask(new CTransparent(tp, CVector(30.0f, 6.0f, -21.3f), CVector(90.0f, 0.0f, 0.0f), CVector(80.0f, 0.5f, 3.0f)));//ステージ裏
	AddTask(new CTransparent(tp, CVector(25.75f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f, 3.3f, 0.08)));//入口から見て左のステージ左のドア付近
	AddTask(new CTransparent(tp, CVector(45.65f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f, 3.3f, 0.08)));//入口から見て右のステージ右のドア付近
	AddTask(new CTransparent(tp, CVector(37.85f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(18.0f, 0.84f, 0.8f)));
	AddTask(new CTransparent(tp, CVector(42.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f)));//入口から見て左のステージ左のドア付近上側
	AddTask(new CTransparent(tp, CVector(18.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f)));//入口から見て右のステージ右のドア付近上側
	AddTask(new CTransparent(tp, CVector(37.0f, 10.0, -16.25f), CVector(0.0f, 0.0f, 90.0f), CVector(33.0f, 4.0f, 0.04)));//入口から見て真ん中のステージ
	AddTask(new CTransparent(tp, CVector(30.0f, 0.4f, -17.1f), CVector(90.0f, 0.0f, 0.0f), CVector(11.3f, 0.26f, 0.3f)));//演説台	

	//mEnemy2.Load(MODEL11);
	//new CEnemy2(&mEnemy2, CVector(-10.0f, -1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f));
	//mHata.Load(MODEL5);
	//new CHATA(&mHata, CVector(30.0f, 100.0f, -19.65f), CVector(), CVector(5.5f, 5.5f, 0.5f));


	//mpBackGround = GET_MODEL("BG");
	//ビルボードの作成
	//new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
	//親インスタンスと親行列はなし
	//mColliderMesh.Set(nullptr, nullptr, mpBackGround);

	CStageManager::Instance()->LoadStage(0);
}

void CGameScene::Update()
{
	CPlayer* player = CPlayer::Instance();

	if (player == nullptr) return;
	//カメラのパラメータを作成する
	CVector e, c, u;  //視点、注視点、上向き
	//視点を求める
	e = player->Position() + CVector(-0.2f, 3.0f, -5.0f) * player->MatrixRotate();
	//注視点を求める
	c = player->Position();
	//上向きを求める
	u = CVector(0.0f, 1.0f, 0.0f) * player->MatrixRotate();
	//カメラの設定
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

	if (player->IsDeath())
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
}