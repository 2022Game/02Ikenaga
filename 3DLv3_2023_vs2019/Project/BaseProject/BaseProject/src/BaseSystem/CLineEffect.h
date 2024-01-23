#pragma once
#include "CEffect.h"
#include "CTexture.h"

class CLinePoint
{
public:
	CVector pos;
	float startWidth;
	float endWidth;
	float elapsed;
	float alpha;
	CLinePoint(const CVector& p, float sw, float ew)
		: pos(p), startWidth(sw), endWidth(ew), elapsed(0.0f), alpha(1.0f) {};
};

// 線分エフェクト
class CLineEffect : public CEffect
{
public:
	// コンストラクタ
	CLineEffect(ETag tag);
	// デストラクタ
	virtual ~CLineEffect();

	// テクスチャ設定
	void SetTexture(std::string texName);

	// 線のポイントを追加
	int AddPoint(const CVector& pos, float startWidth, float endWidth);
	// 線のポイントを削除
	void RemovePoint(int index);

	// 線のポイント数を取得
	int GetPointCount() const;

	// 線のポイントの座標を設定
	void SetPos(int index, const CVector& pos);
	// 線のポイントの座標を取得
	CVector GetPos(int index) const;

	//// 線のポイントの幅を設定
	//void SetWidth(int index, float width);
	//// 線のポイントの幅を取得
	//float GetWidth(int index) const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

protected:
	// 線のテクスチャ
	CTexture* mpTexture;
	// 線のポイントのリスト
	std::vector<CLinePoint*> mPoints;
};
