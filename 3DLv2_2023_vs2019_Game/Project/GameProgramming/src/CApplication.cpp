#include "CApplication.h"
//OpenGL
#include"glut.h"
#include "CRectangle.h"
#include"CVector.h"
#include"CBillBoard.h"
#include "CResourceManager.h"
#include "CStageManager.h"

//#define TEXTYA "AvatarHeadMale_Color_2K.jpeg"

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

CApplication::~CApplication()
{
	delete spUi;  //インスタンスUiの削除
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui() 
{
	return spUi;  //インスタンスのポインタを返す
}

CMatrix  CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

void CApplication::Start()
{
	LOAD_MODEL("Stage", "res\\体育館.obj", "res\\体育館.mtl");
	LOAD_MODEL("Kokki", "res\\国旗.obj", "res\\国旗.mtl");
	LOAD_MODEL("Enemy1", "res\\敵1.obj", "res\\敵1.mtl");
	LOAD_MODEL("Police", "res\\サボるポリス.obj", "res\\サボるポリス.mtl");
	LOAD_MODEL("Transparent", "res\\透明の壁.obj", "res\\透明の壁.mtl");
	LOAD_MODEL("Frame", "res\\枠緑色.obj", "res\\枠緑色.mtl");
	LOAD_MODEL("Clear", "res\\クリア枠.obj", "res\\クリア枠.mtl");
	LOAD_MODEL("Enemy2", "res\\銃持ち敵.obj", "res\\銃持ち敵.mtl");
	LOAD_MODEL("Player", "res\\obama4.obj", "res\\obama4.mtl");
	LOAD_MODEL("BG", "res\\sky.obj", "res\\sky.mtl");

	spUi = new CUi();  //UIクラスの生成
	spUi->Hp2(CClear::Hp());
	spUi->Hp(CPlayer::Hp());

	new CStage(GET_MODEL("Stage"), CVector(30.0f, -0.99f, 0.0f), CVector(), CVector(1.0f, 1.0f, 1.0f));
	CModel* kokki = GET_MODEL("Kokki");
	//new CKokki(&mKokki, CVector(-10.0f, 1.0f, 0.0f), CVector(0.0f, -90.0f, 0.0f), CVector(5.5f, 5.5f,0.5f));
	new CKokki(kokki, CVector(30.0f, 4.0f, -19.65f), CVector(0.0f, -90.0f, 0.0f), CVector(0.5f, 0.5f,0.5f));
	//new CKokki(&mKokki, CVector(30.0f, -1.2f, -17.6f), CVector(0.0f, 0.0f, 0.0f), CVector(48.4f, 0.84f, 0.95f));
	new CKokki(kokki, CVector(23.55f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));//透明の入口から見て左のステージ袖の階段
	new CKokki(kokki, CVector(23.25f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(22.95f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(22.65f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(22.35f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(23.25f, -0.2f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.1f));//左のステージ袖の階段の前の板

	new CKokki(kokki, CVector(36.45f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));//右のステージ袖の階段
	new CKokki(kokki, CVector(36.75f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(37.05f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(37.35f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(37.65f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(36.77, -0.654f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.3f)); //右のステージ袖の階段の前の板

	CModel*tp= GET_MODEL("Transparent");
	new CTransparent(tp, CVector(30.0f, -2.648f, -17.6f), CVector(0.0f, 0.0f, 0.0f), CVector(48.4f, 0.84f, 0.95f));//-2.649
	new CTransparent(tp, CVector(22.15f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(18.0f, 0.84f, 0.8f));
	new CTransparent(tp, CVector(18.0f, -2.648f, -18.5f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 0.84f, 0.6f));//入口から見て左のステージ左のドア付近上側
	new CTransparent(tp, CVector(43.64f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(16.0f, 0.3f, 7.0f));//透明の壁(右)
	new CTransparent(tp, CVector(17.39f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(16.0f, 0.3f, 7.0f)); //左の壁
	new CTransparent(tp, CVector(38.1f, 6.0f, 15.83f), CVector(90.0f, 0.0f, 0.0f), CVector(49.0f, 1.27f, 3.0f));//入口の横の壁(右)
	new CTransparent(tp, CVector(45.7f, 5.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(26.0f, 0.5f, 7.0f));//二階の床の壁(右)
	new CTransparent(tp, CVector(44.75f, 2.4f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(5.5f, 0.8f, 7.0f));//二階の床(右)
	new CTransparent(tp, CVector(15.7f, 5.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(26.0f, 0.5f, 7.0f));//二階の床の壁(左)
	new CTransparent(tp, CVector(18.0f, 2.4f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(5.5f, 0.8f, 7.0f));//二階の床(左)
	new CTransparent(tp, CVector(46.75f, 10.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(20.0f, 10.0f, 9.0f));//屋根
	new CTransparent(tp, CVector(21.9f, 6.0f, 15.83f), CVector(90.0f, 0.0f, 0.0f), CVector(49.0f, 1.27f, 3.0f));//入口の横の壁(左)
	new CTransparent(tp, CVector(30.0f, 7.9f, 15.8f), CVector(90.0f, 0.0f, 0.0f), CVector(15.0f, 1.27f, 3.0f));//入口の横の壁(真ん中)	
	new CTransparent(tp, CVector(30.0f, 6.0f, -21.3f), CVector(90.0f, 0.0f, 0.0f), CVector(80.0f, 0.5f, 3.0f));//ステージ裏
	new CTransparent(tp, CVector(25.75f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f,3.3f, 0.08));//入口から見て左のステージ左のドア付近
	new CTransparent(tp, CVector(45.65f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f, 3.3f, 0.08));//入口から見て右のステージ右のドア付近
	new CTransparent(tp, CVector(37.85f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(18.0f, 0.84f, 0.8f));
	new CTransparent(tp, CVector(42.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f));//入口から見て左のステージ左のドア付近上側
	new CTransparent(tp, CVector(18.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f));//入口から見て右のステージ右のドア付近上側
	new CTransparent(tp, CVector(37.0f, 10.0, -16.25f), CVector(0.0f, 0.0f, 90.0f), CVector(33.0f, 4.0f, 0.04));//入口から見て真ん中のステージ
	new CTransparent(tp, CVector(30.0f, 0.4f, -17.1f), CVector(90.0f, 0.0f, 0.0f), CVector(11.3f,0.26f, 0.3f));//演説台	


	CModel* frameModel = GET_MODEL("Frame");
	new CFrame(frameModel, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	CModel* clearModel = GET_MODEL("Clear");
	new CClear(clearModel, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//mEnemy2.Load(MODEL11);
	//new CEnemy2(&mEnemy2, CVector(-10.0f, -1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f));
	//mHata.Load(MODEL5);
	//new CHATA(&mHata, CVector(30.0f, 100.0f, -19.65f), CVector(), CVector(5.5f, 5.5f, 0.5f));

	CModel* playerMode = GET_MODEL("Player");
	mPlayer.Model(playerMode);
	mPlayer.Position(CVector(30.0f, 3.0f, 1.0f));
	mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));
	mPlayer.Scale(CVector(1.0f, 1.0f, 1.0f));//10.0f, 10.0f, 10.0f
	CMatrix matrix;
	matrix.print();
	mEye = CVector(1.0f, 2.0f, 3.0f);
	mpBackGround = GET_MODEL("BG");
	//ビルボードの作成
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
	//親インスタンスと親行列はなし
	mColliderMesh.Set(nullptr, nullptr, mpBackGround);

	CStageManager::Instance()->LoadStage(0);
	mState = EState::ESTART;
	//mpStage1 = new CStage1()
}

void CApplication::Update()
{
	switch (mState)
	{
	case EState::ESTART:	//状態がスタート
		mpStage1->Start();	//スタート画面表示
		//Enterキーが押されたら
		if (mInput.Key(VK_RETURN))
		{	//状態をプレイ中にする
			mState = EState::EPLAY;
		}
		break;
	//case EState::EPLAY:
	//	//ゲームオーバーか判定
	//	if (mpStage1->IsOver())
	//	{	//状態をゲームオーバーにする
	//		mState = EState::EOVER;
	//	}
	//	//ゲームクリアか判定
	//	if (mpStage1->IsClear())
	//	{	//状態をゲームクリアにする
	//		mState = EState::ECLEAR;
	//	}
	//	break;
	//case EState::EOVER:
	//	//ゲームオーバー処理
	//	mpStage1->Over();
	//	//エンターキー入力時
	//	if (mInput.Key(VK_RETURN))
	//	{	//ゲームのインスタンス削除
	//		delete mpStage1;
	//		//ゲームのインスタンス生成
	//		mpStage1 = new CStage1();
	//		//状態をスタートにする
	//		mState = EState::ESTART;
	//	}
	//	break;
	case EState::ECLEAR:
		//ゲームクリア処理
		mpStage1->Clear();
		if (mInput.Key(VK_RETURN))
		{
			delete mpStage1;
			mpStage1 = new CStage1();
			mState = EState::ESTART;
		}
		break;
	}
	if (mInput.PullKey('1'))
	{
		CStageManager::Instance()->LoadStage(0);
	}
	else if (mInput.PullKey('2'))
	{
		CStageManager::Instance()->LoadStage(1);
	}
	spUi->Hp2(CClear::Hp());
	spUi->Hp(CPlayer::Hp());
	//タスクマネージャの更新
	CTaskManager::Instance()->Update();
	CTaskManager::Instance()->Collision();
	//コリジョンマネージャの衝突処理
	//CCollisionManager::Instance()->Collision();
	//CTaskManager::Instance()->Collision();
	//頂点1、頂点2、頂点3,法線データの作成
	CVector v0, v1, v2, n;
	//法線を上向きで設定する
	n.Set(0.0f, 1.0f, 0.0f);
	//頂点1の座標を設定する
	v0.Set(0.0f, 0.0f, 0.5f);
	//頂点2の座標を設定する
	v1.Set(1.0f, 0.0f, 0.0f);
	//頂点3の座標を設定する
	v2.Set(0.0f, 0.0f, -0.5f);
	if (mInput.Key('J'))
	{
		mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('I'))
	{
		mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	}
	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
	}
	//視点の設定
	//gluLookAt(視点X,視点Y,視点Z,中心X,中心Y,中心Z,上向X,上向Y,上向Z)
	//gluLookAt(mEye.X(), mEye.Y(), mEye.Z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	CTransform trans;  //変換行列インスタンスの作成
	trans.Position(CVector(1.0f, 4.0f, 2.0f));//位置の設定
	trans.Rotation(CVector(-10.0f, -20.0f, -30.0f));  //回転の設定
	trans.Scale(CVector(1.0f, 1.0f, 1.0f));//拡大縮小の設定

	trans.Update(); //行列の更新
	//mPlayer.Update();
	//カメラのパラメータを作成する
	CVector e, c, u;  //視点、注視点、上向き
	//視点を求める
	e = mPlayer.Position() + CVector(-0.2f, 3.0f, -5.0f) * mPlayer.MatrixRotate();
		//注視点を求める
		c = mPlayer.Position();
	//上向きを求める
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
		//モデルビュー行列の取得
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
		//逆行列の取得dd
		mModelViewInverse = mModelViewInverse.Transpose();
		mModelViewInverse.M(0, 3, 0);
		mModelViewInverse.M(1, 3, 0);
		mModelViewInverse.M(2, 3, 0);
	//mPlayer.Render();
	mpBackGround->Render();
	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャの描画
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();
	spUi->Render();  //UIの描画
}
