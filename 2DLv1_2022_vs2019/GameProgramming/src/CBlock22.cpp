#include "CBlock22.h"
#include "CApplication.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//テクスチャ座標
#define TEXCOORD2 10,10,10,10

CBlock22::CBlock22(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK2;
}

void CBlock22::Update()
{
	Texture(Texture(), TEXCOORD);
	const int PITCH = 32;//画像を切り替える間隔
	int mTime = CGame::mTime;
	if ((int)X() % PITCH < PITCH / 2)
	{
		if (mTime > 8900) //左へ移動
		{
			Texture(Texture(), TEXCOORD2);
			if (mTime > 8910) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8920) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8930) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8940) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8950) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8960) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8970) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8980) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8985) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8990) //左へ移動
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8995) //左へ移動
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 9000) //左へ移動
			{
				mEnabled = false;
			}
			else
			{
				if (mTime > 8900) //左へ移動
				{
					Texture(Texture(), TEXCOORD2);
					if (mTime > 8910) //左へ移動
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8920) //左へ移動
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8930) //左へ移動
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8940) //左へ移動
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8950) //左へ移動
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8960) //左へ移動
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8970) //左へ移動
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8980) //左へ移動
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8985) //左へ移動
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8990) //左へ移動
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8995) //左へ移動
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 9000) //左へ移動
					{
						mEnabled = false;
					}
				}
			}
		}
	}
}