/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/

//============================================
//
//	AStar処理 [AStar.h]
//	Author:外部ライブラリ
//
//============================================

#ifndef __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__
#define __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__

#include <vector>
#include <functional>
#include <set>

//===============================================================
// 定数定義
//===============================================================
namespace AStar
{
    struct Vec2i
    {// 2次元座標の構造体
        int x, y;

        // 座標が等しいか
        bool operator == (const Vec2i& coordinates_);

        friend Vec2i operator + (const AStar::Vec2i& left_, const AStar::Vec2i& right_)
        {
            // 2つの座標を足し合わせる。移動方向に基づいて新しい座標を得る
            return{ left_.x + right_.x, left_.y + right_.y };
        }
    };

    using uint = unsigned int;      //unsigned int型のエイリアスです。
    using HeuristicFunction = std::function<uint(Vec2i, Vec2i)>;    // uint型の値を返す
    using CoordinateList = std::vector<Vec2i>;      // Vec2i型の座標のリスト（std::vector<Vec2i>）として定義

    struct Node
    {
        uint G;  // スタートからノードまでの移動コスト
        uint H;  // ゴールまでの推定コスト（ヒューリスティック値）

        Vec2i coordinates;  // ノードの座標
        Node *parent;       // 親ノードへのポインタ。経路を再構築するために使用

        Node(Vec2i coord_, Node *parent_ = nullptr);    // ノード座標と親ノードの設定
        uint getScore();    // ノードの評価値（G + H）を返す
    };

    using NodeSet = std::vector<Node*>;

    class Generator
    {
        bool detectCollision(Vec2i coordinates_);   // 指定した座標に障害物があるか、またはグリッド外かをチェック
        Node* findNodeOnList(NodeSet& nodes_, Vec2i coordinates_);  // 指定した座標に対応するノードがリスト内にあるかを探す
        void releaseNodes(NodeSet& nodes_);     // ノードリストのメモリを解放

    public:
        Generator();
        void setWorldSize(Vec2i worldSize_);        // 探索するグリッド（ワールド）のサイズを設定
        void setDiagonalMovement(bool enable_);     // 斜め移動の許可を設定。trueだと8方向(斜め含む)に移動可能
        void setHeuristic(HeuristicFunction heuristic_);        // ヒューリスティック関数を設定
        CoordinateList findPath(Vec2i source_, Vec2i target_);  // 指定したスタート座標からゴール座標への最短経路を求める
        void addCollision(Vec2i coordinates_);      // 指定した座標に障害物を追加
        void removeCollision(Vec2i coordinates_);   // 指定した座標の障害物を削除
        void clearCollisions();     // すべての障害物を削除

    private:
        HeuristicFunction heuristic;        // ヒューリスティック関数
        CoordinateList direction;   // 移動方向を格納するリスト（通常は上下左右、斜めも含む）
        CoordinateList walls;       // 障害物を格納するリスト
        Vec2i worldSize;    // 探索対象のグリッドのサイズ
        uint directions;    // 移動方向の数（4方向または8方向）
    };

    class Heuristic
    {
        static Vec2i getDelta(Vec2i source_, Vec2i target_);    // 2つの座標の差（x と y の差）を返す

    public:
        static uint manhattan(Vec2i source_, Vec2i target_);    // マンハッタン距離を計算。上下左右の移動のみを考慮した距離
        static uint euclidean(Vec2i source_, Vec2i target_);    // ユークリッド距離を計算。直線距離を基にした距離
        static uint octagonal(Vec2i source_, Vec2i target_);    // オクタゴナル距離を計算。斜め移動を考慮した距離
    };
}

#endif // __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__
