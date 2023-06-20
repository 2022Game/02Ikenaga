#include "CXPlayer.h"

#define ROTATION_YV	CVector(0.0f, 2.0f, 0.0f) //‰ñ“]‘¬“x
#define VELOCITY CVector(0.0f, 0.0f, 0.1f) //ˆÚ“®‘¬“x
#define ROTATION_XV	CVector(1.0f, 0.0f, 0.0f) //‰ñ“]‘¬“x

void CXPlayer::Update()
{
	if (mInput.Key(VK_SPACE))
	{
		if (CXCharacter::mAnimationIndex != 3)
		{
			CXCharacter::ChangeAnimation(3, false, 30);
		}
	}
	else if (CXCharacter::IsAnimationFinished() && mAnimationIndex)
	{
		CXCharacter::ChangeAnimation(4, false, 30);
		if (CXCharacter::IsAnimationFinished() && mAnimationIndex)
		{
			CXCharacter::ChangeAnimation(0, true, 60);
		}
	}
	else
	{
		if (mInput.Key('W'))
		{
			if (mAnimationLoopFlg)
			{
			mPosition = mPosition + VELOCITY * mMatrixRotate;
				CXCharacter::ChangeAnimation(1, true, 60);
			}
		}
		else if (mInput.Key('A'))
		{
			mRotation = mRotation + ROTATION_YV;
		}
		else if (mInput.Key('D'))
		{
			mRotation = mRotation - ROTATION_YV;
		}
		else if (mAnimationLoopFlg)
		{
			CXCharacter::ChangeAnimation(0, true, 60);
		}
	}
	CXCharacter::Update();
}