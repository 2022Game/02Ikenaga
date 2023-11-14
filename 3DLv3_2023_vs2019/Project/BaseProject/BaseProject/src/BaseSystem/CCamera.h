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
	virtual ~CCamera();

	// メインカメラを取得
	static CCamera* MainCamera();
	// 現在のカメラを取得
	static CCamera* CurrentCamera();

	/// <summary>
	/// カメラの状態をコピー
	/// </summary>
	/// <param name="copy">コピー元のカメラ</param>
	void CopyCamera(CCamera* copy);

	/// <summary>
	/// 追従するターゲットを設定
	/// </summary>
	/// <param name="target">追従するターゲット</param>
	void SetFollowTargetTf(CTransform* target);

	/// <summary>
	/// 追従時のオフセット座標を設定
	/// </summary>
	/// <param name="offset"></param>
	void SetFollowTargetOffset(const CVector& offset);

	/// <summary>
	/// 注視する位置を設定（注視点のみ）
	/// </summary>
	/// <param name="pos">注視する位置</param>
	void LookAt(const CVector& pos);
	/// <summary>
	/// 注視する位置を設定（注視点 + 上ベクトル）
	/// </summary>
	/// <param name="pos">注視する位置</param>
	/// <param name="up">上ベクトル</param>
	void LookAt(const CVector& pos, const CVector& up);
	/// <summary>
	/// 注視する位置を設定（視点 + 注視点 + 上ベクトル）
	/// </summary>
	/// <param name="eye">カメラの位置</param>
	/// <param name="at">注視する位置</param>
	/// <param name="up">上ベクトル</param>
	void LookAt(const CVector& eye, const CVector& at, const CVector& up);

	/// <summary>
	/// 画面サイズ変更時のコールバック関数
	/// </summary>
	/// <param name="width">画面の幅</param>
	/// <param name="height">画面の高さ</param>
	void Reshape(int width, int height);
	/// <summary>
	/// 画面の描画エリアを指定
	/// </summary>
	/// <param name="x">横の描画開始位置</param>
	/// <param name="y">縦の描画開始位置</param>
	/// <param name="w">横幅</param>
	/// <param name="h">縦幅</param>
	void Viewport(float x, float y, float w, float h);
	/// <summary>
	/// 透視投影行列を設定
	/// </summary>
	/// <param name="fovy">画角</param>
	/// <param name="aspect">アスペクト比</param>
	/// <param name="zNear">描画最短距離</param>
	/// <param name="zFar">描画最長距離</param>
	void Perspective(float fovy, float aspect, float zNear, float zFar);

	/// <summary>
	/// スクリーン座標からワールド座標へ変換
	/// </summary>
	/// <param name="screenPos">スクリーン座標</param>
	/// <returns>ワールド座標</returns>
	CVector ScreenToWorldPos(const CVector& screenPos);
	/// <summary>
	/// ワールド座標からスクリーン座標へ変換
	/// </summary>
	/// <param name="worldPos">ワールド座標</param>
	/// <returns>スクリーン座標</returns>
	CVector WorldToScreenPos(const CVector& worldPos);

	// 更新
	void Update() override;

	// カメラ反映
	void Apply();

	// 2D用のカメラ行列を反映
	static void Start2DCamera();
	// 3D用のカメラ行列へ戻す
	static void End2DCamera();

protected:
	CVector mEye;		// 視点
	CVector mAt;		// 注視点
	CVector mUp;		// 上ベクトル

private:
	static CCamera* spMainCamera;	// メインカメラ
	bool mIsMainCamera;				// メインカメラかどうか
	CTransform* mFollowTargetTf;	// 追従するターゲットのCTransfrom
	CVector mFollowOffsetPos;		// 追従ターゲットの位置から視点までのオフセット値

	CMatrix mViewMatrix;			// ビュー行列
	CMatrix mProjectionMatrix;		// プロジェクション行列
	CMatrix mViewportMatrix;		// ビューポート行列
	float mFovy;		// カメラの画角
	float mZNear;		// 描画最短距離
	float mZFar;		// 描画最長距離
};