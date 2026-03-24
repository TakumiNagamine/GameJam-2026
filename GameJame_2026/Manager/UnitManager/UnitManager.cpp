#include "../UnitManager/UnitManager.h"
#include "../EnemyManager/EnemyManager.h"
#include "../../Object/Enemy/Enemy.h"
#include "../../InputControl/InputControl.h"
#include "DxLib.h"
#include <cstdlib>
#include <ctime> 
#include <algorithm>

// ===============================
// 矩形当たり判定（このcpp内専用）
// ===============================
//static bool IsColliding(
//    const Vector2D& posA, float wA, float hA,
//    const Vector2D& posB, float wB, float hB)
//{
//    float halfWA = wA * 0.5f;
//    float halfHA = hA * 0.5f;
//    float halfWB = wB * 0.5f;
//    float halfHB = hB * 0.5f;
//
//    if (posA.x + halfWA < posB.x - halfWB) return false;
//    if (posA.x - halfWA > posB.x + halfWB) return false;
//    if (posA.y + halfHA < posB.y - halfHB) return false;
//    if (posA.y - halfHA > posB.y + halfHB) return false;
//
//    return true;
//}

UnitManager::UnitManager()
    : hit_enemy{}
    , time{}
    , resource{}
    , cursor_se{}
    , char_se{}
{
}

void UnitManager::Initialize()
{
    units.clear();
    inputWait = 0;

    resource = ResourceManager::GetInstance();
    //cursor_move_se = resource->GetSounds("Resource/sounds/SE/cursor.mp3");

    cursor_se = resource->GetSounds("Resource/sounds/SE/cursor.mp3");
    char_se = resource->GetSounds("Resource/sounds/SE/set_player.mp3");
    
}

void UnitManager::SetGridInfo(
    int startX,
    int startY,
    int size,
    int col,
    int row)
{
    gridStartX = startX;
    gridStartY = startY;
    cellSize = size;
    gridCol = col;
    gridRow = row;

    cursorCol = 0;
    cursorRow = 0;
}

void UnitManager::Update(int enemy_damage,float delta_second)
{
    InputControl* input = InputControl::GetInstance();

    if (inputWait > 0)
        inputWait--;

    // カーソル移動(マス選択)
    if (inputWait == 0)
    {
        // 十字キー（上）
        if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == ePadInputState::ePress)
        {
            // カーソルSE
            PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
            cursorRow--;
            inputWait = 12;
        }

        // 十字キー（下）
        if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == ePadInputState::ePress)
        {
            // カーソルSE
            PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
            cursorRow++;
            inputWait = 12;
        }

        // 十字キー（左）
        if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == ePadInputState::ePress)
        {
            // カーソルSE
            PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
            cursorCol--;
            inputWait = 12;
        }

        // 十字キー（右）
        if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == ePadInputState::ePress)
        {
            // カーソルSE
            PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
            cursorCol++;
            inputWait = 12;
        }
    }

    // 範囲制限
    if (cursorCol < 0) cursorCol = 0;
    if (cursorCol >= gridCol) cursorCol = gridCol - 1;
    if (cursorRow < 0) cursorRow = 0;
    if (cursorRow >= gridRow) cursorRow = gridRow - 1;


    // カーソル移動（キャラ選択）
    // RBで次のキャラ
    if (input->GetButtonInputState(XINPUT_BUTTON_RIGHT_SHOULDER) == ePadInputState::ePress)
    {

        if (selectedUnitType == UnitType::Character1)
            selectedUnitType = UnitType::Character2;
        else if (selectedUnitType == UnitType::Character2)
            selectedUnitType = UnitType::Character3;
        else
            selectedUnitType = UnitType::Character1;
    }

    // LBで前のキャラ
    if (input->GetButtonInputState(XINPUT_BUTTON_LEFT_SHOULDER) == ePadInputState::ePress)
    {
        if (selectedUnitType == UnitType::Character1)
            selectedUnitType = UnitType::Character3;
        else if (selectedUnitType == UnitType::Character2)
            selectedUnitType = UnitType::Character1;
        else
            selectedUnitType = UnitType::Character2;
    }

    // =====================
    // Aボタン配置
    // =====================
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::ePress)
    {
        if (costManager == nullptr) return;

        // 生存数チェック
        int aliveCount = 0;
        for (const auto& u : units)
        {
            if (u.is_alive) aliveCount++;
        }
        if (aliveCount >= maxUnitCount) return;

        // 占有チェック
        if (IsOccupied(cursorCol, cursorRow)) return;

        // コスト
        int cost = GetUnitCost(selectedUnitType);
        if (!costManager->UseMoney(cost)) return;

        // ===============================
        // ★ カーソルセル中心を取得（←ここが本体）
        // ===============================
        float placeX, placeY;
        GetCellCenter(cursorCol, cursorRow, placeX, placeY);

        AddUnit(placeX, placeY, selectedUnitType);
    }

    // ユニット更新
    for (auto& u : units)
    {
        u.Update(enemy_damage, delta_second);
        
        
    }

    // 死亡ユニット削除
    RemoveDeadUnits();

    // 死亡ユニット削除
    /*for (int i = (int)units.size() - 1; i >= 0; i--)
    {
        if (!units[i].is_alive)
        {
            units.erase(units.begin() + i);
        }
    }*/

    // 表示画像のソート（座標が手前の画像を先に描画させる）（Unit自体の順番を入れ替える）
    // aとbの比較
    std::sort(units.begin(), units.end(), [](const Unit& a, const Unit& b)
        {
            // 
            return a.GetLocation().y < b.GetLocation().y;
        });
}

