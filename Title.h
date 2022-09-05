#pragma once
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
class Title {

public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Title();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Title();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void SetKillandDamage(int* mostKill, int* leastDamage) {
		mostKillNum_ = mostKill;
		leastDamage_ = leastDamage;
	};

	void SetScene(int* scene) { sceneNum_ = scene; }


private: // �����o�ϐ�
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	//�X�v���C�g
	std::unique_ptr<Sprite>title_;
	std::unique_ptr<Sprite>title2_;
	//�V��
	Skydome* skydome_ = nullptr;

	//�X�R�A�p
	int* mostKillNum_ = nullptr;
	int* leastDamage_ = nullptr;

	const float kRotSpeed = 0.5f;

	int* sceneNum_ = nullptr;

	//�ҋ@���t���O
	bool isWait_ = true;
	const int kWaitTimer = 60;
	int waitTimer_ = kWaitTimer;

	Vector3 domeRot_ = { 0.0f,0.0f,0.0f };
};
