#include "CBlock22.h"
#include "CApplication.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//�e�N�X�`�����W
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
	const int PITCH = 32;//�摜��؂�ւ���Ԋu
	int mTime = CGame::mTime;
	if ((int)X() % PITCH < PITCH / 2)
	{
		if (mTime > 8900) //���ֈړ�
		{
			Texture(Texture(), TEXCOORD2);
			if (mTime > 8910) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8920) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8930) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8940) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8950) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8960) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8970) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8980) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8985) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8990) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 8995) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 9000) //���ֈړ�
			{
				mEnabled = false;
			}
			else
			{
				if (mTime > 8900) //���ֈړ�
				{
					Texture(Texture(), TEXCOORD2);
					if (mTime > 8910) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8920) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8930) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8940) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8950) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8960) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8970) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8980) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8985) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 8990) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD2);
					}
					if (mTime > 8995) //���ֈړ�
					{
						Texture(Texture(), TEXCOORD);
					}
					if (mTime > 9000) //���ֈړ�
					{
						mEnabled = false;
					}
				}
			}
		}
	}
}