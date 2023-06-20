#include "CApplication.h"
//OpenGL
#include"glut.h"
#include "CRectangle.h"
#include"CVector.h"
#include"CBillBoard.h"

//#include"CTriangle.h"

//クラスのstatic変数
CTexture CApplication::mTexture;
//CTexture CApplication::mTexture1;
CCharacterManager CApplication::mCharacterManager;

//#define TEXTYA "AvatarHeadMale_Color_2K.jpeg"

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル
#define MODEL_OBJ "res\\f14.obj" ,"res\\f14.mtl"//モデルデータの指定
#define MODEL_1 "res\\Smartphone.obj", "res\\Smartphone.mtl"
#define MODEL_2 "res\\obama4.obj","res\\obama4.mtl"
//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"
//敵輸送機モデル
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"

#define MODEL3 "res\\体育館.obj","res\\体育館.mtl"
#define MODEL4 "res\\国旗.obj","res\\国旗.mtl"
//#define MODEL5 "res\\新旗.obj","res\\新旗.mtl"
#define MODEL6  "res\\敵1.obj","res\\敵1.mtl"
#define MODEL7  "res\\サボるポリス.obj","res\\サボるポリス.mtl"
#define MODEL8 "res\\透明の壁.obj","res\\透明の壁.mtl"
#define MODEL9 "res\\枠緑色.obj","res\\枠緑色.mtl"
#define MODEL10 "res\\クリア枠.obj","res\\クリア枠.mtl"
//CTaskManager CApplication::mTaskManager;
//CTaskManager* CTaskManager::Instance()
//{
//	return mpInstance;
//}

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

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

//CTexture* CApplication::Texture1()
//{
//	return &mTexture1;
//}
//

