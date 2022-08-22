#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RailCamera {
public:
	void Initialize();

	void Update();

	ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};