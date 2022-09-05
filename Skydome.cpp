#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model){
	assert(model);
	model_ = model;

	worldTransform_.scale_ = { 1000.0f,1000.0f,1000.0f };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.translation_ = { 0,0,700 };
	worldTransform_.Initialize();
}

void Skydome::Update(){
	worldTransform_.Transform();
}

void Skydome::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
}
