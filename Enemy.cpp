#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	
	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 2.0f;
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	/*bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});*/

	//ワールド行列
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	Rotate();
	Move();

	worldTransform_.Transform();

	//キャラクター攻撃処理
	/*Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}*/

	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemy:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection) {
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	/*for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}*/
}

void Enemy::Move() {
	float moveSpeed = 0.2f;

	//移動処理
	worldTransform_.translation_.z -=moveSpeed;
}

void Enemy::Rotate() {
	const float kRotSpeed = 0.05f;

	/*if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}*/
}

void Enemy::Attack() {
	//if (input_->TriggerKey(DIK_SPACE)) {
	//	//弾の速度
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	//速度ベクトルを自機の向きに合わせて回転させる
	//	velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

	//	//弾を生成し、初期化
	//	std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//	//弾を登録する
	//	bullets_.push_back(std::move(newBullet));

	//}
}
