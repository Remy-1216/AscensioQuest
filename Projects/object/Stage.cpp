#include "Stage.h"
#include "CharacterBase.h"
#include "LoadCsv.h"
#include "Player.h"
#include "Pad.h"
#include "EffectManager.h"
#include <string>

namespace
{
    // 当たり判定
    constexpr float kDefaultSize = 50.0f;	 // 周囲のポリゴン検出に使用する球の初期サイズ
    constexpr float kHitWidth = 8.0f;	     // 当たり判定カプセルの半径
    constexpr float kHitHeight = 40.0f;	     // 当たり判定カプセルの高さ
    constexpr float kHitBottom = 20.0f;	     // 当たり判定カプセルの位置
    constexpr float kHitBottom2 = -20.0f;	 // 当たり判定カプセルの位置
    constexpr int kHitTryNum = 16;		     // 壁押し出し処理の最大試行回数
    constexpr float kHitSlideLength = 5.0f;	 // 一度の壁押し出し処理でスライドさせる距離

    //ワープ関係
    constexpr float kWarpPointSphereRadius = 100.0f;

    //マップを描画する位置
    constexpr float kMapPosX = 1550.0f;
    constexpr float kMapPosY = 40.0f;

    //プレイヤーの位置表示を描画する位置

    //敵のいないステージの位置
    constexpr float kSafeAreaPosX = 1675.0f;
    constexpr float kSafeAreaPosY = 125.0f;

    //ステージ1と3のX座標
    constexpr float kMap1_3PosX = 1580.0f;

    //ステージ1と2のY座標
    constexpr float kMap1_2PosY = 35.0f;

    //ステージ2と4のX座標
    constexpr float kMap2_4PosX = 1775.0f;

    //ステージ3と4のX座標
    constexpr float kMap3_4PosY = 225.0f;

}

Stage::Stage(int stageKinds)
{
    m_stageKinds = stageKinds;
}

Stage::~Stage()
{
    m_pEffectManager->ClearEffect();
    m_pEffectManager.reset();
    m_pEffectManager = nullptr;

    //画像の削除
    DeleteGraph(m_bgHandle);

    //マップ画像の削除
    DeleteGraph(m_mapHandle);

    //モデルの削除
    MV1DeleteModel(m_stageHandle);
}
void Stage::Init()
{
    m_pEffectManager = std::make_shared<EffectManager>();
    m_pEffectManager->Init();

    //ロード
    if (m_stageKinds == Title || m_stageKinds == Select)
    {
        m_stageHandle = MV1LoadModel("data/model/stage/TitleStage.mv1");
        LoadCsv::GetInstance().LoadStageInfo(m_stage, "title");
        m_posX = m_stage.posX;
        m_posY = m_stage.posY;
        m_posZ = m_stage.posZ;
        m_size = m_stage.size;

        m_stagePos = VGet(-m_posX, -m_posY, -m_posZ);
    }
    if (m_stageKinds == Tutorial)
    {
        m_stageHandle = MV1LoadModel("data/model/stage/TutorialStage.mv1");
        LoadCsv::GetInstance().LoadStageInfo(m_stage, "tutorial");
        m_posX = m_stage.posX;
        m_posY = m_stage.posY;
        m_posZ = m_stage.posZ;
        m_size = m_stage.size;

        m_stagePos = VGet(-m_posX, -m_posY, -m_posZ);
    }

    if (m_stageKinds == Stage1)
    {
        m_stageHandle = MV1LoadModel("data/model/stage/Stage.mv1");

        m_mapHandle = LoadGraph("data/UI/Map.png");

        //ステージの情報を読み込む
        LoadCsv::GetInstance().LoadStageInfo(m_stage, "gamestage");
        m_posX = m_stage.posX;
        m_posY = m_stage.posY;
        m_posZ = m_stage.posZ;
        m_size = m_stage.size;

        m_stagePos = VGet(m_posX, -m_posY, m_posZ);


        m_playerPosHandle = LoadGraph("data/UI/playerPosition.png");
    }
    if (m_stageKinds == Stage2)
    {
        m_stageHandle = MV1LoadModel("data/model/stage/BossStage.mv1");

        //ステージの情報を読み込む
        LoadCsv::GetInstance().LoadStageInfo(m_stage, "gamestage");
        m_posX = m_stage.posX;
        m_posY = m_stage.posY;
        m_posZ = m_stage.posZ;
        m_size = m_stage.size;

        m_stagePos = VGet(m_posX, -m_posY, m_posZ);
    }

    //背景の画像をロードする
    m_bgHandle = LoadGraph("data/BG/bg.png");

    //ステージ1の場合
    if (m_stageKinds == Stage1)
    {
        //ワープ地点を読み込む
        LoadWarpPointPos();
    }

    //マップの端の位置を読み込む
    LoadCsv::GetInstance().LoadMapPosData(m_upperLeft, "upperLeft");
    LoadCsv::GetInstance().LoadMapPosData(m_lowerRight, "lowerRight");

    //モデルのサイズを調整する
    MV1SetScale(m_stageHandle, VGet(m_size, m_size, m_size));

    //モデルと座標位置を設定する
    MV1SetPosition(m_stageHandle, m_stagePos);
}

