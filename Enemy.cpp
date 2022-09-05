#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
using namespace MathUtility;

void Enemy::Initialize(Model* model, Model* modelBullet,Vector3& position, int Destination) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	assert(modelBullet);
	modelBullet_ = modelBullet;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();


	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//SE読み込み
	damageSE_ = audio_->LoadWave("sound/hit.wav");

	destination_ = Destination;
}

void Enemy::Update() {
	if (hitTimer_ > 0)hitTimer_--;
	//ワールド行列
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	Rotate();
	Move();
}

void Enemy::InitializeApproach() {
	//発射タイマー
	fireTimer_ = kFireInterval;
}

void Enemy::Draw(ViewProjection viewProjection) {
	//3Dモデルの描画
	if(hitTimer_<=0)model_->Draw(worldTransform_, viewProjection);
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
}

void Enemy::Fire() {
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 2.0f;

	//自キャラの座標を取得
	Vector3 playerPos = player_->GetWorldPosition();
	//敵の座標を取得
	Vector3 enemyPos = GetWorldPosition();
	//差分ベクトルを計算
	Vector3 velocity = playerPos - enemyPos;
	//ベクトルを正規化
	MathUtility::Vector3Normalize(velocity);
	//ベクトルの長さを、速さに合わせる
	velocity *= kBulletSpeed;
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(modelBullet_, GetWorldPosition(), velocity);

	//弾を登録する
	gameScene_->AddEnemyBullet(std::move(newBullet));
}

void Enemy::Approach() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ += Vector3(0, 0, -0.25f);
	worldTransform_.Transform();
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

void Enemy::Leave() {
	Vector3 move;
	switch (destination_) {
	case 0:
		move = Vector3(-0.5f, 0.5f, -0.5f);
		break;
	case 1:
		move = Vector3(0.5f, 0.5f, -0.5f);
		break;
	case 2:
		move = Vector3(-0.5f, -0.5f, -0.5f);
		break;
	case 3:
		move = Vector3(0.5f, -0.5f, -0.5f);
		break;
	}
	//移動(ベクトルを加算)
	worldTransform_.translation_ +=move;
	worldTransform_.Transform();
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::OnCollision(){
	hitPoint_--;
	audio_->PlayWave(damageSE_, false, 0.25f);
	hitTimer_ = 2;
	if(hitPoint_<=0)isDead_ = true;
}
