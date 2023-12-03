#include "CExpManager.h"
#include "Maths.h"

CExpManeger* CExpManeger::spInstance = nullptr;

// コンストラクタ
CExpManeger::CExpManeger()
{
	spInstance = this;
}


//デストラクタ
CExpManeger::~CExpManeger()
{
	spInstance = nullptr;
}

// 経験値を作成
void CExpManeger::SpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 開始角度はランダムに決める
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	for (int i = 0; i < exp; i++)
	{
		// 等間隔に360度経験値を出現させる
		float per = (float)i / exp;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;
		CExp* e = new CExp();
		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}

}