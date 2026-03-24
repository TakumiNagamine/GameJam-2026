#pragma once
#include <vector>
#include "../../Object/Unit/Unit.h"
#include "../EnemyManager/EnemyManager.h"
#include "../../InputControl/InputControl.h"
#include "../CollisionManager/CollisionManager.h"
#include "../../Object/Unit/UnitType.h"
#include "../CostManager/CostManager.h"
#include "../../Manager/ResourceManager/ResourceManager.h"

class EnemyManager;

class UnitManager
{
private:
    std::vector<Unit> units;    // 可変長変数、配列的なイメージ
    bool debugDrawCollision = true;
    int maxUnitCount = 4;       // ユニットを配置できる数制限

    CostManager* costManager = nullptr; //コスト管理

    ResourceManager* resource;

    // グリッド情報
    int gridStartX = 0;
    int gridStartY = 0;
    int cellSize = 80;
    int gridCol = 0;
    int gridRow = 0;

    // カーソル
    int cursorCol = 0;
    int cursorRow = 0;
    int cursor_se;          // カーソルのSE

    int char_se;            // キャラクター配置時のSE

    int inputWait = 0;

    bool hit_enemy;
    float time;

    UnitType selectedUnitType = UnitType::Character1;

public:
    UnitManager();
    ~UnitManager() = default;

    void Initialize();
    void Update(int enemy_damage,float delta_second);
    void Draw() const;
    void DrawGridCursor() const;

    void SetGridInfo(
        int startX,
        int startY,
        int size,
        int col,
        int row
    );

    void GetCellCenter(int col, int row, float& outX, float& outY) const;
    void AddUnit(float x, float y, UnitType type);
    bool IsOccupied(int col, int row) const;
    void RemoveDeadUnits();
    void SetCostManager(CostManager* cost);

    std::vector<Unit>& GetUnits();
    void CheckCollisionWithEnemies(EnemyManager& enemyManager, float delta_second);

    void SetDebugCollision(bool flag);


    int GetDamage()const;
};