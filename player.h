#pragma once
#include "Model.h"
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
	/// 初期化
	/// </summary>
	void Initialize(Model* modelPlayer,Model* modelBullet);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	//ワールド座標を取得
	Vector3 GetWorldPosition();
	Vector3 GetWorldRotation();

	void SetParent(WorldTransform* parent) { worldTransform_.parent_ = parent; }

	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets_; }
	
	static const int kRadius = 1;
private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;
};