#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

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
	
	//���[���h�s��̓]��
	worldTransform_.TransferMatrix();

	//�L�����N�^�[�U������
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	debugText_->SetPos(50, 70);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection) {
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
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

	//�s��X�V
	//���s�ړ��s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	worldTransform_.matWorld_ *= matTrans;
}

void Player::Rotate() {
	const float kRotSpeed = 0.05f;

	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
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
	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

	}
}
