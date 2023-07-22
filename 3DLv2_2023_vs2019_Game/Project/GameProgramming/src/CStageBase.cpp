#include "CStageBase.h"

//コンストラクタ
CStageBase::CStageBase()
{
}

//デストラク
CStageBase::~CStageBase()
{
	for (CCharacter3* chara : mCharaList)
	{
		chara->Kill();
		delete chara;
	}
	mCharaList.clear();
}

//キャラクターをリストに追加
void CStageBase::AddChara(CCharacter3* chara)
{
	mCharaList.push_back(chara);
}