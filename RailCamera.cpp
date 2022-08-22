#include "RailCamera.h"
#include <math.h>

using namespace MathUtility;

void RailCamera::Initialize(){
	//�J�����̈ʒu��ݒ肷��
	worldTransform_.translation_ = Vector3(0.0f, 0.0f, -50.0f);
	//�J�����̉�]��ݒ肷��
	worldTransform_.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	worldTransform_.Initialize();

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ=1000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update(){
	//�J���������ɓ�����
	worldTransform_.translation_.z -= 0.1f;
	worldTransform_.Transform();

	viewProjection_.eye = worldTransform_.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = Vector3TransformNormal(forward, worldTransform_.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f
	viewProjection_.up = Vector3TransformNormal(up, worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V�����̍X�V
	viewProjection_.UpdateMatrix();
	//�萔�o�b�t�@�ւ̓]��
	viewProjection_.TransferMatrix();
}
