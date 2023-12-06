#include "CBillBoardImage.h"

// コンストラクタ
CBillBoardImage::CBillBoardImage(std::string path, ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CBillBoard(tag, prio, sortOrder, pause)
	, mpAnimData(nullptr)
	, mAnimNo(0)
	, mElapsedTime(0.0f)
{
	// テクスチャ読み込み
	mMaterial.LoadTexture("", path, false);
	CTexture* tex = mMaterial.Texture();

	CVector2 size;
	size.X(tex->Header().width / 256.0f);
	size.Y(tex->Header().height / 256.0f);
	SetSize(size);

	SetColor(CColor::white);

	// UV設定
	SetUV(CRect(0.0f, 0.0f, 1.0f, 1.0f));
}

// デストラクタ
CBillBoardImage::~CBillBoardImage()
{
}

// UV設定
void CBillBoardImage::SetUV(const CRect& uv)
{
	float l = uv.X();
	float t = uv.Y();
	float r = l + uv.W();
	float b = t + uv.H();

	mT[0].UV
	(
		CVector(r, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, b, 0.0f)
	);
	mT[1].UV
	(
		CVector(l, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, t, 0.0f)
	);
}

// アニメーションを再生できるかどうか
bool CBillBoardImage::IsEnableAnim() const
{
	if (mpAnimData == nullptr) return false;
	if (mpAnimData->frames.size() == 0) return false;
	return true;
}

// アニメーションデータを設定
void CBillBoardImage::SetAnimData(TexAnimData* animData)
{
	mpAnimData = animData;
	mAnimNo = 0;
	mElapsedTime = 0.0f;

	// アニメーションが有効であれば、
	// 一番最初の画像のUVを設定
	if (IsEnableAnim())
	{
		CRect uv = mMaterial.Texture()->CalcUV
		(
			mpAnimData->row,
			mpAnimData->col,
			0
		);
		SetUV(uv);
	}
}

// 更新
void CBillBoardImage::Update()
{
	// アニメーションが有効であれば
	if (IsEnableAnim())
	{
		TexAnimFrameData data = mpAnimData->frames[mAnimNo];
		// 現在の画像の表示時間を超えたら
		if (mElapsedTime >= data.time)
		{
			// 次の画像へ切り替え
			mElapsedTime -= data.time;
			mAnimNo = (mAnimNo + 1) % mpAnimData->frames.size();
			CRect uv = mMaterial.Texture()->CalcUV
			(
				mpAnimData->row,
				mpAnimData->col,
				mAnimNo
			);
			SetUV(uv);
		}
		// 経過時間を計測
		mElapsedTime += Time::DeltaTime();
	}

	CBillBoard::Update();
}

// 描画
void CBillBoardImage::Render()
{
	CBillBoard::Render(&mMaterial);
}
