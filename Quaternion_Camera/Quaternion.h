#pragma once
#include <math.h>
#include "DxLib.h"

//クォータニオン構造体
struct Quaternion
{
	double t;   //real
	double x;   //x
	double y;   //y
	double z;   //z
	VECTOR pos;
};

//クォータニオンの計算
Quaternion operator*(Quaternion q1, Quaternion q2);

//回転クォータニオン
Quaternion CreateRotationQuaternion(double radian, VECTOR Axis);

//位置クォータニオン
Quaternion CreateXYZToQuaternion(double PosX, double PosY, double PosZ);

//クォータニオンから回転行列へ
MATRIX QuaternionToMatrix(Quaternion q);