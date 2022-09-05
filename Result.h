#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// �Q�[���V�[��
/// </summary>
class Result {

public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Result();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Result();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(int* enemyKill,int* Damage);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	int* GetMostKill() { return &mostKillNum_; }
	int* GetLeastDamage() { return &leastDamage_; }

	void SetScene(int* scene) { sceneNum_ = scene; }

private: // �����o�ϐ�
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//���U���g�p
	int* enemyKillNum_ = nullptr;
	int* totalDamage_ = nullptr;

	int mostKillNum_ = 0;
	int leastDamage_ = 0;

	int* sceneNum_ = nullptr;

};
