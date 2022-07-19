#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
#include "DebugText.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

//�s���t�F�[�Y
enum class Phase {
	Approach,	//�ڋ߂���
	Leave,		//���E����
};

class Player;

class Enemy {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//�ڋ߃t�F�[�Y�������֐�
	void InitializeApproach();
	//���ˊԊu
	static const int kFireInterval = 10;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets_; }
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
	void Fire();

	void Approach();
	void Leave();

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	Player* player_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//���˃^�C�}�[
	int32_t fireTimer_ = 0;
};