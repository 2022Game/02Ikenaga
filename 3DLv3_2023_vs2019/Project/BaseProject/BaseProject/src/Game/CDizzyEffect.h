#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CDizzy;

// めまいエフェクト
class CDizzyEffect : public CTask
{
public:
	// コンストラクタ
	CDizzyEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero);
	// デストラクタ
	~CDizzyEffect();

	// 開始
	void Start();
	// 終了
	void Stop();

	// めまいをしているかどうか
	bool IsDizzy() const;

	// オフセット位置を設定
	void SetDizzyOffsetPos(const CVector& pos);

	// 更新
	void Update() override;

private:
	// めまいを作成
	void CreateDizzy();

	// 位置を取得
	CVector GetPos() const;

	// エフェクトのリスト
	std::list<CDizzy*> mDizzy;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mOffsetPos;	        // オフセット位置
	bool mIsDizzy;			    // めまいをしているか
	int mStep;
};