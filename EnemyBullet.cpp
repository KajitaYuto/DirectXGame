#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("red.jpg");

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() {
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;
	worldTransform_.Transform();

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}


void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
