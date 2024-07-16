#include "CMushroom.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "CGameEnemyUI.h"
#include "Maths.h"

// マッシュルームのインスタンス
CMushroom* CMushroom::spInstance = nullptr;

#define ENEMY_HEIGHT    0.9f  // 線分コライダー
#define WITHIN_RANGE   40.0f  // 範囲内
#define MOVE_SPEED      0.4f  // 移動速度
#define GRAVITY         0.3f  // 重力
#define WALK_RANGE    100.0f  // 追跡する範囲
#define STOP_RANGE     20.0f  // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f  // 回転する範囲

// マッシュルームのアニメーションデータのテーブル
const CMushroom::AnimData CMushroom::ANIM_DATA[] =
{
	{ "",										                            true,	0.0f,	0.0f},  // Tポーズ
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	        true,	21.0f,	0.3f},	// 植物
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlantToBattle.x",	false,	21.0f,	0.3f},  // 植物からきのこ
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	    true,	18.0f,	0.5f},	// 待機2
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	    true,	18.0f,	0.5f},	// 待機
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack.x",	        false,	26.0f,	0.4f},	// 攻撃
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	        false,	26.0f,	0.6f},	// 攻撃
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	        false,	26.0f,	0.5f},	// 攻撃
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	        false,	23.0f,	0.4f},	// ヒット
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	            false,	26.0f, 0.25f},	// 死ぬ
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	            false,	41.0f,  0.4f},	// めまい
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomRun.x",	            true,	17.0f, 	0.4f},	// 移動
};

int CMushroom::mHp;

// コンストラクタ
CMushroom::CMushroom()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mStateStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//インスタンスの設定
	spInstance = this;

	// 敵の種類
	mType = EEnemyType::eMushroom;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Mushroom");

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

	SetAnimationSpeed(0.3f);
	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	// キャラクターの線分コライダー
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 0.2f, 0.0f);

	// キャラクター押し戻し処理(頭)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.5f, false, 5.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.0f, -0.08f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.25f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.045f, 0.2f, 0.0f);

	// キャラクター押し戻し処理(根)
	mpColliderSphereRoot = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.25f, false, 5.0f
	);
	mpColliderSphereRoot->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereRoot->Position(0.065f, 0.18f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.55f, 0.0f),
		4.5f,false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -4.2f, 0.0f);

	// ダメージを受けるコライダーを作成(かさ)
	mpDamageColUmbrella = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	mpDamageColUmbrella->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColUmbrella->SetCollisionTags({ ETag::eWeapon });
	mpDamageColUmbrella->Position(0.0f, -0.1f, 0.0f);
	
	// ダメージを受けるコライダーを作成(根)
	mpDamageColRoot = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.25f, false
	);
	mpDamageColRoot->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColRoot->SetCollisionTags({ ETag::eWeapon });
	mpDamageColRoot->Position(0.05f, 0.23f, 0.0f);

	// ダメージを与えるコライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.55f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.0f, -0.1f, 0.0f);

	// ダメージを与えるコライダー(根)
	mpAttackColRoot = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.25f, false
	);
	mpAttackColRoot->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColRoot->SetCollisionTags({ ETag::ePlayer });
	mpAttackColRoot->Position(0.05f, 0.23f, 0.0f);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーと攻撃コライダーをマッシュルームの頭の行列にアタッチ
	const CMatrix* headMtx = GetFrameMtx("Armature_mushroom_spine03");
	mpColliderSphereHead->SetAttachMtx(headMtx);
	mpDamageColUmbrella->SetAttachMtx(headMtx);
	mpAttackColHead->SetAttachMtx(headMtx);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーをマッシュルームの体の行列にアタッチ
	const CMatrix* bodyMtx = GetFrameMtx("Armature_mushroom_spine02");
	mpColliderSphereBody->SetAttachMtx(bodyMtx);
	mpDamageColBody->SetAttachMtx(bodyMtx);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーと攻撃コライダーをマッシュルームの根の行列にアタッチ
	const CMatrix* rootMtx = GetFrameMtx("Armature_mushroom_root");
	mpColliderSphereRoot->SetAttachMtx(rootMtx);
	mpDamageColRoot->SetAttachMtx(rootMtx);
	mpAttackColRoot->SetAttachMtx(rootMtx);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);
	mpAttackColRoot->SetEnable(false);
}

// デストラクタ
CMushroom::~CMushroom()
{
	// キャラクターの線分コライダー
	SAFE_DELETE(mpColliderLine);

	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereRoot);

	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColUmbrella);
	SAFE_DELETE(mpDamageColRoot);

	// 攻撃コライダー
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColRoot);
}

// インスタンス
CMushroom* CMushroom::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CMushroom::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CMushroom::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// 戦う前の待機状態
void CMushroom::UpdateIdle()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eIdle);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
	else
	{
		ChangeState(EState::eIdle);
	}
}

// 戦う前の待機状態2
void CMushroom::UpdateIdle2()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// 待機状態3
void CMushroom::UpdateIdle3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle3);
	}
}

// 攻撃
void CMushroom::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 13.0f)
	{
		AttackStart();
		// 攻撃終了待ち状態へ移行
		ChangeState(EState::eAttackWait);
	}
}

