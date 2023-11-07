#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMaterial.h"
#include "CCharaBase.h"

class CXCharacter : public CCharaBase
{
public:
	//コンストラクタ
	CXCharacter(ETag tag, ETaskPriority prio);
	//デストラクタ
	virtual ~CXCharacter();

	//初期化処理
	void Init(CModelX* model);
	//アニメーションの変更
	void ChangeAnimation(int index, bool loop, float framesize);
	//更新処理
	void Update(const CMatrix& m);
	void Update();
	//描画処理
	void Render();
	//アニメーションの再生終了判定
	//true:終了 false:再生中
	bool IsAnimationFinished();

	int AnimationIndex();	//アニメーションの番号の取得

	const CMatrix* GetFrameMtx(std::string name) const;
protected:
	CModelX* mpModel;			//モデルデータ
	CMatrix* mpCombinedMatrix;	//合成行列退避

	bool mAnimationLoopFlg;		//true:アニメーションを繰り返す

	int mAnimationIndex;		//アニメーション番号

	float mAnimationFrame;		//アニメーションの再生フレーム
	float mAnimationFrameSize;	//アニメーションの再生フレーム数
};
#endif