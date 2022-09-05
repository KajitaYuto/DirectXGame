#include "Player.h"

void Player::Initialize(Model* modelPlayer, Model* modelBullet) {
	//NULL�|�C���^�`�F�b�N
	assert(modelPlayer);
	model_ = modelPlayer;
	assert(modelBullet);
	modelBullet_ = modelBullet;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.translation_ = Vector3(0.0f, -10.0f, 40.0f);
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�e�N�X�`���̎擾
	uint32_t texture_= TextureManager::Load("player/HPGauge/HPGauge.png");
	uint32_t HPTexture_= TextureManager::Load("player/HPGauge/HP.png");
	uint32_t ShieldTexture_ = TextureManager::Load("player/HPGauge/Shield.png");
	//�X�v���C�g����
	gauge1_.reset(Sprite::Create(texture_, { 760,580 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	gauge2_.reset(Sprite::Create(texture_, { 520,580 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	HPGauge_.reset(Sprite::Create(HPTexture_, { 520,580 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	shieldGauge_.reset(Sprite::Create(ShieldTexture_, { 760,580 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	//SE�ǂݍ���
	damageSE_= audio_->LoadWave("sound/damage.wav");
	shieldSE_ = audio_->LoadWave("sound/shield.wav");
	shieldBrakeSE_=audio_->LoadWave("sound/shieldBrake.wav");
	healSE_ = audio_->LoadWave("sound/heal.wav");
}

void Player::Update() {
	if (healTimer_ > 0) healTimer_--;
	if (healWaitTimer_ > 0) healWaitTimer_--;
	else if (shieldHP_ < 10 && healTimer_ <= 0) {
		audio_->StopWave(healSE_);
		audio_->PlayWave(healSE_, false, 0.05f);
		shieldHP_++;
		healTimer_ = 10;
	}

	//�Q�[�W�̃T�C�Y�X�V
	shieldGauge_->SetSize({ 16.0f,(float)shieldHP_/10*60});
	shieldGauge_->SetPosition({ 760,580 + ((float)60-((float)shieldHP_ / 10*60)) / 2 });
	HPGauge_->SetSize({ 16.0f,(float)HP_ / 20 * 60 });
	HPGauge_->SetPosition({ 520,580 + ((float)60 - ((float)HP_ / 20 * 60)) / 2 });

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
	debugText_->Printf("PlayerHP:%d, playerShield:%d\n%d",HP_,shieldHP_,audio_->IsPlaying(healSE_));
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

void Player::DrawUI(){
	shieldGauge_->Draw();
	gauge1_->Draw();
	HPGauge_->Draw();
	gauge2_->Draw();
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

void Player::OnCollision(){
	*totalDamage_+=1;
	healWaitTimer_ = 120;
	if (shieldHP_ > 0) {
		shieldHP_--;
		if(shieldHP_==0)audio_->PlayWave(shieldBrakeSE_,false,0.25f);
		else audio_->PlayWave(shieldSE_,false,0.5f);
	}
	else { 
		HP_--;
		if(!audio_->IsPlaying(damageSE_))audio_->PlayWave(damageSE_, false, 0.1f);
	}
	if (HP_ <= 0)isDead_ = true;
}

void Player::Move() {
	float moveSpeed = 0.5f;

	//�����������ňړ��x�N�g����ύX
	/*if (input_->PushKey(DIK_A)) {
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
	}*/

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
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0)isWait_ = false;
		return;
	}

	if (input_->PushKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0.2f, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(modelBullet_,GetWorldPosition(),GetWorldRotation(), velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

		isWait_ = true;
		waitTimer_ = 10;
	}
}
