#include "CCamera.h"
#include "glut.h"
#include "Maths.h"

CCamera* CCamera::spMainCamera = nullptr;

// コンストラクタ
CCamera::CCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CObjectBase(ETag::eCamera, ETaskPriority::eCamera)
	, mIsMainCamera(isMainCamera)
	, mFollowTargetTf(nullptr)
	, mFollowOffsetPos(CVector::zero)
{
	Set(eye, center);

	// メインカメラであれば
	if (mIsMainCamera)
	{
		spMainCamera = this;
	}
}

// デストラクタ
CCamera::~CCamera()
{
	// 削除されたのがメインカメラであれば、
	// 登録しているメインカメラを解除
	if (spMainCamera == this)
	{
		spMainCamera = nullptr;
	}
}

// メインカメラを取得
CCamera* CCamera::MainCamera()
{
	return spMainCamera;
}

void CCamera::Set(const CVector& eye, const CVector& center)
{
	Position(eye);
	CVector forward = (center - eye).Normalized();
	CVector side = CVector::Cross(CVector::up, forward).Normalized();
	CVector up =  CVector::Cross(forward, side).Normalized();
	Rotation(CQuaternion::LookRotation(forward));

	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = Position() - mFollowTargetTf->Position();
	}
}

// 追従するターゲットを設定
void CCamera::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = Position() - mFollowTargetTf->Position();
	}
}

// 注視する位置を設定
void CCamera::LookAt(const CVector& pos)
{
	CVector forward = (pos - Position()).Normalized();
	CVector side = CVector::Cross(CVector::up, forward).Normalized();
	CVector up = CVector::Cross(forward, side).Normalized();
	Rotation(CQuaternion::LookRotation(forward));
}

// 更新
void CCamera::Update()
{
	// 追従するターゲットが設定されていれば、
	// カメラの位置を追従ターゲットの位置に合わせる
	if (mFollowTargetTf != nullptr)
	{
		Position(mFollowTargetTf->Position() + mFollowOffsetPos);
	}
}

// カメラ反映
void CCamera::Apply()
{
	CVector eye = CTransform::Position();
	CVector center = eye + VectorZ().Normalized();
	CVector up = CVector::up;
	gluLookAt
	(
		eye.X(), eye.Y(), eye.Z(),
		center.X(), center.Y(), center.Z(),
		up.X(), up.Y(), up.Z()
	);
}

// 2D用のカメラ行列を反映
void CCamera::Start2DCamera()
{
	// プロジェクション行列へ切り替えて、
	// 行列の退避と行列を初期化
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// 2Dの表示エリアを設定
	gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);

	// モデルビュー行列へ戻して、
	// 行列の退避と行列を初期化
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 2D描画に必要な設定を行う
	glDisable(GL_DEPTH_TEST);	// デプステストをオフ
	glDisable(GL_LIGHTING);		// ライティングをオフ
	glDisable(GL_CULL_FACE);	// カリング無効
}

// 3D用のカメラ行列へ戻す
void CCamera::End2DCamera()
{
	// 3D描画に必要な処理をオンにする
	glEnable(GL_DEPTH_TEST);	// デプステストをオン
	glEnable(GL_LIGHTING);		// ライティングをオン
	glEnable(GL_CULL_FACE);

	// プロジェクション行列へ切り替えて、
	// 2D描画開始時に退避した行列へ戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// モデルビュー行列へ戻して、
	// こちらも退避した行列へ戻す
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
