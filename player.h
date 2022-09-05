#pragma once
#include "Model.h"
#include "Audio.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "assert.h"
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* modelPlayer,Model* modelBullet);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

	//���[���h���W���擾
	Vector3 GetWorldPosition();
	Vector3 GetWorldRotation();

	bool IsDead() const { return isDead_; }

	void SetParent(WorldTransform* parent) { worldTransform_.parent_ = parent; }
	void SetTotalDamage(int* totalDamage) { totalDamage_ = totalDamage; }

	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets_; }
	
	static const int kRadius = 2;
private:

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();
	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();
	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Audio* audio_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;
	//�X�v���C�g
	std::unique_ptr<Sprite>gauge1_;
	std::unique_ptr<Sprite>gauge2_;
	std::unique_ptr<Sprite>HPGauge_;
	std::unique_ptr<Sprite>shieldGauge_;

	//����
	uint32_t shieldSE_ = 0;
	uint32_t shieldBrakeSE_ = 0;
	uint32_t damageSE_ = 0;
	uint32_t healSE_ = 0;

	//HP
	int HP_ = 20;
	int shieldHP_ = 10;
	int healWaitTimer_ = 0;
	int healTimer_ = 0;

	int* totalDamage_ = nullptr;

	bool isDead_ = false;

	//�e���ˑҋ@���t���O
	bool isWait_ = false;
	int waitTimer_ = 0;
};