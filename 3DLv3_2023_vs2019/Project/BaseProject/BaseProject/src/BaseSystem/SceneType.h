#pragma once

// シーンの種類
enum class EScene
{
	eNone,	// どのシーンにも所属していない

	eTitle,	    // タイトルシーン
	eTutorial,  // チュートリアルシーン
	eGame,	    // ゲームシーン
	eMethod,    // 操作方法
	eGameOver,  // ゲームオーバー

	eBootMenu,	// 起動時メニュー
	eColTest,	// 衝突判定テスト
};
