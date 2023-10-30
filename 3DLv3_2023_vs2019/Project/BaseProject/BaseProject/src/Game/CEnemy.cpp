#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// エネミーのインスタンス
CEnemy* CEnemy::spInstance = nullptr;

#define MODEL_SLIME "Character\\Slime\\Slime.x"

#define PLAYER_HEIGHT 2.0f

// エネミーのアニメーションデータのテーブル
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	//{ "Character\\Slime\\animation\\SlimeAttack.x",	true,	26.0f	},  //攻撃
	{ "Character\\Slime\\animation\\SlimeAttack2.x",	true,	80.0f	},  //攻撃2 26.0f
	//{ "Character\\Slime\\animation\\SlimeDie.x",	true,	81.0f	},  //死ぬ 41.0f
	//{ "Character\\Slime\\animation\\SlimeDizzy.x",	true,	100.0f	},  //めまい 41.0f
	//{ "Character\\Slime\\animation\\SlimeGetHit.x",	true,	100.0f	},  //ヒット 26.0f
	//{ "Character\\Slime\\animation\\SlimeIdleBattle.x",	true,	50.0f	},  //アイドルバトル 25.0f
	//{ "Character\\Slime\\animation\\SlimeIdleNormal.x",	true,	51.0f	},  //アイドル通常
	//{ "Character\\Slime\\animation\\SlimeRun.x",	true,	21.0f	},  //走る
	//{ "Character\\Slime\\animation\\SlimeSenseSomethingRoutine.x",	true,	71.0f	},  //見回す
	//{ "Character\\Slime\\animation\\SlimeSenseSomethingStart.x",	true,	63.0f	},  //開始の見回す
	//{ "Character\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  //挑発
	//{ "Character\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  //勝利
	//{ "Character\\Slime\\animation\\SlimeWalk.x",	true,	31.0f	},  //歩く
	//{ "Character\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  //後ろに歩く
    //{ "Character\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  //左に移動
	//{ "Character\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  //右に移動
};

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_SLIME);

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
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
}

CEnemy::~CEnemy()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
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

void CEnemy::UpdateIdle()
{
	// 待機アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle);
}

//更新処理
void CEnemy::Update()
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
	}

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;
}

// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
}

// 描画
void CEnemy::Render()
{
	CXCharacter::Render();
}
