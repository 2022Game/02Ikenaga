#include "CBlock21.h"
#include "CApplication.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//�e�N�X�`�����W
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
	const int PITCH = 32;//�摜��؂�ւ���Ԋu
	int mTime = CGame::mTime;
	if ((int)X() % PITCH < PITCH / 2)
	{
		if (mTime > 7900) //���ֈړ�
		{
			Texture(Texture(), TEXCOORD2);
			if (mTime > 7910) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7920) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7930) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7940) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7950) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7960) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7970) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7980) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7985) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 7990) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 7995) //���ֈړ�
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 8000) //���ֈړ�
			{
				mEnabled = false;
			}
		}
	}
}