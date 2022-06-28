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
	worldTransform_.translation_ = {30,2,50};
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//ワールド行列
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	Rotate();
	Move();

	worldTransform_.Transform();

	//キャラクター攻撃処理
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemy:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::InitializeApproach(){
	//発射タイマー
	fireTimer_ = kFireInterval;
}

void Enemy::Draw(ViewProjection viewProjection) {
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Move() {
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}
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

void Enemy::Fire() {
		//弾の速度
		const float kBulletSpeed = -1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//弾を生成し、初期化
		std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
}

void Enemy::Approach(){
	//移動(ベクトルを加算)
	worldTransform_.translation_ += Vector3(0, 0, -0.25f);
	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	//発射タイマーカウントダウン
	fireTimer_--;
	//指定時間に達した
	if (fireTimer_ <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Leave(){
	//移動(ベクトルを加算)
	worldTransform_.translation_ += Vector3(-0.1f, 0.1f, -0.1f);
}
