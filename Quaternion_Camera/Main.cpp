
#include "DxLib.h"
#include "Quaternion.h"

//ループで必ず行う3大処理
int ProcessLoop()
{
    if (ProcessMessage() != 0) return -1;  //プロセス処理がエラーなら-1を返す
    if (ClearDrawScreen() != 0) return -1; //画面クリア処理がエラーなら-1を返す
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウインドウモードで起動
    ChangeWindowMode(TRUE);
    //ウィンドウタイトルの変更
    SetMainWindowText("クォータニオンテスト");

    //DXライブラリ
    if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0 || CheckHitKey(KEY_INPUT_ESCAPE))
        return -1;      //初期化と裏画面化

    int model;
    MATRIX mat;
    MATRIX matRot;
    MATRIX matTrans;

    float yaw, pitch, roll;
    yaw = pitch = roll = 0.0f;

    VECTOR xAxis, yAxis, zAxis;

    Quaternion quaternion;  //クォータニオン定義
    quaternion.x = quaternion.y = quaternion.z = 0.0f;
    quaternion.t = 1.0f;
    quaternion.pos = VGet(0, 0, -30);

    //モデルと座標を設定
    model = MV1LoadModel("Res/Monk.mv1");
    MV1SetScale(model, VGet(1, 1, 1));

    CreateIdentityMatrix(&mat);
    CreateIdentityMatrix(&matRot);

    //ディレクショナルライトの設定
    CreateDirLightHandle(VGet(0.0f, 1.0f, 0.0f));
    CreateDirLightHandle(VGet(0.0f, -1.0f, 0.0f));
    CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));
    CreateDirLightHandle(VGet(-1.0f, 0.0f, 0.0f));
    CreateDirLightHandle(VGet(0.0f, 0.0f, 1.0f));
    CreateDirLightHandle(VGet(0.0f, 0.0f, -1.0f));
    int speed = 5;

    //メインループ
    while (ProcessLoop() == 0)
    {
        //Zフラグ設定
        SetUseZBuffer3D(true);

        //カメラ設定
        SetCameraNearFar(0.1f, 10000.0f);
        SetCameraPositionAndAngle(VGet(0, 0, -30), 0.0f, 0.0f, 0.0f);
        //Quaternion.pos.x, Quaternion.pos.y, Quaternion.pos.z
        //リセット
        if (CheckHitKey(KEY_INPUT_R) != 0)
        {
            mat = MGetIdent();
            matRot = MGetIdent();
            quaternion.x = quaternion.y = quaternion.z = 0.0f;
            quaternion.t = 1.0f;
        }

        //yaw回転
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
        //pitch回転
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
        //roll回転
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

        mat = MGetScale(VGet(20, 20, 20));                    //スケール

        //ロール
        zAxis = VGet(mat.m[2][0], mat.m[2][1], mat.m[2][2]);
        quaternion = quaternion * CreateRotationQuaternion(roll, zAxis);
        //ピッチ
        xAxis = VGet(mat.m[0][0], mat.m[0][1], mat.m[0][2]);
        quaternion = quaternion * CreateRotationQuaternion(pitch, xAxis);
        //ヨー
        yAxis = VGet(mat.m[1][0], mat.m[1][1], mat.m[1][2]);
        quaternion = quaternion * CreateRotationQuaternion(yaw, yAxis);

        matRot = QuaternionToMatrix(quaternion);
        mat = MMult(mat, matRot);         //回転×スケール

        matTrans = MGetTranslate(VGet(0, 0, 0));
        mat = MMult(mat, matTrans);       //移動×回転×スケール

        MV1SetMatrix(model, mat);         //行列セット
        MV1DrawModel(model);

        ScreenFlip();
    }

    // ＤＸライブラリの後始末
    DxLib_End();
    // ソフトの終了
    return 0;
}