void CApplication::Start()
{
	spUi = new CUi();  //UIクラスの生成
	//三角コライダの確認
	//mColliderTriangle.Set(nullptr, nullptr, CVector(10.0f, 10.0f, -10.0f), CVector(-50.0f, 0.0f, 50.0f), CVector(50.0f, 0.0f, 50.0f));
	//mColliderTriangle2.Set(nullptr, nullptr, CVector(-100.0f, 10.0f, -100.0f), CVector(-50.0f, -4.0f, 50.0f), CVector(50.0f, -4.0f, 50.0f));
	//new CEnemy3(CVector(-5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//new CEnemy3(CVector(5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	mStage.Load(MODEL3);
	new CStage(&mStage, CVector(30.0f, -0.99f, 0.0f), CVector(), CVector(1.0f, 1.0f, 1.0f));
	mKokki.Load(MODEL4);
	//new CKokki(&mKokki, CVector(-10.0f, 1.0f, 0.0f), CVector(0.0f, -90.0f, 0.0f), CVector(5.5f, 5.5f,0.5f));
	new CKokki(&mKokki, CVector(30.0f, 4.0f, -19.65f), CVector(0.0f, -90.0f, 0.0f), CVector(0.5f, 0.5f,0.5f));
	new CKokki(&mKokki, CVector(30.0f, -1.2f, -17.6f), CVector(0.0f, 0.0f, 0.0f), CVector(48.4f, 0.84f, 0.95f));
	new CKokki(&mKokki, CVector(23.55f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));//透明の入口から見て左のステージ袖の階段
	new CKokki(&mKokki, CVector(23.25f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(22.95f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(22.65f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(22.35f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(23.25f, -0.2f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.1f));//左のステージ袖の階段の前の板

	new CKokki(&mKokki, CVector(36.45f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));//右のステージ袖の階段
	new CKokki(&mKokki, CVector(36.75f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(37.05f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(37.35f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(37.65f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(&mKokki, CVector(36.77, -0.654f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.3f)); //右のステージ袖の階段の前の板

	mEnemy1.Load(MODEL6);
	//敵機のインスタンス作成
	//new CEnemy(&mEnemy1, CVector(30.0f, -1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f));
	mPolice.Load(MODEL7);
	//new CPolice(&mPolice, CVector(20.0f, 2.5f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	mTransparent.Load(MODEL8);
	new CKokki(&mKokki, CVector(43.17f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(12.0f, 0.3f, 7.0f));//右の壁
	//new CTransparent(&mTransparent, CVector(23.17f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(12.0f, 0.3f, 7.0f));//透明の壁(右)43.17
	new CKokki(&mKokki, CVector(37.1f, 0.65f, 16.6f), CVector(90.0f, 0.0f, 0.0f), CVector(42.7f, 0.3f, 5.0f));//入口の横の壁(右)
	new CKokki(&mKokki, CVector(22.0f, 6.0f, 16.6f), CVector(90.0f, 0.0f, 0.0f), CVector(49.0f, 0.3f, 3.0f));//入口の横の壁(左)
	new CKokki(&mKokki, CVector(16.86f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(12.0f, 0.3f, 7.0f)); //左の壁
	
	mFrame.Load(MODEL9);
	new CFrame(&mFrame, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	mClear.Load(MODEL10);
	new CClear(&mClear, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//mHata.Load(MODEL5);
	//new CHATA(&mHata, CVector(30.0f, 100.0f, -19.65f), CVector(), CVector(5.5f, 5.5f, 0.5f));

	//敵機のインスタンス作成
	//new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//C5モデルの読み込み
	//mModelC5.Load(MODEL_C5);
	//mCharacter.Model(&mModel);
	//mCharacter.Scale(CVector(0.1f, 0.1f, 0.1f));
	mModel.Load(MODEL_2);
	mPlayer.Model(&mModel);
	mPlayer.Position(CVector(30.0f, 3.0f, 1.0f));
	mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));
	mPlayer.Scale(CVector(1.0f, 1.0f, 1.0f));//10.0f, 10.0f, 10.0f
	CMatrix matrix;
	matrix.print();
	mEye = CVector(1.0f, 2.0f, 3.0f);
	//モデルファイルの入力
	//mModel.Load(MODEL_OBJ);
	mBackGround.Load(MODEL_BACKGROUND);
	//ビルボードの作成
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//背景モデルから三角コライダを生成
	//親インスタンスと親行列はなし
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

void CApplication::Update()
{
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
	mBackGround.Render();
	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャの描画
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();
	spUi->Render();  //UIの描画
	//mPlayer.bullet.Update();
	//mPlayer.bullet.Render();
	//mCharacter.Update();
	//mCharacter.Render();
	//mModel.Render(trans.Matrix());
	//CMatrix matrix, position, rotation, scale;
	//rotation.RotateY(180.0f);  //回転行列設定
	//mModel.Render(matrix);  //モデルの描画
	//描画開始
	//glBegin(形)
	//GL_TRIANGLES:三角形
//	glBegin(GL_TRIANGLES);
//
//	//法線(面の向き)の設定
//	glNormal3f(n.X(), n.Y(), n.Z());
//	//頂点座標の設定
//	glVertex3f(v0.X(),v0.Y(),v0.Z());
//	glVertex3f(v1.X(),v1.Y(),v1.Z());
//	glVertex3f(v2.X(), v2.Y(), v2.Z());
//
//	//法線と頂点の設定
//	n.Set(0.0f, 0.0f, 1.0f);
//	v0.Set(0.5f, 0.0f, 0.0f);
//	v1.Set(0.0f, 1.0f, 0.0f);
//	v2.Set(-0.5f, 0.0f, 0.0f);
//	//三角形2の描画
//	glNormal3f(n.X(), n.Y(), n.Z());
//	glVertex3f(v0.X(), v0.Y(), v0.Z());
//	glVertex3f(v1.X(), v1.Y(), v1.Z());
//	glVertex3f(v2.X(), v2.Y(), v2.Z());
//
//	//法線と頂点の設定
//	n.Set(1.0f, 0.0f, 0.0f);
//	v0.Set(0.0f, 0.5f, 0.0f);
//	v1.Set(0.0f, 0.0f, 1.0f);
//	v2.Set(0.0f, -0.5f, 0.0f);
//	//三角形2の描画
//	glNormal3f(n.X(), n.Y(), n.Z());
//	glVertex3f(v0.X(), v0.Y(), v0.Z());
//	glVertex3f(v1.X(), v1.Y(), v1.Z());
//	glVertex3f(v2.X(), v2.Y(), v2.Z());
//
//	//面の向きはZ軸方向
//	//glNormal3f(0.0f, 0.0f, 1.0f);
//	//glVertex3f(0.0f, 0.0f, 0.0f);
//	//glVertex3f(0.0f, 1.0f, 0.0f);
//	//glVertex3f(-0.5f, 0.0f, 0.0f);
//
//	//glNormal3f(1.0f, 0.0f, 0.0f);
//	//glVertex3f(0.0f, -0.5f, 0.0f);
//	//glVertex3f(0.0f, 0.0f, 0.0f);
//	//glVertex3f(0.0f, 0.0f, 1.0f);
//
//	//三角形クラスのインスタンス作成
//	CTriangle t0;
//	//法線と頂点の設定
//	t0.Vertex(CVector(1.0f, 0.0f, 0.5f), CVector(2.0f, 0.0f, 0.0f), CVector(1.0f, 0.0f, -0.5f));
//	t0.Normal(CVector(0.0f, 1.0f, 0.0f));
//	//三角形の描画
//	t0.Render();
//
//	//三角形クラスのインスタンス作成
//	CTriangle t1;
//	//法線と頂点の設定
//	t1.Vertex(CVector(0.5f, 1.0f, 0.0f), CVector(0.0f, 2.0f, 0.0f), CVector(-0.5f, 1.0f, 0.0f));
//	t1.Normal(CVector(1.0f, 0.0f, 0.0f));
//	//三角形の描画
//	t1.Render();
//
//	//三角形クラスのインスタンス作成
//	CTriangle t2;
//	//法線と頂点の設定
//	t2.Vertex(CVector(0.0f, 0.0f, 2.0f), CVector(0.0f, -0.5f, 1.0f), CVector(0.0f, 0.5f, 1.0f));
//	t2.Normal(CVector(1.0f, 0.0f, 0.0f));
//	//三角形の描画
//	t2.Render();
//
//	//描画終了
//	//glEnd();
}
