#include "CHpGauge2.h"
#include "CBillBoardImage.h"
#include "CSlime.h"
#include "Maths.h"

// ゲージのフレーム画像のファイルパス
#define FRAME_IMAGE "Character\\Player\\HP\\Frame.png"  //HPフレーム画像
// ゲージのバー画像のファイルパス
#define BAR_IMAGE "UI\\white.png"

// フレームの横のサイズ
#define FRAME_SIZE_X (15.0f)
// フレームの縦のサイズ
#define FRAME_SIZE_Y (5.0f)
// 緑の幅
#define FRAME_BORDER (2.0f)
// バーの横のサイズ
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// バーの縦のサイズ
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


// コンストラクタ
CHpGauge2::CHpGauge2()	
	:mMaxValue(100)
	, mValue(100)
{
	mpBarImage = new CBillBoardImage(BAR_IMAGE);
	//mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	//mpFrameImage = new CBillBoardImage(FRAME_IMAGE);
	//mpFrameImage->SetSize(size);
}

// デストラクタ
CHpGauge2::~CHpGauge2()
{

}

// 初期設定
void CHpGauge2::Setup(const CVector& pos, const CVector& dir)
{
	//　移動方向ベクトルを横軸回転して上方向へ向ける
	CVector side = CVector::Cross(CVector::up, dir);
	CQuaternion q;
	q.RotationAxis(side, 75.0f);
	mMoveSpeed = q * dir * 75.0f;
}

// 最大値を設定
void CHpGauge2::SetMaxValue(int value)
{
	mMaxValue = value;
}

// 現在値
void CHpGauge2::SetValue(int value)
{
	mValue = value;
}

// 更新
void CHpGauge2::Update()
{
	// バーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X, BAR_SIZE_Y);
	size.X(BAR_SIZE_X * percent);
	mpBarImage->SetSize(size);

	// HPの割合でバーの色を変更
	CColor color;
	// 10%以下は赤色
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	// 30%以下は黄色
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	// それ以外は緑色
	else color = CColor(0.0f, 1.0f, 0.0f);
	// バーに色を設定
	mpBarImage->SetColor(color);

	// スライムの情報を取得
	CSlime* enemy = CSlime::Instance();
	// スライムの座標
	CVector enemyPos = CVector::zero;
	// スライムまでのベクトル
	CVector enemyVec = CVector::zero;
	if (enemy != nullptr)
	{
		enemyPos = enemy->Position()+CVector(0.0f,0.0f,0.0f);
		enemyVec = enemyPos + mPosition;
	}
	mpBarImage->Position(enemyVec);
}