void Stage::Draw()
{
    //背景の描画
    DrawGraph(0, 0, m_bgHandle, false);

    //ステージを描画する
    MV1DrawModel(m_stageHandle);

    if (m_stageKinds == Stage1)
    {
        for (int i = 0; i < kWarpPointNum; i++)
        {
            m_pEffectManager->DrawWarpPointEffect(m_warpSource[i], i);

            DrawGraph(kMapPosX, kMapPosY, m_mapHandle, true);
        }
        for (int i = 0; i < kWarpPointNum; i++)
        {
            m_pEffectManager->DrawWarpPointEffect(m_warpTarger[i], i + kWarpPointNum);
        }
    }
}

void Stage::DrawShadowModel()
{
    //シャドウマップへステージを描画
    MV1DrawModel(m_stageHandle);

}

//プレイヤーの位置をマップに描画する
void Stage::DrawMap(Player& player)
{
    m_playerPos = player.GetPos();

    //敵のいないステージにいる場合
    if (m_playerPos.x >= m_upperLeft.map1PosX && m_playerPos.z <= m_upperLeft.map1PosZ && m_playerPos.x <= m_lowerRight.map1PosX && m_playerPos.z >= m_lowerRight.map1PosZ)
    {
        DrawGraph(kSafeAreaPosX, kSafeAreaPosY, m_playerPosHandle, true);
    }

    //ステージ1にいる場合
    else if (m_playerPos.x >= m_upperLeft.map2PosX && m_playerPos.z <= m_upperLeft.map2PosZ && m_playerPos.x <= m_lowerRight.map2PosX && m_playerPos.z >= m_lowerRight.map2PosZ)
    {
        DrawGraph(kMap1_3PosX, kMap1_2PosY, m_playerPosHandle, true);
    }

    //ステージ2にいる場合
    else if (m_playerPos.x >= m_upperLeft.map3PosX && m_playerPos.z <= m_upperLeft.map3PosZ && m_playerPos.x <= m_lowerRight.map3PosX && m_playerPos.z >= m_lowerRight.map3PosZ)
    {
        DrawGraph(kMap2_4PosX, kMap1_2PosY, m_playerPosHandle, true);
    }

    //ステージ3にいる場合
    else if (m_playerPos.x >= m_upperLeft.map4PosX && m_playerPos.z <= m_upperLeft.map4PosZ && m_playerPos.x <= m_lowerRight.map4PosX && m_playerPos.z >= m_lowerRight.map4PosZ)
    {
        DrawGraph(kMap1_3PosX, kMap3_4PosY, m_playerPosHandle, true);
    }

    //ステージ4にいる場合
    else if (m_playerPos.x >= m_upperLeft.map5PosX && m_playerPos.z <= m_upperLeft.map5PosZ && m_playerPos.x <= m_lowerRight.map5PosX && m_playerPos.z >= m_lowerRight.map5PosZ)
    {
        DrawGraph(kMap2_4PosX, kMap3_4PosY, m_playerPosHandle, true);
    }
}

