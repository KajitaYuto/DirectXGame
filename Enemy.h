#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
#include "DebugText.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

//行動フェーズ
enum class Phase {
	Approach,	//接近する
	Leave,		//離脱する
};

class Player;

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//接近フェーズ初期化関数
	void InitializeApproach();
	//発射間隔
	static const int kFireInterval = 10;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets_; }
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
	void Fire();

	void Approach();
	void Leave();

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	Player* player_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//発射タイマー
	int32_t fireTimer_ = 0;
};