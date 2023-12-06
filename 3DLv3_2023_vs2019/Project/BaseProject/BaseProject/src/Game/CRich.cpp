#include "CRich.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CCane.h"

// リッチのインスタンス
CRich* CRich::spInstance = nullptr;

#define ENEMY_HEIGHT 2.0f

// リッチのアニメーションデータのテーブル
const CRich::AnimData CRich::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\animation\\RichAttack.x",	true,	90.0f	},	// 攻撃 41.0f
	//{ "Character\\animation\\RichAttack2.x",	true,	71.0f	},	// 攻撃 71.0f
	//{ "Character\\animation\\RichDie.x",	true,	100.0f	},	// 死ぬ 29.0f
	//{ "Character\\animation\\RichGetHit.x",true,	80.0f	},	// ヒット  41.0f
	//{ "Character\\animation\\RichIdle.x",	true,	80.0f	},	// 待機 41.0f
	//{ "Character\\animation\\RichRun.x",	true,	60.0f	},	// 走る 21.0f
	//{ "Character\\animation\\RichVictory.x",	true,	81.0f	},	// 勝利 81.0f
	//{ "Character\\animation\\RichWalk.x",	true,	80.0f	},	// 歩く 41.0f
};

// コンストラクタ
CRich::CRich()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Player2");

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

	mpCane = new CCane();
	mpCane->SetAttachMtx(GetFrameMtx("Hips_Hand_L"));
	mpCane->SetOwner(this);
}

CRich::~CRich()
{
	SAFE_DELETE(mpColliderLine);
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