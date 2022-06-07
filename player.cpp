#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	//ワールド行列
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	Rotate();
	Move();
	
	//ワールド行列の転送
	worldTransform_.TransferMatrix();

	//キャラクター攻撃処理
	Attack();
	if (bullet_) {
		bullet_->Update();
	}

	debugText_->SetPos(50, 70);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection) {
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

void Player::Move() {
	float moveSpeed = 0.5f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= moveSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += moveSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += moveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= moveSpeed;
	}

	//行列更新
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	worldTransform_.matWorld_ *= matTrans;
}

void Player::Rotate() {
	const float kRotSpeed = 0.05f;

	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
	matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);

	worldTransform_.matWorld_ *= matRotY;
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet;
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullet_ = newBullet;

	}
}
