/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/

//============================================
//
//	AStar���� [AStar.h]
//	Author:�O�����C�u����
//
//============================================

#ifndef __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__
#define __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__

#include <vector>
#include <functional>
#include <set>

//===============================================================
// �萔��`
//===============================================================
namespace AStar
{
    struct Vec2i
    {// 2�������W�̍\����
        int x, y;

        // ���W����������
        bool operator == (const Vec2i& coordinates_);

        friend Vec2i operator + (const AStar::Vec2i& left_, const AStar::Vec2i& right_)
        {
            // 2�̍��W�𑫂����킹��B�ړ������Ɋ�Â��ĐV�������W�𓾂�
            return{ left_.x + right_.x, left_.y + right_.y };
        }
    };

    using uint = unsigned int;      //unsigned int�^�̃G�C���A�X�ł��B
    using HeuristicFunction = std::function<uint(Vec2i, Vec2i)>;    // uint�^�̒l��Ԃ�
    using CoordinateList = std::vector<Vec2i>;      // Vec2i�^�̍��W�̃��X�g�istd::vector<Vec2i>�j�Ƃ��Ē�`

    struct Node
    {
        uint G;  // �X�^�[�g����m�[�h�܂ł̈ړ��R�X�g
        uint H;  // �S�[���܂ł̐���R�X�g�i�q���[���X�e�B�b�N�l�j

        Vec2i coordinates;  // �m�[�h�̍��W
        Node *parent;       // �e�m�[�h�ւ̃|�C���^�B�o�H���č\�z���邽�߂Ɏg�p

        Node(Vec2i coord_, Node *parent_ = nullptr);    // �m�[�h���W�Ɛe�m�[�h�̐ݒ�
        uint getScore();    // �m�[�h�̕]���l�iG + H�j��Ԃ�
    };

    using NodeSet = std::vector<Node*>;

    class Generator
    {
        bool detectCollision(Vec2i coordinates_);   // �w�肵�����W�ɏ�Q�������邩�A�܂��̓O���b�h�O�����`�F�b�N
        Node* findNodeOnList(NodeSet& nodes_, Vec2i coordinates_);  // �w�肵�����W�ɑΉ�����m�[�h�����X�g���ɂ��邩��T��
        void releaseNodes(NodeSet& nodes_);     // �m�[�h���X�g�̃����������

    public:
        Generator();
        void setWorldSize(Vec2i worldSize_);        // �T������O���b�h�i���[���h�j�̃T�C�Y��ݒ�
        void setDiagonalMovement(bool enable_);     // �΂߈ړ��̋���ݒ�Btrue����8����(�΂ߊ܂�)�Ɉړ��\
        void setHeuristic(HeuristicFunction heuristic_);        // �q���[���X�e�B�b�N�֐���ݒ�
        CoordinateList findPath(Vec2i source_, Vec2i target_);  // �w�肵���X�^�[�g���W����S�[�����W�ւ̍ŒZ�o�H�����߂�
        void addCollision(Vec2i coordinates_);      // �w�肵�����W�ɏ�Q����ǉ�
        void removeCollision(Vec2i coordinates_);   // �w�肵�����W�̏�Q�����폜
        void clearCollisions();     // ���ׂĂ̏�Q�����폜

    private:
        HeuristicFunction heuristic;        // �q���[���X�e�B�b�N�֐�
        CoordinateList direction;   // �ړ��������i�[���郊�X�g�i�ʏ�͏㉺���E�A�΂߂��܂ށj
        CoordinateList walls;       // ��Q�����i�[���郊�X�g
        Vec2i worldSize;    // �T���Ώۂ̃O���b�h�̃T�C�Y
        uint directions;    // �ړ������̐��i4�����܂���8�����j
    };

    class Heuristic
    {
        static Vec2i getDelta(Vec2i source_, Vec2i target_);    // 2�̍��W�̍��ix �� y �̍��j��Ԃ�

    public:
        static uint manhattan(Vec2i source_, Vec2i target_);    // �}���n�b�^���������v�Z�B�㉺���E�̈ړ��݂̂��l����������
        static uint euclidean(Vec2i source_, Vec2i target_);    // ���[�N���b�h�������v�Z�B������������ɂ�������
        static uint octagonal(Vec2i source_, Vec2i target_);    // �I�N�^�S�i���������v�Z�B�΂߈ړ����l����������
    };
}

#endif // __ASTAR_HPP_8F637DB91972F6C878D41D63F7E7214F__
