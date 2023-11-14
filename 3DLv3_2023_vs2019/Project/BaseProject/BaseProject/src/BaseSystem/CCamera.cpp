#include "CCamera.h"
#include "glut.h"
#include "Maths.h"
#include "CDebugCamera.h"

CCamera* CCamera::spMainCamera = nullptr;

// コンストラクタ
CCamera::CCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CObjectBase(ETag::eCamera, ETaskPriority::eCamera)
	, mIsMainCamera(isMainCamera)
	, mFollowTargetTf(nullptr)
	, mFollowOffsetPos(CVector::zero)
	, mFovy(CAMERA_FOVY)
	, mZNear(CAMERA_ZNEAR)
	, mZFar(CAMERA_ZFAR)
{
	LookAt(eye, center, CVector::up);
	Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);

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

// 現在のカメラを取得
CCamera* CCamera::CurrentCamera()
{
	// デバッグカメラが有効であれば、
	if (CDebugCamera::IsOn())
	{
		// デバッグカメラを返す
		return CDebugCamera::DebugCamera();
	}
	// デバッグカメラが無効であれば、
	else
	{
		// メインカメラを返す
		return spMainCamera;
	}
}

// カメラの状態をコピー
void CCamera::CopyCamera(CCamera* copy)
{
	if (copy == nullptr) return;

	Position(copy->Position());
	Rotation(copy->Rotation());
	mViewMatrix = copy->mViewMatrix;
	mProjectionMatrix = copy->mProjectionMatrix;
	mViewportMatrix = copy->mViewportMatrix;
	mFovy = copy->mFovy;
	mZNear = copy->mZNear;
	mZFar = copy->mZFar;
	mEye = copy->mEye;
	mAt = copy->mAt;
	mUp = copy->mUp;
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

// 追従時のオフセット座標を設定
void CCamera::SetFollowTargetOffset(const CVector& offset)
{
	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = offset;
	}
}

// 注視する位置を設定（注視点のみ）
void CCamera::LookAt(const CVector& pos)
{
	LookAt(mEye, pos, mUp);
}

// 注視する位置を設定（注視点 + 上ベクトル）
void CCamera::LookAt(const CVector& pos, const CVector& up)
{
	LookAt(mEye, pos, up);
}

// 注視する位置を設定（視点 + 注視点 + 上ベクトル）
void CCamera::LookAt(const CVector& eye, const CVector& at, const CVector& up)
{
	mEye = eye; mAt = at; mUp = up;
	mViewMatrix.Identity();
	CVector f = (mEye - mAt).Normalized();
	CVector r = CVector::Cross(mUp, f).Normalized();
	CVector u = CVector::Cross(f, r).Normalized();
	mViewMatrix.M(0, 0, r.X()); mViewMatrix.M(1, 0, r.Y()); mViewMatrix.M(2, 0, r.Z());
	mViewMatrix.M(0, 1, u.X()); mViewMatrix.M(1, 1, u.Y()); mViewMatrix.M(2, 1, u.Z());
	mViewMatrix.M(0, 2, f.X()); mViewMatrix.M(1, 2, f.Y()); mViewMatrix.M(2, 2, f.Z());
	mViewMatrix.M(3, 0, -CVector::Dot(mEye, r));
	mViewMatrix.M(3, 1, -CVector::Dot(mEye, u));
	mViewMatrix.M(3, 2, -CVector::Dot(mEye, f));

	Position(mEye);
	Rotation(CQuaternion::LookRotation(f));

	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = Position() - mFollowTargetTf->Position();
	}
}

// 画面サイズ変更処理
void CCamera::Reshape(int width, int height)
{
	//画面の描画エリアの指定
	Viewport(0.0f, 0.0f, width, height);

	Perspective
	(
		CAMERA_FOVY,
		(float)width / height,
		CAMERA_ZNEAR,
		CAMERA_ZFAR
	);
}

// 画面の描画エリアを指定
void CCamera::Viewport(float x, float y, float w, float h)
{
	glViewport(x, y, w, h);
	mViewportMatrix.Identity();
	mViewportMatrix.M(0, 0, w * 0.5f);
	mViewportMatrix.M(3, 0, x + w * 0.5f);
	mViewportMatrix.M(1, 1, -h * 0.5f);
	mViewportMatrix.M(3, 1, y + h * 0.5f);
}

// 透視投影行列を設定
void CCamera::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	//各値を設定
	mFovy = fovy;
	mZNear = zNear;
	mZFar = zFar;

	//プロジェクション行列へ切り替え
	glMatrixMode(GL_PROJECTION);
	//行列を初期化して、新しいプロジェクション行列を設定
	glLoadIdentity();
	gluPerspective(mFovy, aspect, mZNear, mZFar);
	//プロジェクション行列を記憶しておく
	glGetFloatv(GL_PROJECTION_MATRIX, mProjectionMatrix.M());

	//モデルビュー行列へ戻す
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// スクリーン座標からワールド座標へ変換
CVector CCamera::ScreenToWorldPos(const CVector& screenPos)
{
	float z = screenPos.Z();
	float d = ((z * (mZFar + mZNear) - 2.0f * mZFar * mZNear) / (mZFar - mZNear) / z);
	CVector4 v =
		mViewportMatrix.Inverse() *
		mProjectionMatrix.Inverse() *
		mViewMatrix.Inverse() *
		CVector4(screenPos.X(), screenPos.Y(), d, 1.0f);
	return CVector(v.X() / v.W(), v.Y() / v.W(), v.Z() / v.W());
}

// ワールド座標からスクリーン座標へ変換
CVector CCamera::WorldToScreenPos(const CVector& worldPos)
{
	CVector4 v =
		mViewMatrix *
		mProjectionMatrix *
		mViewportMatrix *
		CVector4(worldPos, 1.0f);
	return CVector(v.X() / v.W(), v.Y() / v.W(), v.Z() / fabsf(v.W()));
}

// 更新
void CCamera::Update()
{
	// 追従するターゲットが設定されていれば、
	// カメラの位置を追従ターゲットの位置に合わせる
	if (mFollowTargetTf != nullptr)
	{
		Position(mFollowTargetTf->Position() + mFollowOffsetPos);
		mEye = Position();
		mAt = mEye + -VectorZ().Normalized();
	}

	LookAt(mEye, mAt, mUp);
}

// カメラ反映
void CCamera::Apply()
{
	glMultMatrixf(mViewMatrix.M());
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
