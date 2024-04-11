#include "CBee3.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CNeedle.h"

// 蜂3のインスタンス
CBee3* CBee3::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.12f         // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 18.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// 蜂3のアニメーションデータのテーブル
const CBee3::AnimData CBee3::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Bee\\animation\\BeeIdle.x",	true,	40.0f	},	    // 待機 20.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeAttack.x",	true,	40.0f	},	    // 攻撃 17.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeGetHit.x",	true,	50.0f	},	    // ヒット 13.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeDie.x",	true,	90.0f	},	        // 死ぬ 20.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveBWD.x",	true,	42.0f	},	    // 移動 21.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeMoveFWD.x",	true,	42.0f	},	    // 移動2 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveLFT.x",	true,	42.0f	},	    // 左移動 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveRGT.x",	true,	42.0f	},	    // 右移動 21.0f
};

// コンストラクタ
CBee3::CBee3()
	: mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mIsSpawnedNeedleEffect(false)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Bee");

	//最初に1レベルに設定
	ChangeLevel(5);

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
	mpColliderLine->Position(0.0f, 5.0f, 0.0f);

	// キャラクター押し戻し処理(頭)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(-0.1f, 0.0f, 0.03f);

	// キャラクター押し戻し処理(口ばし)
	mpColliderSphereBeak = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.04f, false, 2.0f
	);
	mpColliderSphereBeak->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBeak->Position(-0.23f, 0.01f, 0.1f);

	// キャラクター押し戻し処理(口ばし2)
	mpColliderSphereBeak2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.02f, false, 2.0f
	);
	mpColliderSphereBeak2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBeak2->Position(-0.35f, 0.01f, 0.2f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.25f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.0f, 0.18f, 0.0f);

	// キャラクター押し戻し処理(尻尾)
	mpColliderSphereTail = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 2.0f
	);
	mpColliderSphereTail->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(尻尾2)
	mpColliderSphereTail2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.1f, false, 2.0f
	);
	mpColliderSphereTail2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail2->Position(0.02f, 0.14f, 0.0f);

	// キャラクター押し戻し処理(尻尾3)
	mpColliderSphereTail3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.05f, false, 2.0f
	);
	mpColliderSphereTail3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail3->Position(-0.05f, 0.0f, 0.04f);

	// キャラクター押し戻し処理(尻尾4)
	mpColliderSphereTail4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.03f, false, 2.0f
	);
	mpColliderSphereTail4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail4->Position(-0.15f, 0.0f, 0.1f);

	// キャラクター押し戻し処理(尻尾5)
	mpColliderSphereTail5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.02f, false, 2.0f
	);
	mpColliderSphereTail5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail5->Position(-0.25f, 0.0f, 0.105f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHead->Position(-0.1f, 0.0f, 0.03f);

	// ダメージを受けるコライダーを作成(口ばし)
	mpDamageColBeak = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.04f, false
	);
	mpDamageColBeak->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBeak->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBeak->Position(-0.23f, 0.01f, 0.1f);

	// ダメージを受けるコライダーを作成(口ばし2)
	mpDamageColBeak2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.02f, false
	);
	mpDamageColBeak2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBeak2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBeak2->Position(-0.35f, 0.01f, 0.2f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.25f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, 0.18f, 0.0f);

	// ダメージを受けるコライダーを作成(尻尾)
	mpDamageColTail = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	mpDamageColTail->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(尻尾2)
	mpDamageColTail2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	mpDamageColTail2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail2->Position(0.02f, 0.14f, 0.0f);

	// ダメージを受けるコライダーを作成(尻尾3)
	mpDamageColTail3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.05f, false
	);
	mpDamageColTail3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail3->Position(-0.05f, 0.0f, 0.04f);

	// ダメージを受けるコライダーを作成(尻尾4)
	mpDamageColTail4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.03f, false
	);
	mpDamageColTail4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail4->Position(-0.15f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(尻尾5)
	mpDamageColTail5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.02f, false
	);
	mpDamageColTail5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail5->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail5->Position(-0.25f, 0.0f, 0.105f);

	// ダメージを与えるコライダー
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.01f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(-0.28f, 0.0f, 0.1f);

	// キャラクター押し戻しとダメージを受けるコライダーを頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpColliderSphereBeak->SetAttachMtx(headMty);
	mpColliderSphereBeak2->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpDamageColBeak->SetAttachMtx(headMty);
	mpDamageColBeak2->SetAttachMtx(headMty);

	// キャラクター押し戻しとダメージを受けるコライダーを体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// キャラクター押し戻しとダメージを受けるコライダーを尻尾の行列にアタッチ
	const CMatrix* tailMty = GetFrameMtx("Armature_Tail01");
	mpColliderSphereTail->SetAttachMtx(tailMty);
	mpColliderSphereTail2->SetAttachMtx(tailMty);
	mpDamageColTail->SetAttachMtx(tailMty);
	mpDamageColTail2->SetAttachMtx(tailMty);

	// キャラクター押し戻しとダメージを受けるコライダーと攻撃コライダーを蜂の針の行列にアタッチ
	const CMatrix* needleMty = GetFrameMtx("Armature_Tail02");
	mpColliderSphereTail3->SetAttachMtx(needleMty);
	mpColliderSphereTail4->SetAttachMtx(needleMty);
	mpColliderSphereTail5->SetAttachMtx(needleMty);
	mpDamageColTail3->SetAttachMtx(needleMty);
	mpDamageColTail4->SetAttachMtx(needleMty);
	mpDamageColTail5->SetAttachMtx(needleMty);
	mpAttackCol->SetAttachMtx(needleMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
}

