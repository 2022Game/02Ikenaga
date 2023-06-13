#include "CXPlayer.h"

#define ROTATION_YV	CVector(0.0f, 2.0f, 0.0f) //‰ñ“]‘¬“x
#define VELOCITY CVector(0.0f, 0.0f, 0.1f) //ˆÚ“®‘¬“x
#define ROTATION_XV	CVector(1.0f, 0.0f, 0.0f) //‰ñ“]‘¬“x

void CXPlayer::Update()
{
	if (mInput.Key('A'))
	{
		mRotation = mRotation + ROTATION_YV;
	}
	if (mInput.Key('D'))
	{
		mRotation = mRotation - ROTATION_YV;
	}
	if (mInput.Key('W'))
	{
		mPosition = mPosition + VELOCITY * mMatrixRotate;
		CXCharacter::ChangeAnimation(1, true, 60);
	}
	else
	{
		CXCharacter::ChangeAnimation(0, true, 60);
	}
	CXCharacter::Update();
}