// 攻撃2
void CMushroom::UpdateAttack2()
{
	SetAnimationSpeed(0.6f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃2アニメーション開始＋攻撃開始(一回目)
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 5.0f && mAnimationFrame <= 10.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ1 : 攻撃終了
	case 1:
		if (mAnimationFrame >= 11.0f && mAnimationFrame <= 12.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
		// ステップ2 : 攻撃開始(二回目)
	case 2:
		if (mAnimationFrame >= 13.0f && mAnimationFrame <= 17.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ3 : 攻撃終了
	case 3:
		if (mAnimationFrame >= 18.0f && mAnimationFrame <= 20.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
		// ステップ4 : 攻撃開始(三回目)
	case 4:
		if (mAnimationFrame >= 21.0f && mAnimationFrame < 25.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ5 : 攻撃終了
	case 5:
		if (mAnimationFrame >= 25.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}


// 攻撃3
void CMushroom::UpdateAttack3()
{
	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();
	SetAnimationSpeed(0.5f);
	mpAttackColHead->SetEnable(false);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃3アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack3);
		mStateStep++;
		break;
		// ステップ1 : 攻撃開始
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ2 : 攻撃の時に前に移動するか止まるか
	case 2:
		if (mAnimationFrame >= 0.0f)
		{
			if (vectorPos <= 25.0f)
			{
				mMoveSpeed += newPos * MOVE_SPEED;
				mStateStep++;
			}
			else if (vectorPos >= 26.0f)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mStateStep++;
			}
		}
		break;
		// ステップ3 : 攻撃終了待ち
	case 3:
		if (mAnimationFrame >= 25.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃終了待ち
void  CMushroom::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle3);
	}
}

// ヒット
void CMushroom::UpdateHit()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eHit);

	if (IsAnimationFinished())
	{
		// めまいをfalseにする
		bool stan = false;
		// 確率を最小に0最大20
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			ChangeState(EState::eDizzy);
		}
		else
		{
			ChangeState(EState::eIdle3);
		}
	}
}

// 死ぬ
void CMushroom::UpdateDie()
{
	SetAnimationSpeed(0.25f);
	ChangeAnimation(EAnimType::eDie);

	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::MushroomDeath();
	}
}

// めまい(混乱)
void CMushroom::UpdateDizzy()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eDizzy);

	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// 移動
void CMushroom::UpdateRun()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eRun);
	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * MOVE_SPEED;
		// 回転する範囲であれば
		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	if (vectorPos <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle3);
	}
	// 攻撃用の待機モーションへ
	else if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle3);
	}
}

// 更新処理
void CMushroom::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;
	mMoveSpeed.Y(-GRAVITY);

	if (mState != EState::eAttack3)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 戦う前の待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// 戦う前の待機状態2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// 待機状態3
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃2
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// 攻撃3
	case EState::eAttack3:
		UpdateAttack3();
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
		// めまい(混乱)
	case EState::eDizzy:
		UpdateDizzy();
		break;
		// 移動
	case EState::eRun:
		UpdateRun();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState !=EState::eIdle && mState != EState::eIdle2 && mState != EState::eDie)
	{
		mpGameUI->SetHpGaugeOffsetPos(CVector(0.0f, 30.0f, 0.0f));
		mpGameUI->GetHpGauge()->SetShow(true);
	}
	else
	{
		mpGameUI->GetHpGauge()->SetShow(false);
	}

	if (mState == EState::eIdle3 || mState == EState::eRun || mState == EState::eHit)
	{
		mAttackTime++;

		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		if (mAttackTime > 200)
		{
			// 攻撃2
			bool Attack2 = false;
			// 攻撃3
			bool Attack3 = false;
			// 確率を最小に3最大6
			int probability2 = Math::Rand(2, 5);
			int probability3 = Math::Rand(6, 10);
			if (probability2 == 2)Attack2 = true;
			if (probability3 == 6)Attack3 = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else if (Attack3)
			{
				ChangeState(EState::eAttack3);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
			|| mState == EState::eAttackWait)
		{
			mAttackTime = 0;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE || mState == EState::eAttack3)
	{
		Position(Position() + mMoveSpeed);
	}

	if (player->Position().Y() >= 16.0f)
	{
		mpDamageColUmbrella->SetEnable(true);
	}
	if (player->Position().Y() >= 10.0f)
	{
		mpColliderSphereHead->SetEnable(true);
	}
	if (player->Position().Y() <= 0.5f)
	{
		mpDamageColUmbrella->SetEnable(false);
	}
	if (player->Position().Y() <= 5.0f)
	{
 		mpColliderSphereHead->SetEnable(false);
	}

	// キャラクターの更新
	CXCharacter::Update();
	
	// キャラの押し戻しコライダー
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereRoot->Update();

	// ダメージを受けるコライダー
	mpDamageColBody->Update();
	mpDamageColUmbrella->Update();
	mpDamageColRoot->Update();

	// 攻撃コライダー
	mpAttackColHead->Update();
	mpAttackColRoot->Update();

	mIsGrounded = false;

	CEnemy::Update();
}

// 衝突処理
void CMushroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColHead || self == mpAttackColRoot&& mState != EState::eIdle && mState != EState::eIdle2 &&
		mState != EState::eIdle3)
	{
		// キャラのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(1.0f,this, chara);

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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereRoot)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CMushroom::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack || mState==EState::eAttack2)
	{
		mpAttackColHead->SetEnable(true);
	}
	if (mState == EState::eAttack3)
	{
		mpAttackColRoot->SetEnable(true);
	}
}

// 攻撃終了
void CMushroom::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColHead->SetEnable(false);
	mpAttackColRoot->SetEnable(false);
}

// 1レベルアップ
void CMushroom::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CMushroom::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY2_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
	mpGameUI->SetLv(mCharaStatus.level);
}

// 被ダメージ処理
void CMushroom::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		ChangeState(EState::eHit);
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

// 死亡処理
void CMushroom::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}

// 描画
void CMushroom::Render()
{
	CXCharacter::Render();
}