#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CExp.h"

// エネミーのインスタンス
CEnemy* CEnemy::spInstance = nullptr;

#define MODEL_SLIME "Character\\Slime\\Slime.x"

#define ENEMY_HEIGHT 1.0f
#define GRAVITY 0.0625f

// エネミーのアニメーションデータのテーブル
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	{ "Character\\Slime\\animation\\SlimeIdleNormal.x",	true,	70.0f	},  //アイドル通常 51.0f
	{ "Character\\Slime\\animation\\SlimeSenseSomethingStart.x",	true,	120.0f	},  //開始の見回す 63.0f
	{ "Character\\Slime\\animation\\SlimeSenseSomethingRoutine.x",	true,	140.0f	},  //見回す 71.0f
	{ "Character\\Slime\\animation\\SlimeIdleBattle.x",	true,	25.0f	},  //アイドルバトル 25.0f
	{ "Character\\Slime\\animation\\SlimeAttack.x",	true,	26.0f	},  //攻撃 26.0f
	{ "Character\\Slime\\animation\\SlimeAttack2.x",	true,	70.0f	},  //攻撃2 26.0f
	{ "Character\\Slime\\animation\\SlimeGetHit.x",	true,	65.0f	},  //ヒット 26.0f
	{ "Character\\Slime\\animation\\SlimeDie.x",	true,	81.0f	},  //死ぬ 41.0f
	{ "Character\\Slime\\animation\\SlimeDizzy.x",	true,	100.0f	},  //めまい 41.0f
	//{ "Character\\Slime\\animation\\SlimeRun.x",	true,	21.0f	},  //走る
	//{ "Character\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  //挑発
	//{ "Character\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  //勝利
	//{ "Character\\Slime\\animation\\SlimeWalk.x",	true,	31.0f	},  //歩く
	//{ "Character\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  //後ろに歩く
    //{ "Character\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  //左に移動
	//{ "Character\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  //右に移動
};

bool CEnemy::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

int CEnemy::mHp;

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
	,mAttackTime(0)
	,mDizzyTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_SLIME);

	//mpHpGauge = new CHpGauge();
	//mpHpGauge->SetPos(10.0f, 90.f);

	//最初に1レベルに設定
	ChangeLevel(1);

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター押し戻し処理
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::eEnemy,
		0.5f,false,5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->Position(0.0f, 0.3f, 0.0f);

	// ダメージを受けるコライダーを作成
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.5f,false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageCol->Position(0.0f, 0.3f,0.0f);

	// ダメージを与えるコライダー
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f,false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.0f, 0.3f, 0.0f);

}

CEnemy::~CEnemy()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	// ダメージを受けるコライダーを削除
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
	SAFE_DELETE(mpModel);
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CEnemy::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CEnemy::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 待機2状態
void CEnemy::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdleWait;
	}
}

// 攻撃した時の待機状態
void CEnemy::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// 待機2の終了待ち
void CEnemy::UpdateIdleWait()
{
	// 待機3アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle3);
	// 待機3アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 攻撃
void CEnemy::UpdateAttack()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack);
	// 攻撃終了待ち状態へ移行
	if (IsAnimationFinished())
	{
		mState = EState::eAttackWait;
		AttackStart();
	}
}

// 攻撃2
void CEnemy::UpdateAttack2()
{
	// 攻撃2アニメーションを開始
	ChangeAnimation(EAnimType::eAttack2);
	// 攻撃2終了待ち状態へ移行
	if (IsAnimationFinished())
	{
		mState = EState::eAttackWait;
		AttackStart();
	}
}

// 攻撃と攻撃2終了待ち
void CEnemy::UpdateAttackWait()
{
	// 攻撃と攻撃2アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);

		AttackEnd();
	}
}

// ヒット
void CEnemy::UpdateHIt()
{
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		bool stan = false;
		int w = Math::Rand(0, 10);
		if (w == 1)stan = true;

		if (stan)
		{
			mState = EState::eDizzy;
			mDizzyTime = 0;
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle4);
		}
	}
}

// 死ぬ時
void CEnemy::UpdateDie()
{
	// 死ぬ時のアニメーションを開始
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		CVector posY = Position();
		posY.Y(4.5f);
		CExp* exp = new CExp();
		exp->Position(posY);
		exp->Scale(4.0f, 4.0f, 4.0f);
	}
}

// めまい(混乱)
void CEnemy::UpdateDizzy()
{
	// めまい(混乱)アニメーションを開始
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		mDizzyTime = 0;
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);
	}
}

// 更新処理
void CEnemy::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	mHp = mCharaStatus.hp;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eIdle2:
		UpdateIdle2();
		break;
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// 待機終了待ち
	case EState::eIdleWait:
		UpdateIdleWait();
		break;
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃2
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// 攻撃終了待ち
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// ヒット
	case EState::eHit:
		UpdateHIt();
		break;
		// 死ぬ時
	case EState::eDie:
		UpdateDie();
		break;
		// めまい(混乱)
	case EState::eDizzy:
		UpdateDizzy();
		break;
	}
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	Position(Position() + mMoveSpeed);

	if (mCharaStatus.hp <= 0)
	{
		// 死亡処理
		mState = EState::eDie;
	}

	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		mAttackTime++;
		if (mAttackTime > 200)
		{
			mState = EState::eAttack;

		}
		else if (mCharaStatus.hp <= 0)
		{
			mAttackTime = 0;
		}
		if (mState == EState::eAttack || mState == EState::eDizzy)
		{
			mAttackTime = 0;
		}
	}

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;

	// エネミーのデバック表示
	static bool debug = false;
	if (CInput::PushKey('Q'))
	{
		debug = !debug;
	}
	if (debug)
	{
		CDebugPrint::Print(" HP %d/%d\n", mCharaStatus.hp, mCharaMaxStatus.hp);
		CDebugPrint::Print(" 攻撃時間計測 :%d", mAttackTime);
	}
	//mpHpGauge->SetValue(mCharaStatus2.hp);
}

// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol)
	{
		// キャラのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				int damage = 0;
				damage = mCharaStatus.power;
				// ダメージを与える
				chara->TakeDamage(damage,this);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
	// フィールドとの接地判定
	else if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CEnemy::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CEnemy::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 描画
void CEnemy::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CEnemy::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CEnemy::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;
}

// 被ダメージ処理
void CEnemy::TakeDamage(int damage, CObjectBase* causedObj)
{
	//死亡していたら、ダメージは受けない
	//if (mCharaStatus.hp <= 0)return;

	//HPからダメージを引く
	//mCharaStatus.hp = max(mCharaStatus.hp - damage, 0);
	//mCharaStatus.hp -= damage;
	if (mCharaStatus.hp -= damage)
	{
		mState = EState::eHit;
	}
	// HPが0以下になったら、
	if (mCharaStatus.hp <= 0)
	{
		// 死亡処理
		mState = EState::eDie;
	}

	if (causedObj != nullptr)
	{
		// ダメージを与えた相手の方向へ向く
		CVector dir = causedObj->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));

		// ノックバックでダメージを与えた相手の方向から
		// 後ろにズラす
		Position(Position() - dir * Scale().X() * 0.4f);
	}
}