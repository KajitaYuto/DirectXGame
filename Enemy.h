#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
#include "Audio.h"
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
class GameScene;

class Enemy {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, Model* modelBullet,Vector3& position,int Destination);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//�ڋ߃t�F�[�Y�������֐�
	void InitializeApproach();
	//���ˊԊu
	static const int kFireInterval = 15;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	void OnCollision();
	bool IsDead() const { return isDead_; }

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
	void Fire();

	void Approach();
	void Leave();

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//���˃^�C�}�[
	int32_t fireTimer_ = 0;
	//�f�X�t���O
	bool isDead_ = false;
	//HP
	int hitPoint_ = 3;
	//�q�b�g�G�t�F�N�g�^�C�}�[
	int hitTimer_ = 0;

	int destination_ = 0;

	uint32_t damageSE_ = 0;
};