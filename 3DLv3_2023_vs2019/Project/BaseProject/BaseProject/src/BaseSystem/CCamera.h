#pragma once
#include "CObjectBase.h"

// カメラ
class CCamera : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="eye">カメラの視点</param>
	/// <param name="center">カメラの注視点</param>
	/// <param name="isMainCamera">メインカメラかどうか</param>
	CCamera(const CVector& eye, const CVector& center, bool isMainCamera = true);
	// デストラクタ
	~CCamera();

	// メインカメラを取得
	static CCamera* MainCamera();

	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="eye">視点</param>
	/// <param name="center">注視点</param>
	void Set(const CVector& eye, const CVector& center);

	/// <summary>
	/// 追従するターゲットを設定
	/// </summary>
	/// <param name="target">追従するターゲット</param>
	void SetFollowTargetTf(CTransform* target);

	/// <summary>
	/// 注視する位置を設定
	/// </summary>
	/// <param name="pos"></param>
	void LookAt(const CVector& pos);

	// 更新
	void Update();

	// カメラ反映
	void Apply();

	// 2D用のカメラ行列を反映
	static void Start2DCamera();
	// 3D用のカメラ行列へ戻す
	static void End2DCamera();

private:
	static CCamera* spMainCamera;	// メインカメラ
	bool mIsMainCamera;				// メインカメラかどうか
	CTransform* mFollowTargetTf;	// 追従するターゲットのCTransfrom
	CVector mFollowOffsetPos;		// 追従ターゲットの位置から視点までのオフセット値
};