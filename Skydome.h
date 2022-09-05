#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void SetRotation(Vector3 rot) { worldTransform_.rotation_ = rot; }
	void SetTranslation(Vector3 translation) { worldTransform_.translation_ = translation; }
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};