#include "CXPlayer.h"

#define ROTATION_YV	CVector(0.0f, 2.0f, 0.0f) //回転速度
#define VELOCITY CVector(0.0f, 0.0f, 0.1f) //移動速度
#define ROTATION_XV	CVector(1.0f, 0.0f, 0.0f) //回転速度

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
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
	if (mInput.Key('A'))
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
	}
	if (mInput.Key('W'))
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
	CXCharacter::Update();
}