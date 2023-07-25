#include "CTask.h"

//コンストラクタ
CTask::CTask(EScene scene)
	: mpNext(nullptr)
	, mpPrev(nullptr)
	, mPriority((int)TaskPriority::eDefault)
	, mEnabled(true)
	, mIsHidden(false)
	, mPauseType(TaskPauseType::eDefault)
	, mSceneType(scene)
{
}

//デストラクタ
CTask::~CTask()
{
}

//更新
void CTask::Update()
{
}

//描画
void CTask::Render()
{
}

//ポーズの種類を設定
void CTask::SetPauseType(TaskPauseType type)
{
	mPauseType = type;
}

//ポーズの種類を取得
TaskPauseType CTask::GetPauseType() const
{
	return mPauseType;
}

void CTask::SetHidden(bool isHidden)
{
	mIsHidden = isHidden;
}

bool CTask::IsHidden()const
{
	return mIsHidden;
}

//所属するシーンを設定
void CTask::SetSceneType(EScene scene)
{
	mSceneType = scene;
}

//所属するシーンを取得
EScene CTask::GetSceneType() const
{
	return mSceneType;
}

void CTask::Kill()
{
	mEnabled = false;
}

bool CTask::IsKill()
{
	return !mEnabled;
}
