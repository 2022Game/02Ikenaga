#include "CRich.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// リッチのインスタンス
CRich* CRich::spInstance = nullptr;

#define MODEL_RICH "Character\\Rich.x"

#define ENEMY_HEIGHT 2.0f

// リッチのアニメーションデータのテーブル
const CRich::AnimData CRich::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\RichAttack.x",	true,	90.0f	},	// 攻撃 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	true,	70.0f	},	// 攻撃 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	true,	70.0f	},	// 攻撃 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	true,	70.0f	},	// 攻撃 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	true,	70.0f	},	// めまい 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	true,	70.0f	},	// ヒット 23.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	true,	41.0f	},	// 待機 18.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	true,	41.0f	},	// 待機2 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	true,	21.0f	},	// 植物 21.0f
};

// コンストラクタ
CRich::CRich()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_RICH);

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
}

CRich::~CRich()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpModel);
}

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

void CRich::UpdateIdle()
{
	// 待機アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle);
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
	}

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;
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
}

// 描画
void CRich::Render()
{
	CXCharacter::Render();
}