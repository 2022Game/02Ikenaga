#include "CShieldRotate.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define MOVESPEED 0.5f

// コンストラク
CShieldRotate::CShieldRotate(const CVector& pos,const CColor& col)
	: mStateStep(0)
	, mDefenseUp(false)
	, mElapsedDefenseUpTime(0.0f)
{
	mpShieldRotate = CResourceManager::Get<CModel>("ShieldRotate");
	Position(pos);
	mpShieldRotate->SetColor(col);
}

// デストラクタ
CShieldRotate::~CShieldRotate()
{
}

// 更新
void CShieldRotate::Update()
{
	CPlayer* player = CPlayer::Instance();
	mDefenseUp = player->mDefenseUp;
	mElapsedDefenseUpTime = player->mElapsedDefenseUpTime;

	if (mDefenseUp == true)
	{
		switch (mStateStep)
		{
		case 0:
			if (mElapsedDefenseUpTime <= 15.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 1:
			if (mElapsedDefenseUpTime >= 15.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 2:
			if (mElapsedDefenseUpTime >= 16.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 3:
			if (mElapsedDefenseUpTime >= 16.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 4:
			if (mElapsedDefenseUpTime >= 17.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 5:
			if (mElapsedDefenseUpTime >= 17.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 6:
			if (mElapsedDefenseUpTime >= 18.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 7:
			if (mElapsedDefenseUpTime >= 18.2f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 8:
			if (mElapsedDefenseUpTime >= 18.4f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 9:
			if (mElapsedDefenseUpTime >= 18.6f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 10:
			if (mElapsedDefenseUpTime >= 18.8f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 11:
			if (mElapsedDefenseUpTime >= 19.0f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 12:
			if (mElapsedDefenseUpTime >= 19.2f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 13:
			if (mElapsedDefenseUpTime >= 19.4f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 14:
			if (mElapsedDefenseUpTime >= 19.6f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 15:
			if (mElapsedDefenseUpTime >= 19.8f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 16:
			if (mElapsedDefenseUpTime >= 19.9f)
			{
				CTask::SetShow(true);
				mStateStep = 0;
			}
			break;
		}
	}
	else
	{
		CTask::SetShow(false);
	}
	//Position(Position() + mMoveSpeed);
}

// 描画
void CShieldRotate::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpShieldRotate->Render(Matrix());
		mpShieldRotate->SetColor(mColor);
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpShieldRotate->Render(sm * (*mpAttachMtx));
	}
}