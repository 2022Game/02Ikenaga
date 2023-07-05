#pragma once
#include <vector>
#include "CCharacter3.h"

class CStageBase
{
public:
	//コンストラクタ
	CStageBase();
	//デストラクタ
	virtual ~CStageBase();
protected:
	//キャラクターをリストに追加
	void AddChara(CCharacter3* chara);
private:
	//生成したキャラクターのリスト
	std::vector<CCharacter3*> mCharaList;
};
