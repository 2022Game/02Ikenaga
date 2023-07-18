#include "CXPlayer.h"
#include "CActionCamera.h"

#define ROTATION_YV	CVector(0.0f, 2.0f, 0.0f) //回転速度
#define VELOCITY CVector(0.0f, 0.0f, 0.1f) //移動速度
#define ROTATION_XV	CVector(1.0f, 0.0f, 0.0f) //回転速度

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	// //体
	mColSphereBody.Matrix(&mpCombinedMatrix[9]);
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[12]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[22]);
}

CXPlayer::CXPlayer()
	:mColSphereBody(this, nullptr, CVector(), 0.5f,CCollider::ETag::EBODY)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	mTag = EPLAYER;
}

void CXPlayer::Update()
{
	if (mInput.Key(VK_SPACE))
	{
		int mTime = 0;
		if (mTime < 30)
		{
			mTime++;
			if (CXCharacter::mAnimationIndex != 3)
			{
				if (mAnimationLoopFlg == true)
				{
					CXCharacter::ChangeAnimation(3, false, 30);
					if (CXCharacter::IsAnimationFinished() && mAnimationIndex)
					{
						if (mAnimationLoopFlg != true)
						{
							CXCharacter::ChangeAnimation(4, false, 30);
						}
						if (CXCharacter::IsAnimationFinished() && mAnimationIndex)
						{
							CXCharacter::ChangeAnimation(0, true, 60);
						}
					}
				}
			}
			mTime = 0;
		}
	}
	if (CXCharacter::IsAnimationFinished() && mAnimationIndex)
	{
		if (mAnimationLoopFlg != true)
		{
			CXCharacter::ChangeAnimation(4, false, 30);
		}
		if (CXCharacter::IsAnimationFinished() && mAnimationIndex)
		{
			CXCharacter::ChangeAnimation(0, true, 60);
		}
	}
	/*if (mInput.Key('A'))
	{
		if (mAnimationLoopFlg)
		{
			mRotation = mRotation + ROTATION_YV;
		}
	}
	if (mInput.Key('D'))
	{
		if (mAnimationLoopFlg)
		{
			mRotation = mRotation - ROTATION_YV;
		}
	}*/
	if (mInput.Key('W') || mInput.Key('S') || mInput.Key('A') || mInput.Key('D'))
	{
		if (mAnimationLoopFlg)
		{
			mPosition = mPosition + VELOCITY * mMatrixRotate;
			CXCharacter::ChangeAnimation(1, true, 60);
		}
	}
	else if (mAnimationLoopFlg)
	{
		CXCharacter::ChangeAnimation(0, true, 60);
	}
	//カメラ䛾前方
	CVector cameraZ = CActionCamera::Instance()->VectorZ();
	//カメラ䛾左方向
	CVector cameraX = CActionCamera::Instance()->VectorX();
	//キャラクタ䛾前方
	CVector charZ = mMatrixRotate.VectorZ();
	//XZ平面にして正規化
	cameraZ.Y(0.0f); cameraZ = cameraZ.Normalize();
	cameraX.Y(0.0f); cameraX = cameraX.Normalize();
	charZ.Y(0.0f); charZ = charZ.Normalize();
	//移動方向䛾設定
	CVector move;
	if (mInput.Key('A')) {
		move = move + cameraX;
	}
	if (mInput.Key('D')) {
		move = move - cameraX;
	}
	if (mInput.Key('W')) {
		move = move + cameraZ;
	}
	if (mInput.Key('S')) {
		move = move - cameraZ;
		//ChangeAnimation(1, true, 60);
	}
	//移動あり
	if (move.Length() > 0.0f)
	{
		//遊び
		const float MARGIN = 0.06f;
		//正規化
		move = move.Normalize();
		//自分䛾向きと向かせたい向きで外積
		float cross = charZ.Cross(move).Y();
		//自分䛾向きと向かせたい向きで内積
		float dot = charZ.Dot(move);
		//外積がプラス䛿左回転
		if (cross > MARGIN) {
			mRotation.Y(mRotation.Y() + 5.0f);
		}
		//外積がマイナス䛿右回転
		else if (cross < -MARGIN) {
			mRotation.Y(mRotation.Y() - 5.0f);
		}
		//前後䛾向きが同じとき内積䛿 1.0
		else if (dot < 1.0f - MARGIN) {
			mRotation.Y(mRotation.Y() - 5.0f);
		}
		//移動方向へ移動
		//mPosition = mPosition + move * 0.1f;
		//ChangeAnimation(1, true, 60);
	}
	CXCharacter::Update();
}