void UnitManager::Draw() const
{

    // ユニット
    for (const auto& u : units)
    {
        if (!u.is_alive) continue;  // ← これ重要
        u.Draw();
      
        //DrawExtendFormatString(70, u.location.y, 1.0, 1.0, GetColor(255, 255, 255), "%d", u.hp);

    }

    // 中央カーソル
    DrawGridCursor() ;

    // 下UIのキャラ選択カーソル

    // ユニットの数（選択できるキャラ数）
    int unitCount = 3;

    // UI全体の開始位置（左上座標）
    int uiStartX = 300;
    int uiStartY = 620;

    // UI全体のサイズ
    int uiWidth = 900;
    int uiHeight = 120;

    // 1キャラ分の横幅（UI全体をユニット数で分割）
    int oneWidth = uiWidth / unitCount;

    // 現在選択されているユニットの番号を取得
    int selectedIndex = (int)selectedUnitType;

    // 選択カーソルの左上座標を計算
    int uiX1 = uiStartX + oneWidth * selectedIndex;
    int uiY1 = uiStartY;

    // 選択カーソルの右下座標を計算
    int uiX2 = uiX1 + oneWidth;
    int uiY2 = uiY1 + uiHeight;

    // 選択中のキャラ枠を黄色の四角で表示
    DrawBox(uiX1, uiY1, uiX2, uiY2, GetColor(255, 255, 0), FALSE);

    // 外側に少し大きい枠を描いて強調表示
    DrawBox(uiX1 - 2, uiY1 - 2, uiX2 + 2, uiY2 + 2, GetColor(255, 255, 0), FALSE);


    // マス目カーソル座標デバッグ
    //DrawFormatString(10, 10, GetColor(255, 255, 255),
    //    "Cursor : %d %d", cursorCol, cursorRow);

    //// セレクトキャラデバッグ用
    //DrawFormatString(10, 100, GetColor(255, 255, 0),
    //    "Selected : %d", (int)selectedUnitType);
   
}

void UnitManager::DrawGridCursor() const
{
    // ===== GridDraw と完全に同じ設定 =====
    static const int GRID_COL = 8;
    static const int GRID_ROW = 6;
    static const int CELL_SIZE = 80;

    float gridScaleX = 1.25f;
    float gridScaleY = 1.15f;

    int gridStartX_local = 270;
    int gridStartY_local = 50;

    int trapezoidTopInset = 40;
    int trapezoidTopOffsetY = 40;

    float gridWidth = GRID_COL * CELL_SIZE * gridScaleX;
    float gridHeight = GRID_ROW * CELL_SIZE * gridScaleY;

    // ===== 台形4頂点 =====
    int topLeftX = gridStartX_local + trapezoidTopInset;
    int topRightX = gridStartX_local + (int)gridWidth - trapezoidTopInset;
    int topY = gridStartY_local + trapezoidTopOffsetY;

    int bottomLeftX = gridStartX_local;
    int bottomRightX = gridStartX_local + (int)gridWidth;
    int bottomY = gridStartY_local + (int)gridHeight;

    // ===== カーソル行の上下ライン =====
    float ty0 = (float)cursorRow / GRID_ROW;
    float ty1 = (float)(cursorRow + 1) / GRID_ROW;

    int leftX0 = (int)(topLeftX + (bottomLeftX - topLeftX) * ty0);
    int rightX0 = (int)(topRightX + (bottomRightX - topRightX) * ty0);
    int posY0 = (int)(topY + (bottomY - topY) * ty0);

    int leftX1 = (int)(topLeftX + (bottomLeftX - topLeftX) * ty1);
    int rightX1 = (int)(topRightX + (bottomRightX - topRightX) * ty1);
    int posY1 = (int)(topY + (bottomY - topY) * ty1);

    // ===== カーソル列の左右 =====
    float tx0 = (float)cursorCol / GRID_COL;
    float tx1 = (float)(cursorCol + 1) / GRID_COL;

    int x1 = (int)(leftX0 + (rightX0 - leftX0) * tx0);
    int x2 = (int)(leftX0 + (rightX0 - leftX0) * tx1);
    int x3 = (int)(leftX1 + (rightX1 - leftX1) * tx0);
    int x4 = (int)(leftX1 + (rightX1 - leftX1) * tx1);

    unsigned int col = GetColor(255, 0, 0);

    // ===== 台形セル枠 =====
    DrawLine(x1, posY0, x2, posY0, col);
    DrawLine(x2, posY0, x4, posY1, col);
    DrawLine(x4, posY1, x3, posY1, col);
    DrawLine(x3, posY1, x1, posY0, col);
}

