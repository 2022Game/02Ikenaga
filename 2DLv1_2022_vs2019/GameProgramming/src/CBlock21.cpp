#include "CBlock21.h"
#include "CApplication.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//テクスチャ座標
#define TEXCOORD2 10,10,10,10

CBlock21::CBlock21(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK2;
}

void CBlock21::Update()
{
	Texture(Texture(), TEXCOORD);
	const int PITCH = 32;//画像を切り替える間隔
	int mTime = CGame::mTime;
	if ((int)X() % PITCH < PITCH / 2)
	{
		if (mTime > 7900) //左へ移動
		{
			Texture(Texture(), TEXCOORD2);
			if (mTime > 7910) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7920) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7930) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7940) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7950) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7960) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7970) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7980) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7985) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7990) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7995) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8000) //左へ移動
			{
				mEnabled = false;
			}
		}
	}
}