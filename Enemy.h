#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
#include "DebugText.h"
#include "Input.h"
#include <memory>
#include <list>

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

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

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

	/*std::list<std::unique_ptr<PlayerBullet>> bullets_;*/

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};