void UnitManager::GetCellCenter(int col, int row, float& outX, float& outY) const
{
    // GridDraw と完全一致
    static const int GRID_COL = 8;
    static const int GRID_ROW = 6;
    static const int CELL_SIZE = 80;

    float gridScaleX = 1.25f;
    float gridScaleY = 1.15f;

    int gridStartX_local = 270;
    int gridStartY_local = 50;

    int trapezoidTopInset = 40;
    int trapezoidTopOffsetY = 40;

    float gridWidth = GRID_COL * CELL_SIZE * gridScaleX;
    float gridHeight = GRID_ROW * CELL_SIZE * gridScaleY;

    // 台形頂点
    int topLeftX = gridStartX_local + trapezoidTopInset;
    int topRightX = gridStartX_local + (int)gridWidth - trapezoidTopInset;
    int topY = gridStartY_local + trapezoidTopOffsetY;

    int bottomLeftX = gridStartX_local;
    int bottomRightX = gridStartX_local + (int)gridWidth;
    int bottomY = gridStartY_local + (int)gridHeight;

    // 行補間 
    float ty = (row + 0.5f) / GRID_ROW;

    float leftX =
        topLeftX + (bottomLeftX - topLeftX) * ty;

    float rightX =
        topRightX + (bottomRightX - topRightX) * ty;

    float posY =
        topY + (bottomY - topY) * ty;

    // 列補間
    float tx = (col + 0.5f) / GRID_COL;

    outX = leftX + (rightX - leftX) * tx;
    outY = posY;
}

void UnitManager::AddUnit(float x, float y, UnitType type)
{
    Unit unit(type);

    //// 念のためログ
    //printfDx("Unit Create : %.1f %.1f\n", x, y);

    unit.Initialize(x, y);
    units.push_back(unit);

    // キャラクター生成時のSE
    PlaySoundMem(char_se, DX_PLAYTYPE_BACK);
}

bool UnitManager::IsOccupied(int col, int row) const
{
    for (const auto& u : units)
    {
        Vector2D pos = u.GetLocation();

        int unitCol = (int)((pos.x - gridStartX) / cellSize);
        int unitRow = (int)((pos.y - gridStartY) / cellSize);

        if (unitCol == col && unitRow == row)
        {
            return true;
        }
    }
    return false;
}

void UnitManager::RemoveDeadUnits()
{
    units.erase(
        std::remove_if(units.begin(), units.end(),
            [](const Unit& u)
            {
                return !u.is_alive;
            }),
        units.end());
}

void UnitManager::SetCostManager(CostManager* cost)
{
    costManager = cost;
}

// ======================================================
// ★ Enemyとの衝突判定（矩形 × 矩形）
// ======================================================
void UnitManager::CheckCollisionWithEnemies(EnemyManager& enemyManager, float delta_second)
{    
    // Enemy一覧を取得
    auto& enemies = enemyManager.GetEnemies();

    // Unitを1体ずつ処理
    for (auto& u : units)
    {
        // 死んでいるUnitは無視
        if (!u.is_alive) continue;

        // ヒット判定をfalse
        hit_enemy = false;

        // Enemyを1体ずつ処理
        for (auto& e : enemies)
        {
            if (!e.is_alive) continue;

            if (CollisionManager::CheckBoxCollision(
                u.GetLocation(), u.GetCollisionWidth(), u.GetCollisionHight(),
                e.location, e.GetCollisionWidth(), e.GetCollisionHight()))
            {

                hit_enemy = true;

                // Unitのバトル処理（Enemyから攻撃を受ける）
                u.UpdateBattle(e.GetDamage(), delta_second);

                break; // 1体と当たったら次のUnitへ
            }
            
        }

        if (!hit_enemy)
        {
            // 時間の加算
            time += delta_second;

            // 1秒経過したら
            if (time >= 1.5f)
            {
                u.ResetAttack();
                time = 0.0f;
            }
            
        }
    }

    // 死んだUnitを削除
    for (int i = (int)units.size() - 1; i >= 0; i--)
    {
        if (!units[i].is_alive)
        {
            units.erase(units.begin() + i);
        }
    }
}

std::vector<Unit>& UnitManager::GetUnits()
{
    return units;
}

void UnitManager::SetDebugCollision(bool flag)
{
    debugDrawCollision = flag;
}

int UnitManager::GetDamage() const
{
    int damage = 0;

    for (const auto& u : units)
    {
        damage += u.GetDamage();
    }

    return damage;
}