CBee3::~CBee3()
{
	SAFE_DELETE(mpColliderLine);
	
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBeak);
	SAFE_DELETE(mpColliderSphereBeak2);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTail);
	SAFE_DELETE(mpColliderSphereTail2);
	SAFE_DELETE(mpColliderSphereTail3);
	SAFE_DELETE(mpColliderSphereTail4);
	SAFE_DELETE(mpColliderSphereTail5);

	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBeak);
	SAFE_DELETE(mpDamageColBeak2);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTail);
	SAFE_DELETE(mpDamageColTail2);
	SAFE_DELETE(mpDamageColTail3);
	SAFE_DELETE(mpDamageColTail4);
	SAFE_DELETE(mpDamageColTail5);
}

CBee3* CBee3::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBee3::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CBee3::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle;
		}
	}
}

// 攻撃
void CBee3::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	
	if (mAnimationFrame >= 5.0f)
	{
		AttackStart();
	}
	if (mAnimationFrame >= 15.0f)
	{
		AttackEnd();
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (mAnimationFrame >= 14.0f)
	{
		if (vectorp >= 30.0f)
		{
			// 針を生成済みフラグを初期化
			mIsSpawnedNeedleEffect = false;
			// 針を生成していない
			if (!mIsSpawnedNeedleEffect)
			{
				CNeedle* needle = new CNeedle
				(
					this,
					Position() + CVector(0.0f, 8.0f, 0.0f),
					VectorZ(),
					150.0f,
					100.0f
				);
				needle->SetColor(CColor(1.0f, 0.0f, 1.0f));
				needle->Scale(5.0f, 5.0f, 5.0f);
				needle->Rotate(-90.0f, 0.0f, 0.0f);
				needle->SetOwner(this);

				mIsSpawnedNeedleEffect = true;
			}
		}

		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃終了待ち
void CBee3::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle;
	}
}

// ヒット
void CBee3::UpdateHit()
{
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 死ぬ
void CBee3::UpdateDie()
{
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::BeeBigDeath();
	}
}

// 移動
void CBee3::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// 追跡をやめて止まる
	if (vectorp <= 20.0f && vectorp >= 23.0f)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

		// 回転する範囲であれば
		if (vectorp <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));

			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
		}
	}
	// 範囲内の時、移動し追跡する
	else if (vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	// 追跡が止まった時、待機モーションへ
	if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 更新処理
void CBee3::Update()
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
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃終了待ち
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// ヒット
	case EState::eHit:
		UpdateHit();
		break;
		// 死ぬ
	case EState::eDie:
		UpdateDie();
		break;
		// 移動
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HPゲージの座標を更新(敵の座標の少し上の座標)
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eAttack && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDie && mState != EState::eRun)
	{
		mState = EState::eIdle;
	}

	if (mState == EState::eRun || mState == EState::eIdle || mState == EState::eAttack ||
		mState == EState::eDie || mState == EState::eHit || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle && vectorp <= 40.0f || mState == EState::eHit || mState == EState::eRun
		|| mState == EState::eAttack || mState == EState::eAttackWait)
	{
		mAttackTime++;

		if (vectorp <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		if (mAttackTime > 200)
		{
			mState = EState::eAttack;
		}
		if (mState == EState::eAttack)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eIdle || mState == EState::eRun)
	{
		if (mFlyingTime <= 200 && Position().Y() <= 0.0f)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.02f);
		}

		if (mFlyingTime >= 200 && Position().Y() >= 0.1f)
		{
			Position(Position().X(), Position().Y() - 0.5f, Position().Z());
		}
	}

	if (Position().Y() >= 0.1f || vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mFlyingTime++;
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}
	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	// キャラクターの更新
	CXCharacter::Update();

	mpColliderSphereHead->Update();
	mpColliderSphereBeak->Update();
	mpColliderSphereBeak2->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereTail->Update();
	mpColliderSphereTail2->Update();
	mpColliderSphereTail3->Update();
	mpColliderSphereTail4->Update();
	mpColliderSphereTail5->Update();

	mpDamageColHead->Update();
	mpDamageColBeak->Update();
	mpDamageColBeak2->Update();
	mpDamageColBody->Update();
	mpDamageColTail->Update();
	mpDamageColTail2->Update();
	mpDamageColTail3->Update();
	mpDamageColTail4->Update();
	mpDamageColTail5->Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CBee3::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol && mState != EState::eIdle)
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

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphereHead || self == mpColliderSphereBeak || self == mpColliderSphereBeak2
		|| self == mpColliderSphereBody || self == mpColliderSphereTail || self == mpColliderSphereTail2
		|| self == mpColliderSphereTail3 || self == mpColliderSphereTail4 || self == mpColliderSphereTail5)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CBee3::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CBee3::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 描画
void CBee3::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBee3::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBee3::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY5_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CBee3::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		mState = EState::eHit;
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
		Position(Position() - dir * Scale().X() * 0.6f);
	}
}

// 死亡処理
void CBee3::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}