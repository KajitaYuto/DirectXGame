#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0,2,50};
}

void Enemy::Update() {
	//�f�X�t���O�̗������e���폜
	/*bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});*/

	//���[���h�s��
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	Rotate();
	Move();

	worldTransform_.Transform();

	//�L�����N�^�[�U������
	/*Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}*/

	debugText_->SetPos(50, 90);
	debugText_->Printf("Enemy:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection) {
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//�e�`��
	/*for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}*/
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

void Enemy::Attack() {
	//if (input_->TriggerKey(DIK_SPACE)) {
	//	//�e�̑��x
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//	velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

	//	//�e�𐶐����A������
	//	std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//	//�e��o�^����
	//	bullets_.push_back(std::move(newBullet));

	//}
}

void Enemy::Approach(){
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Vector3(0, 0, -0.25f);
	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave(){
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Vector3(-0.1f, 0.1f, -0.1f);
}
