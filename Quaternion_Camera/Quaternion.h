#pragma once
#include <math.h>
#include "DxLib.h"

//�N�H�[�^�j�I���\����
struct Quaternion
{
	double t;   //real
	double x;   //x
	double y;   //y
	double z;   //z
	VECTOR pos;
};

//�N�H�[�^�j�I���̌v�Z
Quaternion operator*(Quaternion q1, Quaternion q2);

//��]�N�H�[�^�j�I��
Quaternion CreateRotationQuaternion(double radian, VECTOR Axis);

//�ʒu�N�H�[�^�j�I��
Quaternion CreateXYZToQuaternion(double PosX, double PosY, double PosZ);

//�N�H�[�^�j�I�������]�s���
MATRIX QuaternionToMatrix(Quaternion q);