#include "CRich.h"
#include "CCollisionManager.h"
#include "CCane.h"
#include "CPlayer.h"
#include "Maths.h"
#include "CMagicCircle.h"
#include "CInput.h"

// リッチのインスタンス
CRich* CRich::spInstance = nullptr;

#define ENEMY_HEIGHT    2.0f
#define ROTATE_RANGE  250.0f  // 回転する範囲

// リッチのアニメーションデータのテーブル
const CRich::AnimData CRich::ANIM_DATA[] =
{
	{ "",								                  true,	   0.0f,  0.0f},  // Tポーズ
	{ "Character\\Enemy\\Rich\\animation\\RichIdle.x",	  true,   41.0f,  0.4f},  // 待機 41.0f
	{ "Character\\Enemy\\Rich\\animation\\RichAttack.x",  false,  41.0f,  0.4f},  // 攻撃 41.0f
	{ "Character\\Enemy\\Rich\\animation\\RichAttack2.x", false,  71.0f,  0.5f},  // 攻撃 71.0f
	{ "Character\\Enemy\\Rich\\animation\\RichGetHit.x",  false,  41.0f,  0.4f},  // ヒット  41.0f
	{ "Character\\Enemy\\Rich\\animation\\RichDie.x",	  false,  29.0f,  0.5f},  // 死ぬ 29.0f
	{ "Character\\Enemy\\Rich\\animation\\RichVictory.x", false,  81.0f,  0.4f},  // 勝利 81.0f
	//{ "Character\\Enemy\\Rich\\animation\\RichRun.x",	true,	21.0f,  0.5f},	// 走る 21.0f
};

// コンストラクタ
CRich::CRich()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mAttackTime(0)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Rich");

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

	SetAnimationSpeed(0.4f);
	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクターの押し戻しコライダーを作成(体)
	mpColCapsuleBody = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 1.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		10.0f, false, 20.0f
	);
	mpColCapsuleBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColCapsuleBody->Position(0.0f, -5.5f, -1.0f);

	// キャラクターの押し戻しコライダーを作成(左腕)
	mpColSphereArmL = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.35f, false, 20.0f
	);
	mpColSphereArmL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereArmL->Position(0.0f, -0.15f, 0.0f);

	// キャラクターの押し戻しコライダーを作成(右腕)
	mpColSphereArmR = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.35f, false, 20.0f
	);
	mpColSphereArmR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereArmR->Position(0.0f, -0.15f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 1.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		10.0f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -5.5f, -1.0f);

	// ダメージを受けるコライダーを作成(左腕)
	mpDamageColArmL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	mpDamageColArmL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmL->Position(0.0f, -0.15f, 0.0f);

	// ダメージを受けるコライダーを作成(右腕)
	mpDamageColArmR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	mpDamageColArmR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmR->Position(0.0f, -0.15f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーをリッチの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Spine");
	mpColCapsuleBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// 押し戻しコライダーとダメージを受けるコライダーをリッチの左腕の行列にアタッチ
	const CMatrix* armLeftMty = GetFrameMtx("Armature_Hand_L");
	mpColSphereArmL->SetAttachMtx(armLeftMty);
	mpDamageColArmL->SetAttachMtx(armLeftMty);

	// 押し戻しコライダーとダメージを受けるコライダーをリッチの右腕の行列にアタッチ
	const CMatrix* armRightMty = GetFrameMtx("Armature_Hand_R");
	mpColSphereArmR->SetAttachMtx(armRightMty);
	mpDamageColArmR->SetAttachMtx(armRightMty);
}

// デストラクタ
CRich::~CRich()
{
	// 線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColCapsuleBody);
	SAFE_DELETE(mpColSphereArmL);
	SAFE_DELETE(mpColSphereArmR);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColArmL);
	SAFE_DELETE(mpDamageColArmR);
}

// インスタンス
CRich* CRich::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CRich::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CRich::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
}

// 待機状態
void CRich::UpdateIdle()
{
	SetAnimationSpeed(0.4f);
	// 待機アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= 50.0f)
	{
		ChangeState(EState::eIdle2);
	}
	else if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// 待機状態2
void CRich::UpdateIdle2()
{
	SetAnimationSpeed(0.4f);
	// 待機アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// 攻撃
void CRich::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);
	
	//magicCircle->Position(0.0f, 0.0f, 0.0f);

	if (mAnimationFrame >= 41.0f)
	{
		ChangeState(EState::eAttackWait);
	}
}

// 攻撃2
void CRich::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 71.0f)
	{
		ChangeState(EState::eAttackWait);
	}
}

// 攻撃終了待ち
void CRich::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// ヒット
void CRich::UpdateHit()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

//更新処理
void CRich::Update()
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
	}

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState == EState::eIdle2)
	{
		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		mAttackTime++;
		if (mAttackTime >= 300)
		{
			ChangeState(EState::eAttack);
			//ChangeState(EState::eAttack2);
		}
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラクターの押し戻しコライダー
	mpColCapsuleBody->Update();
	mpColSphereArmL->Update();
	mpColSphereArmR->Update();

	// ダメージを受けるコライダー
	mpDamageColBody->Update();
	mpDamageColArmL->Update();
	mpDamageColArmR->Update();

	if (CInput::PushKey('Z'))
	{
		CMagicCircle* magicCircle = new CMagicCircle
		(
			this,
			Position() + CVector(0.0f, -9.65f, 0.0f)
		);
		magicCircle->SetColor(CColor::red);
		magicCircle->Scale(15.0f, 15.0f, 15.0f);
	}

	mIsGrounded = false;
	CDebugPrint::Print("距離 %f\n", vectorPos);
}

// 衝突処理
void CRich::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColCapsuleBody || self == mpColSphereArmL || self == mpColSphereArmR)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 1レベルアップ
void CRich::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CRich::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY10_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CRich::TakeDamage(int damage, CObjectBase* causedObj)
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
		//Death();
	}
}

// 描画
void CRich::Render()
{
	CXCharacter::Render();
}