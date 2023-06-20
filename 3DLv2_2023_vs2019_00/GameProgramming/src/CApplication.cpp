#include "CApplication.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"

#include "CCollisionManager.h"

#include "CBillBoard.h"
#include "CEnemy3.h"

//クラスのstatic変数
CTexture CApplication::mTexture;
//CCharacterManager CApplication::mCharacterManager;
//CCamera CApplication::mCamera;

CUi* CApplication::spUi = nullptr;
CApplication::~CApplication()
{
	delete spUi;	//インスタンスUiの削除
}
CUi* CApplication::Ui()
{
	return spUi;
}

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル
//モデルデータの指定
#define MODEL_OBJ "res\\f14.obj", "res\\f14.mtl"
//敵輸送機モデル
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl"

//背景モデルデータの指定
#define MODEL_BACKGROUND  "res\\sky.obj", "res\\sky.mtl"


//CTaskManager CApplication::mTaskManager;

//CTaskManager* CApplication::TaskManager()
//{
//	return &mTaskManager;
//}

//CCamera* CApplication::Camera()
//{
//	return &mCamera;
//}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

//CCharacterManager* CApplication::CharacterManager()
//{
//	return &mCharacterManager;
//}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//3Dモデルファイルの読み込み
	mModelX.Load(MODEL_FILE);
	mXPlayer.Init(&mModelX);
	//mCharacter.Init(&mModelX);
	mFont.Load("FontG.png", 1, 4096 / 64);
	//spUi = new CUi();	//UIクラスの生成
	////モデルファイルの入力
	//mModel.Load(MODEL_OBJ);
	//mBackGround.Load(MODEL_BACKGROUND);
	//CMatrix matrix;
	//matrix.Print();

	//mEye = CVector(1.0f, 2.0f, 3.0f);

	//mPlayer.Model(&mModel);
	//mPlayer.Scale(CVector(0.1f, 0.1f, 0.1f));
	//mPlayer.Position(CVector(0.0f, 0.0f, -3.0f));
	//mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));

	////C5モデルの読み込み
	//mModelC5.Load(MODEL_C5);
	////敵機のインスタンス作成
	//new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f),
	//	CVector(), CVector(0.1f, 0.1f, 0.1f));
	//new CEnemy(&mModelC5, CVector(20.0f, 10.0f, -130.0f),
	//	CVector(), CVector(0.1f, 0.1f, 0.1f));

	////ビルボードの生成
	////new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);

	//////三角コライダの確認
	////mColliderTriangle.Set(nullptr, nullptr
	////	, CVector(-50.0f, 0.0f, -50.0f)
	////	, CVector(-50.0f, 0.0f, 50.0f)
	////	, CVector(50.0f, 0.0f, -50.0f));
	////mColliderTriangle2.Set(nullptr, nullptr
	////	, CVector(-50.0f, 0.0f, 50.0f)
	////	, CVector(50.0f, 0.0f, 50.0f)
	////	, CVector(50.0f, 0.0f, -50.0f));

	////背景モデルから三角コライダを生成
	////親インスタンスと親行列はなし
	//mColliderMesh.Set(nullptr, nullptr, &mBackGround);

	//new CEnemy3(CVector(-5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//new CEnemy3(CVector(5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));

}

void CApplication::Update()
{
	/*if (mCharacter.IsAnimationFinished())
	{
		if (mModelX.AnimationSet().size()>mCharacter.AnimationIndex())
		{
			mCharacter.ChangeAnimation(1 + mCharacter.AnimationIndex(), false, 60);
		}
	}*/
	//mXPlayer.ChangeAnimation(0, true, 60);
	//mModelX.AnimateFrame();
	//キャラクタクラスの更新
	mXPlayer.Update();
	//mXPlayer.ChangeAnimation(0, false, 60);
	//mCharacter.Update(CMatrix());
	/*mModelX.AnimationSet()[0]->Time(mModelX.AnimationSet()[0]->Time() + 1.0f);
	mModelX.AnimationSet()[0]->Time((int)mModelX.AnimationSet()[0]->Time() %
		(int)(mModelX.AnimationSet()[0]->MaxTime() + 1));*/
	////最初のアニメーションの現在時間を45にする
	////mModelX.AnimationSet()[0]->Time(0);
	////最初のアニメーションの重みを1.0(100%)にする
	//mModelX.AnimationSet()[0]->Weight(1.0f);
	////フレームの変換行列をアニメーションで更新する
	//mModelX.AnimateFrame();
	////フレームの合成行列を計算する
	//mModelX.Frames()[0]->AnimateCombined(&mMatrix);
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);
	//カメラの設定
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	if (mInput.Key('I'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(-1);
	}
	if (mInput.Key('J'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(-1);
	}

	//X軸＋回転
	if (mInput.Key('K'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(1);
	}
	//Y軸＋回転
	if (mInput.Key('L'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(1);
	}
	//行列設定
	glMultMatrixf(mMatrix.M());

	//頂点にアニメーションを適用する
	mModelX.AnimateVertex();
	//モデル描画
	//mModelX.Render();
	mXPlayer.Render();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12,"3D PROGRAMMING");

	//2Dの描画終了
	CCamera::End();
	////タスクマネージャの更新
	////mTaskManager.Update();
	//CTaskManager::Instance()->Update();
	////コリジョンマネージャの衝突処理
	////削除	CCollisionManager::Instance()->Collision();
	//CTaskManager::Instance()->Collision();

	////頂点1､頂点2､頂点3,法線データの作成
	//CVector v0, v1, v2, n;
	////法線を上向きで設定する
	//n.Set(0.0f, 1.0f, 0.0f);
	////頂点1の座標を設定する
	//v0.Set(0.0f, 0.0f, 0.5f);
	////頂点2の座標を設定する
	//v1.Set(1.0f, 0.0f, 0.0f);
	////頂点3の座標を設定する
	//v2.Set(0.0f, 0.0f, -0.5f);

	//if (mInput.Key('J'))
	//{
	//	mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	//}
	//if (mInput.Key('L'))
	//{
	//	mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	//}
	//if (mInput.Key('I'))
	//{
	//	mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	//}
	//if (mInput.Key('K'))
	//{
	//	mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	//}	
	//if (mInput.Key('O'))
	//{
	//	mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
	//}
	//if (mInput.Key('M'))
	//{
	//	mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	//}

	////mPlayer.Update();

	////カメラのパラメータを作成する
	//CVector e, c, u;//視点、注視点、上方向
	////視点を求める
	//e = mPlayer.Position() + CVector(-0.2f, 1.0f, -3.0f) * mPlayer.MatrixRotate();
	////注視点を求める
	//c = mPlayer.Position();
	////上方向を求める
	//u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
	////カメラの設定
	//gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	////モデルビュー行列の取得
	//glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	////逆行列の取得
	//mModelViewInverse = mModelViewInverse.Transpose();
	//mModelViewInverse.M(0, 3, 0);
	//mModelViewInverse.M(1, 3, 0);
	//mModelViewInverse.M(2, 3, 0);

	////mPlayer.Render();

	//mBackGround.Render();

	////タスクリストの削除
	//CTaskManager::Instance()->Delete();
	////タスクマネージャの描画
	//CTaskManager::Instance()->Render();

	//CCollisionManager::Instance()->Render();

	//spUi->Render();	//UIの描画

}
