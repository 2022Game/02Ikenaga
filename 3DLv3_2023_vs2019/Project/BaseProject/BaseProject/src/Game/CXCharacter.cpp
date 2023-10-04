#include "CXCharacter.h"

//コンストラクタ
CXCharacter::CXCharacter(ETag tag, ETaskPriority prio)
	: CCharaBase(tag, prio)
	, mpModel(nullptr)
	, mpCombinedMatrix(nullptr)
	, mAnimationLoopFlg(false)
	, mAnimationIndex(0)
	, mAnimationFrame(0.0f)
	, mAnimationFrameSize(0.0f)
{
}

//デストラクタ
CXCharacter::~CXCharacter()
{
	SAFE_DELETE_ARRAY(mpCombinedMatrix)
}

/*
Init
初期化処理
*/
void CXCharacter::Init(CModelX* model)
{
	mpModel = model;
	//合成行列退避エリアの確保
	mpCombinedMatrix = new CMatrix[model->Frames().size()];
	//最初のアニメーションにする
	mAnimationIndex = 0;
	//繰り返し再生する
	mAnimationLoopFlg = true;
	//1アニメーション目の最大フレーム数
	mAnimationFrameSize = model->AnimationSet()[0]->MaxTime();
	//アニメーションのフレームを最大にする
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//アニメーションの重みを1.0(100%)にする
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}


/*
ChangeAnimation
アニメーションを切り替える
index:アニメーションの番号
loop:true:繰り返す
framesize:最後まで再生するのに使用されるフレーム数
*/
void CXCharacter::ChangeAnimation(int index, bool loop, float framesize)
{
	//同じ場合は切り替えない
	if (mAnimationIndex == index) return;
	//今のアニメーションの重みを0.0(0%)にする
	mpModel->AnimationSet()[mAnimationIndex]->Weight(0.0f);
	//番号、繰り返し、フレーム数を指定
	mAnimationIndex = index % mpModel->AnimationSet().size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//アニメーションのフレームを最初にする
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//アニメーションの重みを1.0(100%)にする
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}

/*
Update
更新する
matrix:移動、回転、拡大縮小の行列
*/
void CXCharacter::Update(const CMatrix& matrix)
{
	if (mpModel == nullptr) return;
	for (size_t i = 0; i < mpModel->AnimationSet().size(); i++)
	{
		mpModel->AnimationSet()[i]->mWeight = 0.0f;
		mpModel->AnimationSet()[mAnimationIndex]->mWeight = 1.0f;
	}
	//最後まで再生する
	if (mAnimationFrame <= mAnimationFrameSize)
	{
		//アニメーションの時間を計算
		mpModel->AnimationSet()[mAnimationIndex]->Time(
			mpModel->AnimationSet()[mAnimationIndex]->MaxTime() *
			mAnimationFrame / mAnimationFrameSize);
		//フレームを進める
		mAnimationFrame++;
	}
	else
	{
		//繰り返しの場合は、フレームを0に戻す
		if (mAnimationLoopFlg)
		{
			//アニメーションのフレームを最初にする
			mAnimationFrame = 0.0f;
			mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
		}
		else
		{
			mpModel->AnimationSet()[mAnimationIndex]->
				Time(mpModel->AnimationSet()[mAnimationIndex]
					->MaxTime());
		}
	}
	//フレームの変換行列をアニメーションで更新する
	mpModel->AnimateFrame();
	//フレームの合成行列を計算する
	mpModel->Frames()[0]->AnimateCombined(&matrix);
	//合成行列の退避
	for (size_t i = 0; i < mpModel->Frames().size(); i++)
	{
		mpCombinedMatrix[i] =
			mpModel->Frames()[i]->CombinedMatrix();
	}
}

//更新処理
void CXCharacter::Update()
{
	//アニメーションを更新する
	Update(Matrix());
}

/*
Render
描画する
*/
void CXCharacter::Render()
{
	mpModel->RenderShader(mpCombinedMatrix);
}

bool CXCharacter::IsAnimationFinished()
{
	return mAnimationFrame >= mAnimationFrameSize;
}

int CXCharacter::AnimationIndex()
{
	return mAnimationIndex;
}