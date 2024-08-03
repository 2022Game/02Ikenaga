#include "CEnemy.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CModel.h"

class CMagicCircle;
class CDrainEffect;
class CShieldRotate;
class CHealCircle;
class CIceBreath;
class CHit;

/*
 リッチクラス
 プレイヤーの推定レベル(81～90)
 エネミークラスを継承
*/
class CLich : public CEnemy
{
public:
	//インスタンスのポインタの取得
	static CLich* Instance();

	// コンストラクタ
	CLich();
	// デストラクタ
	~CLich();

	// HP吸収または回復
	virtual void DrainHp();

	// 更新処理
	void Update();

	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	//ダメージを与えたオブジェクト
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	// 召喚した敵の死亡処理
	void DeathSummonEnemy(CEnemy* enemy) override;

	// 死亡処理
	void Death() override;

	/// <summary>
	/// 防御力の強化割合を取得
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	float GetDefBuff(const CVector& attackDir) const override;

	// 1レベルアップ
	void LevelUp();
	// レベルの変更
	void ChangeLevel(int level);

	// ランダムに位置を取得
	CVector GetRandomSpawnPos()override;

	// 描画
	void Render();

private:
	// 待機状態
	void UpdateIdle();
	// 待機状態2
	void UpdateIdle2();

	// 攻撃
	void UpdateAttack();
	// 攻撃2
	void UpdateAttack2();
	// 攻撃終了待ち
	void UpdateAttackWait();

	// ヒット
	void UpdateHit();
	// 死ぬ
	void UpdateDie();
	// 召喚
	void UpdateSummon();
	// 移動
	void UpdateRun();

	// ランダム召喚する敵のインデックス値を取得
	int GetRandomSummonIndex() const;
	// ランダム召喚
	void RandomSummon();

	// 敵の召喚データ
	struct SpawnData
	{
		EEnemyType type;     // 敵の種類
		float dist;          // 生成距離
		CVector offsetPos;   // 生成位置のオフセット位置
		CColor circleColor;  // 魔法陣の色
		float circleScale;   // 魔法陣のスケール値
		float monsterScale;  // 生成する敵のスケール値
		float spawnRatio;    // 召喚するタイミング
	};
	// 敵の召喚データのテーブル
	static const SpawnData SPAWN_DATA[];

	CEnemy* SpawnEnemy(EEnemyType type) const;

	float mAttackTime;   // 次の攻撃時間
	float mElapsedTime;  // 経過時間

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,	   // Tポーズ
		eIdle,	   // 待機
		eAttack,   // 攻撃
		eAttack2,  // 攻撃2
		eHit,      // ヒット
		eDie,      // 死ぬ
		eSummon,   // 召喚
		eRun,	   // 移動

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// リッチのインスタンス
	static CLich* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
		float animSpeed;    // アニメーションの再生速度
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// リッチの状態
	enum class EState
	{
		eIdle,		  // 待機
		eIdle2,		  // 待機2
		eAttack,	  // 攻撃
		eAttack2,	  // 攻撃2
		eAttackWait,  // 攻撃終了待ち
		eHit,         // ヒット
		eDie,         // 死ぬ
		eSummon,      // 召喚
		eRun,         // 移動
	};
	EState mState;	  // エネミーの状態
	int mStateStep;   // State内のステップ処理

	// 状態を切り替え
	void ChangeState(EState state);

	CVector mMoveSpeed;	 // 移動速度
	bool mIsGrounded;    // 接地しているかどうか

	// 線分コライダー
	CColliderLine* mpColLineSide;    // 線分コライダー(横)
	CColliderLine* mpColLineHeight;  // 線分コライダー(縦)

	// キャラクターの押し戻しコライダー
	CColliderCapsule* mpColCapsuleBody;  // 体
	CColliderSphere* mpColSphereArmL;    // 左腕
	CColliderSphere* mpColSphereArmR;    // 右腕

	// ダメージを受けるコライダー
	CColliderCapsule* mpDamageColBody;  // 体
	CColliderSphere* mpDamageColArmL;   // 左腕
	CColliderSphere* mpDamageColArmR;   // 右腕
	CTransform* mpRideObject;

	// 回転するシールド
	CShieldRotate* mpShieldRotate;
	CShieldRotate* mpShieldRotate2;
	CShieldRotate* mpShieldRotate3;
	CShieldRotate* mpShieldRotate4;

	CMagicCircle* mpMagicCircle;  // 魔法陣
	CEnemy* mpSpawnEnemy;         // 自身が召喚した敵のポインタ
	CVector mMCStartPos;          // 魔法陣のアニメーション開始位置
	CQuaternion mMCStartRot;      // 魔法陣のアニメーション開始時の回転値
	int mRandomSummonIndex;       // 敵を召喚するランダムインデックス値

	CDrainEffect* mpDrain;      // ドレイン
	CHealCircle* mpHealCircle;  // 回復サークル
	CIceBreath* mpIceBreath;    // アイスブレス
	CHit* mpHitEffect;          // ヒットエフェクト
};
