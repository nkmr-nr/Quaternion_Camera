
#include "DxLib.h"
#include "Quaternion.h"

//���[�v�ŕK���s��3�又��
int ProcessLoop()
{
    if (ProcessMessage() != 0) return -1;  //�v���Z�X�������G���[�Ȃ�-1��Ԃ�
    if (ClearDrawScreen() != 0) return -1; //��ʃN���A�������G���[�Ȃ�-1��Ԃ�
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // �E�C���h�E���[�h�ŋN��
    ChangeWindowMode(TRUE);
    //�E�B���h�E�^�C�g���̕ύX
    SetMainWindowText("�N�H�[�^�j�I���e�X�g");

    //DX���C�u����
    if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0 || CheckHitKey(KEY_INPUT_ESCAPE))
        return -1;      //�������Ɨ���ʉ�

    int model;
    MATRIX mat;
    MATRIX matRot;
    MATRIX matTrans;

    float yaw, pitch, roll;
    yaw = pitch = roll = 0.0f;

    VECTOR xAxis, yAxis, zAxis;

    Quaternion quaternion;  //�N�H�[�^�j�I����`
    quaternion.x = quaternion.y = quaternion.z = 0.0f;
    quaternion.t = 1.0f;
    quaternion.pos = VGet(0, 0, -30);

    //���f���ƍ��W��ݒ�
    model = MV1LoadModel("Res/Monk.mv1");
    MV1SetScale(model, VGet(1, 1, 1));

    CreateIdentityMatrix(&mat);
    CreateIdentityMatrix(&matRot);

    //�f�B���N�V���i�����C�g�̐ݒ�
    CreateDirLightHandle(VGet(0.0f, 1.0f, 0.0f));
    CreateDirLightHandle(VGet(0.0f, -1.0f, 0.0f));
    CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));
    CreateDirLightHandle(VGet(-1.0f, 0.0f, 0.0f));
    CreateDirLightHandle(VGet(0.0f, 0.0f, 1.0f));
    CreateDirLightHandle(VGet(0.0f, 0.0f, -1.0f));
    int speed = 5;

    //���C�����[�v
    while (ProcessLoop() == 0)
    {
        //Z�t���O�ݒ�
        SetUseZBuffer3D(true);

        //�J�����ݒ�
        SetCameraNearFar(0.1f, 10000.0f);
        SetCameraPositionAndAngle(VGet(0, 0, -30), 0.0f, 0.0f, 0.0f);
        //Quaternion.pos.x, Quaternion.pos.y, Quaternion.pos.z
        //���Z�b�g
        if (CheckHitKey(KEY_INPUT_R) != 0)
        {
            mat = MGetIdent();
            matRot = MGetIdent();
            quaternion.x = quaternion.y = quaternion.z = 0.0f;
            quaternion.t = 1.0f;
        }

        //yaw��]
        bool yawFlag = false;
        if (CheckHitKey(KEY_INPUT_W))
        {
            quaternion.pos.x += speed;
        }
        if (CheckHitKey(KEY_INPUT_S))
        {
            quaternion.pos.x -= speed;
        }
        if (CheckHitKey(KEY_INPUT_LEFT) != 0)
        {
            yaw = 0.1f;
            yawFlag = true;
        }
        if (CheckHitKey(KEY_INPUT_RIGHT) != 0)
        {
            yaw = -0.1f;
            yawFlag = true;
        }
        if (!yawFlag)
        {
            yaw = 0.0f;
        }
        //pitch��]
        bool pitchFlag = false;
        if (CheckHitKey(KEY_INPUT_UP) != 0)
        {
            pitch = 0.1f;
            pitchFlag = true;
        }
        if (CheckHitKey(KEY_INPUT_DOWN) != 0)
        {
            pitch = -0.1f;
            pitchFlag = true;
        }
        if (!pitchFlag)
        {
            pitch = 0.0f;
        }
        //roll��]
        bool rollFlag = false;
        if (CheckHitKey(KEY_INPUT_Z) != 0)
        {
            roll = 0.1f;
            rollFlag = true;
        }
        if (CheckHitKey(KEY_INPUT_X) != 0)
        {
            roll = -0.1f;
            rollFlag = true;
        }
        if (!rollFlag)
        {
            roll = 0.0f;
        }

        mat = MGetScale(VGet(20, 20, 20));                    //�X�P�[��

        //���[��
        zAxis = VGet(mat.m[2][0], mat.m[2][1], mat.m[2][2]);
        quaternion = quaternion * CreateRotationQuaternion(roll, zAxis);
        //�s�b�`
        xAxis = VGet(mat.m[0][0], mat.m[0][1], mat.m[0][2]);
        quaternion = quaternion * CreateRotationQuaternion(pitch, xAxis);
        //���[
        yAxis = VGet(mat.m[1][0], mat.m[1][1], mat.m[1][2]);
        quaternion = quaternion * CreateRotationQuaternion(yaw, yAxis);

        matRot = QuaternionToMatrix(quaternion);
        mat = MMult(mat, matRot);         //��]�~�X�P�[��

        matTrans = MGetTranslate(VGet(0, 0, 0));
        mat = MMult(mat, matTrans);       //�ړ��~��]�~�X�P�[��

        MV1SetMatrix(model, mat);         //�s��Z�b�g
        MV1DrawModel(model);

        ScreenFlip();
    }

    // �c�w���C�u�����̌�n��
    DxLib_End();
    // �\�t�g�̏I��
    return 0;
}