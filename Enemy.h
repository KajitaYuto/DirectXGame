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

//行動フェーズ
enum class Phase {
	Approach,	//接近する
	Leave,		//離脱する
};

class Player;
class GameScene;

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Model* modelBullet,Vector3& position,int Destination);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//接近フェーズ初期化関数
	void InitializeApproach();
	//発射間隔
	static const int kFireInterval = 15;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void OnCollision();
	bool IsDead() const { return isDead_; }

	static const int kRadius = 2;
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

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//発射タイマー
	int32_t fireTimer_ = 0;
	//デスフラグ
	bool isDead_ = false;
	//HP
	int hitPoint_ = 3;
	//ヒットエフェクトタイマー
	int hitTimer_ = 0;

	int destination_ = 0;

	uint32_t damageSE_ = 0;
};