//ワープ先のステージ名を描画する
void Stage::DrawWarpDestinationName()
{

}

//ワープポイントの情報を読み込む
void Stage::LoadWarpPointPos()
{
    for (int i = 0; i < kWarpPointNum; i++)
    {
        std::string warpPoint = "WarpPoint" + std::to_string(i);

        LoadCsv::GetInstance().LoadWarpPointPos(m_warpPointPos, warpPoint.c_str());
        m_warpSource[i] = VGet(m_warpPointPos.warpSourceX, m_warpPointPos.warpSourceY, m_warpPointPos.warpSourceZ);
        m_warpTarger[i] = VGet(m_warpPointPos.warpTargetX, m_warpPointPos.warpTargetY, m_warpPointPos.warpTargetZ);
    }
}

//プレイヤーがワープポイントに触れたかどうかを判定する
void Stage::WarpPoint(Player& player, const Pad& pad)
{
    m_playerPos = player.GetPos();

    for (int i = 0; i < kWarpPointNum; i++)
    {
        m_distance = VSub(m_playerPos, m_warpSource[i]);

        if (VSize(m_distance) < kWarpPointSphereRadius + player.GetSphereRadius())
        {
            player.Warp(pad, m_warpTarger[i]);
        }
    }

    for (int i = 0; i < kWarpPointNum; i++)
    {
        m_distance = VSub(m_playerPos, m_warpTarger[i]);

        if (VSize(m_distance) < kWarpPointSphereRadius + player.GetSphereRadius())
        {
            player.Warp(pad, m_warpSource[i]);
        }
    }
}

VECTOR Stage::CheckObjectCol(CharacterBase& character, const VECTOR& moveVec)
{
    m_oldPos = character.GetPos();		    // 移動前の座標	
    m_nextPos = VAdd(m_oldPos, moveVec);    // 移動後の座標

    // オブジェクトの周囲にあるステージポリゴンを取得する
    auto hitDim = MV1CollCheck_Sphere(m_stageHandle, -1, m_oldPos, kDefaultSize + VSize(moveVec));

    // 検出されたポリゴンが壁ポリゴンか床ポリゴンか判別し、保存する
    // MEMO:壁ポリゴン(XZ平面に垂直なポリゴン)、床ポリゴン(XZ平面に垂直でないポリゴン)
    AnalyzeWallAndFloor(hitDim, m_oldPos);

    // 壁ポリゴンとの当たり判定をチェックする
    m_nextPos = CheckHitPlayerWithWall(m_nextPos);
    // 床ポリゴンとの当たり判定をチェックする
    m_nextPos = CheckHitPlayerWithFloor(m_nextPos);

    // 検出したプレイヤーの周囲のポリゴン情報の後始末をする
    MV1CollResultPolyDimTerminate(hitDim);

    return m_nextPos;
}

//プレイヤーとマップの壁の当たり判定
void Stage::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition)
{
    // 壁ポリゴンと床ポリゴンの数を初期化する
    m_wallNum = 0;
    m_floorNum = 0;

    // 検出されたポリゴンの数だけ繰り返す
    for (int i = 0; i < hitDim.HitNum; i++)
    {
        // XZ平面に垂直かどうか
        // MEMO:垂直かどうかはポリゴンの法線のY成分が0に限りなく近いかどうかで判断する
        bool isVertical = hitDim.Dim[i].Normal.y < 0.1f && hitDim.Dim[i].Normal.y > -0.1f;

        // XZ平面に垂直な場合(壁ポリゴンと判断された場合)
        if (isVertical)
        {
            // 壁ポリゴンと判断された場合でも、プレイヤーのY座標+1.0fより高いポリゴンのみ当たり判定を行う
            if (hitDim.Dim[i].Position[0].y > checkPosition.y + 1.0f ||
                hitDim.Dim[i].Position[1].y > checkPosition.y + 1.0f ||
                hitDim.Dim[i].Position[2].y > checkPosition.y + 1.0f)
            {
                // ポリゴンの数が列挙できる限界数に達していない場合、ポリゴンを配列に追加する
                if (m_wallNum < MaxHitColl)
                {
                    // ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
                    m_wall[m_wallNum] = &hitDim.Dim[i];
                    m_wallNum++;
                }
            }
        }
        // XZ平面に垂直でない場合
        else
        {
            // ポリゴンの数が列挙できる限界数に達していない場合、ポリゴンを配列に追加
            if (m_floorNum < MaxHitColl)
            {
                // ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
                m_floor[m_floorNum] = &hitDim.Dim[i];
                m_floorNum++;
            }
        }
    }
}

