#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
using namespace MathUtility;

void Enemy::Initialize(Model* model,Vector3& position, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();


	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	//���[���h�s��
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	Rotate();
	Move();
}

void Enemy::InitializeApproach() {
	//���˃^�C�}�[
	fireTimer_ = kFireInterval;
}

void Enemy::Draw(ViewProjection viewProjection) {
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
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
	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	//���L�����̍��W���擾
	Vector3 playerPos = player_->GetWorldPosition();
	//�G�̍��W���擾
	Vector3 enemyPos = GetWorldPosition();
	//�����x�N�g�����v�Z
	Vector3 velocity = playerPos - enemyPos;
	//�x�N�g���𐳋K��
	MathUtility::Vector3Normalize(velocity);
	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;
	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, GetWorldPosition(), velocity);

	//�e��o�^����
	gameScene_->AddEnemyBullet(std::move(newBullet));
}

void Enemy::Approach() {
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Vector3(0, 0, -0.25f);
	worldTransform_.Transform();
	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	//���˃^�C�}�[�J�E���g�_�E��
	fireTimer_--;
	//�w�莞�ԂɒB����
	if (fireTimer_ <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Leave() {
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Vector3(-0.1f, 0.1f, -0.1f);
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
	isDead_ = true;
}
