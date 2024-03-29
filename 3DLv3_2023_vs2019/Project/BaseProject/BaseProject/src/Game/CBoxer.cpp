#include "CBoxer.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"

// ボクサーのインスタンス
CBoxer* CBoxer::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.05f         // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 24.5f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// ボクサーのアニメーションデータのテーブル
const CBoxer::AnimData CBoxer::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDance.x",	true,	170.0f	},	    // ダンス 85.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerIdle.x",	true,	42.0f	},	    // 待機 21.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerAttack.x",	true,	35.0f	},	    // 攻撃 19.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerAttack2.x",	true,	52.0f	},	    // 攻撃 26.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerGetHit.x",	true,	36.0f	},	    // ヒット 17.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDefense.x",	true,	42.0f	},	    // 防御 21.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDefenseHit.x",	true,	42.0f	},  // 防御中のヒット 21.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDie.x",	true,	90.0f	},	        // 死ぬ 24.0f
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerDieRecover.x",	true,	48.0f	},	// 起き上がる 24.0f
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerDizzy.x",	true,	66.0f	},	    // めまい 24.0f
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerJump.x",	true,	46.0f	},	    // ジャンプ 23.0f
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerJumpStart.x",	true,	36.0f	},	// ジャンプの開始 9.0f
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerJumpEnd.x",	true,	52.0f	},	// ジャンプの終了 26.0f
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerRun.x",	true,	42.0f	},	// 走る 21.0
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerSlide.x",	true,	72.0f	},	// 滑る 36.0
};

// コンストラクタ
CBoxer::CBoxer()
	: mpRideObject(nullptr)
	, mAttackTime(0)
	, mDefenseTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Boxer");

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
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター押し戻し処理(頭)
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,0.4f, false, 2.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.04f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphere2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.5f, false, 4.0f
	);
	mpColliderSphere2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere2->Position(0.07f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(右手)
	mpColliderSphere3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.27f, false, 3.0f
	);
	mpColliderSphere3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere3->Position(0.0f, -0.1f, 0.0f);

	// キャラクター押し戻し処理(左手)
	mpColliderSphere4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.27f, false, 3.0f
	);
	mpColliderSphere4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere4->Position(0.0f, -0.1f, 0.0f);

	// キャラクター押し戻し処理(右足)
	mpColliderSphere5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphere5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere5->Position(0.0f, 0.03f, 0.0f);

	// キャラクター押し戻し処理(左足)
	mpColliderSphere6 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphere6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere6->Position(0.0f, 0.03f, 0.0f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.4f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageCol->Position(0.04f, 0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.5f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol2->Position(0.07f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(右手)
	mpDamageCol3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.27f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol3->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol3->Position(0.0f, -0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(左手)
	mpDamageCol4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.27f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol4->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol4->Position(0.0f, -0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(右足)
	mpDamageCol5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol5->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol5->Position(0.0f, 0.25f, 0.0f);

	// ダメージを受けるコライダーを作成(左足)
	mpDamageCol6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol6->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol6->Position(0.0f, 0.25f, 0.0f);

	// ダメージを与えるコライダー(右手)
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.27f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.0f, -0.1f, 0.0f);

	// ダメージを与えるコライダー(右足)
	mpAttackCol2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.06f, false
	);
	mpAttackCol2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol2->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol2->Position(0.0f, 0.03f, 0.0f);

	// ダメージを与えるコライダー(左足)
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.06f, false
	);
	mpAttackCol3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol3->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol3->Position(0.0f, 0.03f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーをボクサーの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_neck_01");
	mpColliderSphere->SetAttachMtx(headMty);
	mpDamageCol->SetAttachMtx(headMty);

	// 押し戻しコライダーとダメージを受けるコライダーをボクサーの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_spine_02");
	mpColliderSphere2->SetAttachMtx(bodyMty);
	mpDamageCol2->SetAttachMtx(bodyMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをボクサーの右手の行列にアタッチ
	const CMatrix* rightHandMty = GetFrameMtx("Armature_drill_r");
	mpColliderSphere3->SetAttachMtx(rightHandMty);
	mpDamageCol3->SetAttachMtx(rightHandMty);
	mpAttackCol->SetAttachMtx(rightHandMty);

	// 押し戻しコライダーとダメージを受けるコライダーをボクサーの左手の行列にアタッチ
	const CMatrix* leftHandMty = GetFrameMtx("Armature_drill_l");
	mpColliderSphere4->SetAttachMtx(leftHandMty);
	mpDamageCol4->SetAttachMtx(leftHandMty);

	// ダメージを受けるコライダーをボクサーの右足の行列にアタッチ
	const CMatrix* rightFootMty = GetFrameMtx("Armature_thigh_r");
	mpDamageCol5->SetAttachMtx(rightFootMty);

	// ダメージを受けるコライダーをボクサーの左足の行列にアタッチ
	const CMatrix* leftFootMty = GetFrameMtx("Armature_thigh_l");
	mpDamageCol6->SetAttachMtx(leftFootMty);

	// 押し戻しコライダーと攻撃コライダーをボクサーの右足の行列にアタッチ
	const CMatrix* rightFootMty2 = GetFrameMtx("Armature_foot_r");
	mpColliderSphere5->SetAttachMtx(rightFootMty2);
	mpAttackCol2->SetAttachMtx(rightFootMty2);

	// 押し戻しコライダーと攻撃コライダーをボクサーの左足の行列にアタッチ
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_foot_l");
	mpColliderSphere6->SetAttachMtx(leftFootMty2);
	mpAttackCol3->SetAttachMtx(leftFootMty2);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
}

CBoxer::~CBoxer()
{
	//SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpColliderSphere2);
	SAFE_DELETE(mpColliderSphere3);
	SAFE_DELETE(mpColliderSphere4);
	SAFE_DELETE(mpColliderSphere5);
	SAFE_DELETE(mpColliderSphere6);

	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
	SAFE_DELETE(mpDamageCol3);
	SAFE_DELETE(mpDamageCol4);
	SAFE_DELETE(mpDamageCol5);
	SAFE_DELETE(mpDamageCol6);

	SAFE_DELETE(mpAttackCol);
	SAFE_DELETE(mpAttackCol2);
	SAFE_DELETE(mpAttackCol3);
}

CBoxer* CBoxer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBoxer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CBoxer::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 待機状態2
void CBoxer::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 攻撃
void CBoxer::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃2
void CBoxer::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CBoxer::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle2;
	}
}

// ヒット
void CBoxer::UpdateHit()
{
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// めまいをfalseにする
		bool stan = false;
		// 確率を最小に0最大40
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			//mState = EState::eDizzy;
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			mState = EState::eIdle2;
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// 防御
void CBoxer::UpdateDefense()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefense);

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= ROTATE_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		if (mDefenseTime >= 800)
		{
			mState = EState::eAttack;
		}
		else
		{
			mState = EState::eDefense;
		}
	}
}

