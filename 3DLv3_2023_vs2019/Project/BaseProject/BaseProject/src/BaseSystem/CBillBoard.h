#ifndef CBILLBOARD_H
#define CBILLBOARD_H
#include "CObjectBase.h"
#include "CTriangle.h"
#include "CMaterial.h"
#include "CColor.h"

/*
ビルボードクラス
常にカメラの方を向く四角形
*/
class CBillBoard : public CObjectBase {
public:
	//コンストラクタ
	CBillBoard(ETag tag = ETag::eNone,
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);

	void SetSize(const CVector2& size);
	void SetColor(const CColor& color);

	//更新
	void Update() override;
	//描画
	void Render() override;
	void Render(CMaterial* mpMaterial);

protected:
	//三角形2つ
	CTriangle mT[2];
	//マテリアル
	CMaterial mMaterial;
};
#endif
