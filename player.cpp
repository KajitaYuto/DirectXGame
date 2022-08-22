#include "Player.h"

void Player::Initialize(Model* modelPlayer, Model* modelBullet) {
	//NULL�|�C���^�`�F�b�N
	assert(modelPlayer);
	model_ = modelPlayer;
	assert(modelBullet);
	this->modelBullet_ = modelBullet;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = Vector3(0.0f, 0.0f, 50.0f);
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update() {
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//���[���h�s��
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	Rotate();
	Move();
	
	worldTransform_.Transform();

	//�L�����N�^�[�U������
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

#ifdef _DEBUG
	debugText_->SetPos(50, 70);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
#endif
}

void Player::Draw(ViewProjection viewProjection) {
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection);
	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x=worldTransform_.matWorld_.m[3][0];
	worldPos.y=worldTransform_.matWorld_.m[3][1];
	worldPos.z=worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetWorldRotation()
{
	Vector3 worldRota;
	worldRota.x = worldTransform_.rotation_.x;
	worldRota.y = worldTransform_.rotation_.y;
	worldRota.z = worldTransform_.rotation_.z;
	return worldRota;
}

void Player::OnCollision()
{
}

void Player::Move() {
	float moveSpeed = 0.5f;

	//�����������ňړ��x�N�g����ύX
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

	//�ړ����E���W
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x=max(worldTransform_.translation_.x,-kMoveLimitX);
	worldTransform_.translation_.x=min(worldTransform_.translation_.x,+kMoveLimitX);
	worldTransform_.translation_.y=max(worldTransform_.translation_.y,-kMoveLimitY);
	worldTransform_.translation_.y=min(worldTransform_.translation_.y,+kMoveLimitY);
}

void Player::Rotate() {
	const float kRotSpeed = 0.05f;

	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(modelBullet_,GetWorldPosition(),GetWorldRotation(), velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

	}
}
