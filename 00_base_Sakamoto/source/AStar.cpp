//============================================
//
//	AStar���� [AStar.cpp]
//	Author:�O�����C�u����
//
//============================================

#include "AStar.h"
#include <algorithm>
#include <math.h>

//===============================================================
// �萔��`
//===============================================================
using namespace std::placeholders;      // �ꏊ�̕ێ�

//===============================================================
// 2��Vec2i�I�u�W�F�N�g���������W�������`�F�b�N
//===============================================================
bool AStar::Vec2i::operator == (const Vec2i& coordinates_)
{
    return (x == coordinates_.x && y == coordinates_.y);
}

//===============================================================
// �T���o�H�̃m�[�h
//===============================================================
AStar::Node::Node(Vec2i coordinates_, Node *parent_)
{
    // �m�[�h���W
    parent = parent_;

    // �e�m�[�h�ւ̃|�C���^�ŁA�o�H���t�ɂ��ǂ�Ƃ��Ɏg�p
    coordinates = coordinates_;

    // �X�^�[�g����m�[�h�܂ł̈ړ��R�X�g(G)�ƃq���[���X�e�B�b�N�l�i�S�[���܂ł̐���R�X�g�j(H)
    G = H = 0;
}

//===============================================================
// �ړ��R�X�g�Ɛ���R�X�g�̍��v�l
//===============================================================
AStar::uint AStar::Node::getScore()
{
    return G + H;
}

//===============================================================
// �o�H�𐶐�����N���X
//===============================================================
AStar::Generator::Generator()
{// A*�A���S���Y���̎�v�ȕ������Ǘ�
    // �΂߈ړ��𖳌�
    setDiagonalMovement(false);

    // �q���[���X�e�B�b�N���}���n�b�^�������ɐݒ�
    setHeuristic(&Heuristic::manhattan);

    // �ړ������̃x�N�g���̃��X�g�B�㉺���E�Ǝ΂�
    direction =
    {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
    };
}

//===============================================================
// �O���b�h�T�C�Y
//===============================================================
void AStar::Generator::setWorldSize(Vec2i worldSize_)
{
    // �T���Ώۂ̃O���b�h�T�C�Y
    worldSize = worldSize_;
}

//===============================================================
// �΂߈ړ��̋���ݒ肵�A��������ύX�i4�����܂���8�����j
//===============================================================
void AStar::Generator::setDiagonalMovement(bool enable_)
{
    // �ړ�������
    directions = (enable_ ? 8 : 4);
}

//===============================================================
// �g�p����q���[���X�e�B�b�N�֐����w��
//===============================================================
void AStar::Generator::setHeuristic(HeuristicFunction heuristic_)
{
    // �q���[���X�e�B�b�N�֐�
    heuristic = std::bind(heuristic_, _1, _2);
}

//===============================================================
// �w�肵�����W�ɏ�Q����ǉ�
//===============================================================
void AStar::Generator::addCollision(Vec2i coordinates_)
{
    // ��Q���i�[���X�g
    walls.push_back(coordinates_);
}

//===============================================================
// �w�肵����Q�����폜
//===============================================================
void AStar::Generator::removeCollision(Vec2i coordinates_)
{
    auto it = std::find(walls.begin(), walls.end(), coordinates_);

    if (it != walls.end())
    {// ����̗v�f�폜
        walls.erase(it);
    }
}

//===============================================================
// ���ׂĂ̏�Q�����폜
//===============================================================
void AStar::Generator::clearCollisions()
{
    // �S�폜
    walls.clear();
}

//===============================================================
// �w�肵���X�^�[�g�ƃS�[���̊Ԃ̌o�H��T��
//===============================================================
AStar::CoordinateList AStar::Generator::findPath(Vec2i source_, Vec2i target_)
{
    Node *current = nullptr;
    NodeSet openSet;        // �T���Ώۂ̃m�[�h
    NodeSet closedSet;      // �T���ς݂̃m�[�h
    openSet.reserve(100);
    closedSet.reserve(100);
    openSet.push_back(new Node(source_));

    // openSet����Œ�]���l�igetScore()���ŏ��j�̃m�[�h��I�сA�T��
    while (!openSet.empty())
    {// �I�������m�[�h���S�[���ł���΃��[�v�𔲂��A�o�H���m��
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

        // �אڃm�[�h�̒T��
        for (uint i = 0; i < directions; ++i)
        {// �������X�g�Ɋ�Â��ėאڃm�[�h��T�����A��Q����W���ɂ���m�[�h���X�L�b�v
            Vec2i newCoordinates(current->coordinates + direction[i]);

            // �R�X�g���v�Z���āA�V�����m�[�h��openSet�ɒǉ�
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
// �T�|�[�g�֐�
//===============================================================
AStar::Node* AStar::Generator::findNodeOnList(NodeSet& nodes_, Vec2i coordinates_)
{// �w����W�̃m�[�h�����X�g������T���A���������m�[�h�̃|�C���^��Ԃ�
    for (auto node : nodes_)
    {
        if (node->coordinates == coordinates_) {
            return node;
        }
    }

    return nullptr;
}

//===============================================================
// �T�|�[�g�֐�
//===============================================================
void AStar::Generator::releaseNodes(NodeSet& nodes_)
{// ���X�g���̃m�[�h��������������
    for (auto it = nodes_.begin(); it != nodes_.end();)
    {
        delete *it;
        it = nodes_.erase(it);
    }
}

//===============================================================
// �Փˌ��o
//===============================================================
bool AStar::Generator::detectCollision(Vec2i coordinates_)
{
    if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
        coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
        std::find(walls.begin(), walls.end(), coordinates_) != walls.end())
    {// ���W���O���b�h�̊O�ɂ��邩��Q�����X�g�Ɋ܂܂��ꍇ
        return true;
    }

    return false;
}

//===============================================================
// 2�̍��W�̍��ix��y�̍��j��Ԃ�
//===============================================================
AStar::Vec2i AStar::Heuristic::getDelta(Vec2i source_, Vec2i target_)
{
    return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
}

//===============================================================
// �}���n�b�^������
//===============================================================
AStar::uint AStar::Heuristic::manhattan(Vec2i source_, Vec2i target_)
{// source_����target_�܂ł̐���R�X�g��Ԃ�
    auto delta = std::move(getDelta(source_, target_));

    // �㉺���E�̈ړ��݂̂��l����������
    return static_cast<uint>(10 * (delta.x + delta.y));
}

//===============================================================
// ���[�N���b�h����
//===============================================================
AStar::uint AStar::Heuristic::euclidean(Vec2i source_, Vec2i target_)
{// source_����target_�܂ł̐���R�X�g��Ԃ�
    auto delta = std::move(getDelta(source_, target_));

    // ������������ɂ�������
    return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

//===============================================================
// �I�N�^�S�i������
//===============================================================
AStar::uint AStar::Heuristic::octagonal(Vec2i source_, Vec2i target_)
{// source_����target_�܂ł̐���R�X�g��Ԃ�
    auto delta = std::move(getDelta(source_, target_));

    // �΂߈ړ����l����������
    return 10 * (delta.x + delta.y) + (-6) * min(delta.x, delta.y);
}
