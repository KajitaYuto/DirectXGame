#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
using namespace MathUtility;

void Enemy::Initialize(Model* model, Model* modelBullet,Vector3& position, int Destination) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	assert(modelBullet);
	modelBullet_ = modelBullet;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();


	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//SE�ǂݍ���
	damageSE_ = audio_->LoadWave("sound/hit.wav");

	destination_ = Destination;
}

void Enemy::Update() {
	if (hitTimer_ > 0)hitTimer_--;
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

	//�e�̑��x
	const float kBulletSpeed = 2.0f;

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
	newBullet->Initialize(modelBullet_, GetWorldPosition(), velocity);

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
	//�ړ�(�x�N�g�������Z)
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
