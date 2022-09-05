#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"

class RailCamera {
public:
	void Initialize();

	void Update();

	void Move();

	ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	Input* input_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};