// 壁ポリゴンと床ポリゴンの数を取得する
VECTOR Stage::CheckHitPlayerWithWall(const VECTOR& checkPosition)
{
    // 補正後の位置
    VECTOR fixedPos = checkPosition;

    if (m_wallNum == 0) return fixedPos;

    // 壁からの押し出し処理を行う
    for (int i = 0; i < kHitTryNum; i++)
    {
        // 当たる可能性のある壁ポリゴンを全て見る
        bool isHitWall = false;
        for (int i = 0; i < m_wallNum; i++)
        {
            // i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
            auto poly = m_wall[i];

            // オブジェクトの終点
            VECTOR fixedEndPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));
            // オブジェクトが壁に当たっているか
            bool isHit = HitCheck_Capsule_Triangle(fixedPos, fixedEndPos, kHitWidth, poly->Position[0], poly->Position[1], poly->Position[2]);

            // オブジェクトと当たっている場合
            if (isHit)
            {
                // オブジェクトを壁の法線方向に移動させる
                fixedPos = VAdd(fixedPos, VScale(poly->Normal, kHitSlideLength));

                // 移動した壁ポリゴンと接触しているかどうか判定する
                for (int j = 0; j < m_wallNum; j++)
                {
                    poly = m_wall[j];
                    if (isHit)
                    {
                        isHitWall = true;
                        break;
                    }
                }

                // 全てのポリゴンと当たっていなかったらループ終了
                if (!isHitWall) break;
            }
        }

        // 全ての壁ポリゴンと接触しなくなったらループから抜ける
        if (!isHitWall) break;
    }

    return fixedPos;
}

//
VECTOR Stage::CheckHitPlayerWithFloor(const VECTOR& checkPosition)
{
    VECTOR fixedPos = checkPosition;

    if (m_floorNum == 0) return fixedPos;

    bool isHitFloor = false;
    float maxY = 0.0f;

    for (int i = 0; i < m_floorNum; i++)
    {
        auto poly = m_floor[i]; // i番目の床ポリゴンのアドレス

        HITRESULT_LINE lineResult{};  // 線分とポリゴンとの当たり判定の結果を代入する構造体
        VECTOR topPos = VAdd(fixedPos, VGet(0.0f, kHitHeight, 0.0f));       // オブジェクトの始点先の位置
        VECTOR bottomPos = VAdd(fixedPos, VGet(0.0f, kHitBottom, 0.0f));    // オブジェクトの終点少し低い位置
        VECTOR bottomPos2 = VAdd(fixedPos, VGet(0.0f, kHitBottom2, 0.0f));  // オブジェクトの終点さらに低い位置

        // 始点からそこそこ低い位置の間で当たっているかを判定する
        lineResult = HitCheck_Line_Triangle(topPos, bottomPos2, poly->Position[0], poly->Position[1], poly->Position[2]);

        // 既に当たったポリゴンがあり、検出した床ポリゴンより低い場合何もしない
        if (lineResult.HitFlag)
        {
            if (!(isHitFloor && maxY > lineResult.Position.y))
            {
                // 接触したY座標を保存する
                isHitFloor = true;
                maxY = lineResult.Position.y;
            }
        }
    }

    // 床ポリゴンに当たった場合
    if (isHitFloor)
    {
        fixedPos.y = maxY;
    }

    return fixedPos;
}
