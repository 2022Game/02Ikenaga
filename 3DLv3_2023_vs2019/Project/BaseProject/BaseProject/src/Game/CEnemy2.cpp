#include "CEnemy2.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// エネミー2のインスタンス
CEnemy2* CEnemy2::spInstance = nullptr;

#define MODEL_MUSHROOM "Character\\Mushroom\\Mushroom.x"

#define PLAYER_HEIGHT 2.0f

// エネミーのアニメーションデータのテーブル
const CEnemy2::AnimData CEnemy2::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Mushroom\\anim\\attack.x",	true,	26.0f	},	// 攻撃
};

// コンストラクタ
CEnemy2::CEnemy2()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_MUSHROOM);

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

CEnemy2::~CEnemy2()
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

CEnemy2* CEnemy2::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CEnemy2::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

void CEnemy2::UpdateIdle()
{
	// 待機アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle);
}

//更新処理
void CEnemy2::Update()
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
void CEnemy2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CEnemy2::Render()
{
	CXCharacter::Render();
}
