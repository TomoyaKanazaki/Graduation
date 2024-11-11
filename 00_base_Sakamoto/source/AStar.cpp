//============================================
//
//	AStar処理 [AStar.cpp]
//	Author:外部ライブラリ
//
//============================================

#include "AStar.h"
#include <algorithm>
#include <math.h>

//===============================================================
// 定数定義
//===============================================================
using namespace std::placeholders;      // 場所の保持

//===============================================================
// 2つのVec2iオブジェクトが同じ座標を持つかチェック
//===============================================================
bool AStar::Vec2i::operator == (const Vec2i& coordinates_)
{
    return (x == coordinates_.x && y == coordinates_.y);
}

//===============================================================
// 探索経路のノード
//===============================================================
AStar::Node::Node(Vec2i coordinates_, Node *parent_)
{
    // ノード座標
    parent = parent_;

    // 親ノードへのポインタで、経路を逆にたどるときに使用
    coordinates = coordinates_;

    // スタートからノードまでの移動コスト(G)とヒューリスティック値（ゴールまでの推定コスト）(H)
    G = H = 0;
}

//===============================================================
// 移動コストと推定コストの合計値
//===============================================================
AStar::uint AStar::Node::getScore()
{
    return G + H;
}

//===============================================================
// 経路を生成するクラス
//===============================================================
AStar::Generator::Generator()
{// A*アルゴリズムの主要な部分を管理
    // 斜め移動を無効
    setDiagonalMovement(false);

    // ヒューリスティックをマンハッタン距離に設定
    setHeuristic(&Heuristic::manhattan);

    // 移動方向のベクトルのリスト。上下左右と斜め
    direction =
    {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
    };
}

//===============================================================
// グリッドサイズ
//===============================================================
void AStar::Generator::setWorldSize(Vec2i worldSize_)
{
    // 探索対象のグリッドサイズ
    worldSize = worldSize_;
}

//===============================================================
// 斜め移動の許可を設定し、方向数を変更（4方向または8方向）
//===============================================================
void AStar::Generator::setDiagonalMovement(bool enable_)
{
    // 移動方向数
    directions = (enable_ ? 8 : 4);
}

//===============================================================
// 使用するヒューリスティック関数を指定
//===============================================================
void AStar::Generator::setHeuristic(HeuristicFunction heuristic_)
{
    // ヒューリスティック関数
    heuristic = std::bind(heuristic_, _1, _2);
}

//===============================================================
// 指定した座標に障害物を追加
//===============================================================
void AStar::Generator::addCollision(Vec2i coordinates_)
{
    // 障害物格納リスト
    walls.push_back(coordinates_);
}

//===============================================================
// 指定した障害物を削除
//===============================================================
void AStar::Generator::removeCollision(Vec2i coordinates_)
{
    auto it = std::find(walls.begin(), walls.end(), coordinates_);

    if (it != walls.end())
    {// 特定の要素削除
        walls.erase(it);
    }
}

//===============================================================
// すべての障害物を削除
//===============================================================
void AStar::Generator::clearCollisions()
{
    // 全削除
    walls.clear();
}

//===============================================================
// 指定したスタートとゴールの間の経路を探索
//===============================================================
AStar::CoordinateList AStar::Generator::findPath(Vec2i source_, Vec2i target_)
{
    Node *current = nullptr;
    NodeSet openSet;        // 探索対象のノード
    NodeSet closedSet;      // 探索済みのノード
    openSet.reserve(100);
    closedSet.reserve(100);
    openSet.push_back(new Node(source_));

    // openSetから最低評価値（getScore()が最小）のノードを選び、探索
    while (!openSet.empty())
    {// 選択したノードがゴールであればループを抜け、経路を確定
        auto current_it = openSet.begin();
        current = *current_it;

        for (auto it = openSet.begin(); it != openSet.end(); it++)
        {
            auto node = *it;

            if (node->getScore() <= current->getScore())
            {
                current = node;
                current_it = it;
            }
        }

        if (current->coordinates == target_)
        {
            break;
        }

        closedSet.push_back(current);
        openSet.erase(current_it);

        // 隣接ノードの探索
        for (uint i = 0; i < directions; ++i)
        {// 方向リストに基づいて隣接ノードを探索し、障害物や閉集合にあるノードをスキップ
            Vec2i newCoordinates(current->coordinates + direction[i]);

            // コストを計算して、新しいノードをopenSetに追加
            if (detectCollision(newCoordinates) ||
                findNodeOnList(closedSet, newCoordinates))
            {
                continue;
            }

            uint totalCost = current->G + ((i < 4) ? 10 : 14);

            Node *successor = findNodeOnList(openSet, newCoordinates);

            if (successor == nullptr)
            {
                successor = new Node(newCoordinates, current);
                successor->G = totalCost;
                successor->H = heuristic(successor->coordinates, target_);
                openSet.push_back(successor);
            }
            else if (totalCost < successor->G)
            {
                successor->parent = current;
                successor->G = totalCost;
            }
        }
    }

    CoordinateList path;

    while (current != nullptr)
    {
        path.push_back(current->coordinates);
        current = current->parent;
    }

    releaseNodes(openSet);
    releaseNodes(closedSet);

    return path;
}

//===============================================================
// サポート関数
//===============================================================
AStar::Node* AStar::Generator::findNodeOnList(NodeSet& nodes_, Vec2i coordinates_)
{// 指定座標のノードをリスト内から探し、見つかったノードのポインタを返す
    for (auto node : nodes_)
    {
        if (node->coordinates == coordinates_) {
            return node;
        }
    }

    return nullptr;
}

//===============================================================
// サポート関数
//===============================================================
void AStar::Generator::releaseNodes(NodeSet& nodes_)
{// リスト内のノードをメモリから解放
    for (auto it = nodes_.begin(); it != nodes_.end();)
    {
        delete *it;
        it = nodes_.erase(it);
    }
}

//===============================================================
// 衝突検出
//===============================================================
bool AStar::Generator::detectCollision(Vec2i coordinates_)
{
    if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
        coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
        std::find(walls.begin(), walls.end(), coordinates_) != walls.end())
    {// 座標がグリッドの外にあるか障害物リストに含まれる場合
        return true;
    }

    return false;
}

//===============================================================
// 2つの座標の差（xとyの差）を返す
//===============================================================
AStar::Vec2i AStar::Heuristic::getDelta(Vec2i source_, Vec2i target_)
{
    return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
}

//===============================================================
// マンハッタン距離
//===============================================================
AStar::uint AStar::Heuristic::manhattan(Vec2i source_, Vec2i target_)
{// source_からtarget_までの推定コストを返す
    auto delta = std::move(getDelta(source_, target_));

    // 上下左右の移動のみを考慮した距離
    return static_cast<uint>(10 * (delta.x + delta.y));
}

//===============================================================
// ユークリッド距離
//===============================================================
AStar::uint AStar::Heuristic::euclidean(Vec2i source_, Vec2i target_)
{// source_からtarget_までの推定コストを返す
    auto delta = std::move(getDelta(source_, target_));

    // 直線距離を基にした距離
    return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

//===============================================================
// オクタゴナル距離
//===============================================================
AStar::uint AStar::Heuristic::octagonal(Vec2i source_, Vec2i target_)
{// source_からtarget_までの推定コストを返す
    auto delta = std::move(getDelta(source_, target_));

    // 斜め移動を考慮した距離
    return 10 * (delta.x + delta.y) + (-6) * min(delta.x, delta.y);
}
