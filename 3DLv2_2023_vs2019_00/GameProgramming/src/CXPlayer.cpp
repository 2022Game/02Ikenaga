#include "CXPlayer.h"

#define ROTATION_YV	CVector(0.0f, 2.0f, 0.0f) //��]���x
#define VELOCITY CVector(0.0f, 0.0f, 0.1f) //�ړ����x
#define ROTATION_XV	CVector(1.0f, 0.0f, 0.0f) //��]���x

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//��
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
	//��
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