// 防御中のヒット
void CBoxer::UpdateDefenseHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefenseHit);
	if (IsAnimationFinished())
	{
		mState = EState::eDefense;
	}
}

// 死ぬ
void CBoxer::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::Death();
	}
}

// めまい(混乱)
void CBoxer::UpdateDizzy()
{
	//ChangeAnimation(EAnimType::eDizzy);
}

// 更新処理
void CBoxer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// 待機状態2
	case EState::eIdle2:
		UpdateIdle2();
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
		UpdateHit();
		break;
		// 防御
	case EState::eDefense:
		UpdateDefense();
		break;
		// 防御中のヒット
	case EState::eDefenseHit:
		UpdateDefenseHit();
		break;
		// 死ぬ
	case EState::eDie:
		UpdateDie();
		break;
		// めまい(混乱)
	case EState::eDizzy:
		UpdateDizzy();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack
		&& mState != EState::eAttackWait && mState != EState::eHit && mState != EState::eDefense && mState != EState::eDefenseHit
		&& mState != EState::eDie)
	{
		mState = EState::eIdle2;
	}
	if (mState == EState::eIdle2)
	{
		mAttackTime++;

		if (mAttackTime > 200)
		{
			// 攻撃2
			bool Attack2 = false;
			// 防御
			bool Defense = false;
			// 確率を最小に2最大5
			int probability2 = Math::Rand(2, 5);
			// 確率を最小に6最大10
			int probability3 = Math::Rand(6, 10);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 6)Defense = true;
			if (Attack2)
			{
				mState = EState::eAttack2;
			}
			else if (Defense)
			{
				mState = EState::eDefense;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
	}

	// 防御時間計測
	if (mState == EState::eDefense || mState == EState::eDefenseHit)
	{
		mDefenseTime++;
	}
	else
	{
		mDefenseTime = 0;
	}

	CDebugPrint::Print(" 攻撃時間: %d\n", mAttackTime);
	CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// キャラクターの更新
	CXCharacter::Update();

	mpColliderSphere->Update();
	mpColliderSphere2->Update();
	mpColliderSphere3->Update();
	mpColliderSphere4->Update();
	mpColliderSphere5->Update();
	mpColliderSphere6->Update();

	mpDamageCol->Update();
	mpDamageCol2->Update();
	mpDamageCol3->Update();
	mpDamageCol4->Update();
	mpDamageCol5->Update();
	mpDamageCol6->Update();

	mpAttackCol->Update();
	mpAttackCol2->Update();
	mpAttackCol3->Update();

	mIsGrounded = false;
}

// 衝突処理
void CBoxer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol || self == mpAttackCol2 || self == mpAttackCol3 
		&& mState != EState::eIdle && mState != EState::eIdle2)
	{
		// キャラのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(this, chara);

				// ダメージを与える
				chara->TakeDamage(damage, this);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			//mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphere || self == mpColliderSphere2 || self == mpColliderSphere3
		|| self == mpColliderSphere4 || self == mpColliderSphere5 || self == mpColliderSphere6)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CBoxer::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
	mpAttackCol2->SetEnable(true);
	mpAttackCol3->SetEnable(true);
}

// 攻撃終了
void CBoxer::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
}

// 描画
void CBoxer::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBoxer::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBoxer::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY4_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CBoxer::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense)
		{
			mState = EState::eDefenseHit;
		}
		else
		{
			mState = EState::eHit;
		}
	}
	// HPが0以下になったら、
	if (mCharaStatus.hp <= 0)
	{
		// 死亡処理
		Death();
	}

	if (causedObj != nullptr)
	{
		// ダメージを与えた相手の方向へ向く
		CVector dir = causedObj->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));

		// ノックバックでダメージを与えた相手の方向から後ろにズラす
		Position(Position() - dir * Scale().X() * 0.2f);
	}
}

// 防御力の強化割合を取得
float CBoxer::GetDefBuff(const CVector& attackDir)const
{
	// 防御状態であれば、防御2倍
	if (mState == EState::eDefense) return 2.0f;

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}


// 死亡処理
void CBoxer::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}