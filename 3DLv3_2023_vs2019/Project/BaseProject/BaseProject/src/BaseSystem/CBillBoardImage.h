#pragma once
#include "CBillBoard.h"
#include "CRect.h"
class CCharaBase;

// 常にカメラの方向を向くイメージ
class CBillBoardImage : public CBillBoard
{
public:
	// コンストラクタ
	CBillBoardImage(std::string path,
		ETag tag = ETag::eNone,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// デストラクタ
	~CBillBoardImage();

	/// <summary>
	/// UV設定
	/// </summary>
	/// <param name="uv"></param>
	void SetUV(const CRect& uv);

	/// <summary>
	/// アニメーションデータを設定
	/// </summary>
	/// <param name="animData"></param>
	void SetAnimData(TexAnimData* animData);

	// アニメーションが終わったかどうか
	bool IsEndAnim() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// エフェクトの発射キャラクターを設定
	void SetOwner(CCharaBase* owner);

	// エフェクトの発射キャラクターを取得
	CCharaBase* GetOwner() const;

private:
	// アニメーションを再生できるかどうか
	bool IsEnableAnim() const;

	// アニメーションデータのリスト
	TexAnimData* mpAnimData;
	// 現在のアニメーション番号
	int mAnimNo;
	// アニメーション用の経過時間計測変数
	float mElapsedTime;

protected:
	// 攻撃がヒットしたオブジェクトを追加
	void AddAttackHitObj(CObjectBase* obj);
	// 既に攻撃がヒットしているオブジェクトかどうか
	bool IsAttackHitObj(CObjectBase* obj) const;

	// 攻撃がヒット済みのオブジェクトリスト
	std::list<CObjectBase*> mAttackHitObjects;

	// エフェクトの所持キャラクター
	CCharaBase* mOwner;
};