//============================================
//
//	�ړ���Ԃ̊Ǘ����� [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"
#include "enemy.h"

//============================================
// �O���錾
//============================================
class CObjectCharacter;         // �L�����N�^�[���
class CItem;                    // �A�C�e�����
class CObjectX;                 // �I�u�W�F�N�gX�̏��

//============================================
// �ړ���Ԃ̃C���^�[�t�F�[�X�N���X
//============================================
class CMoveState
{
public:

    // �f�o�b�O�p�ړ����
    enum STATE
    {
        STATE_NONE = 0,     // �����Ȃ�
        STATE_CONTROL,      // ����
        STATE_ASTAR,        // �ǐ�
        STATE_RANDOM,       // �����_��
        STATE_STOP,         // ��~
        STATE_ROLL,         // �]����

        STATE_MAX
    };

    // �ړ�����
    enum ROTSTATE
    {
        ROTSTATE_LEFT = 0,	// ������
        ROTSTATE_RIGHT,		// �E����
        ROTSTATE_UP,		// �����
        ROTSTATE_DOWN,		// ������
        ROTSTATE_MAX,		// �ő�
        ROTSTATE_WAIT,	    // �ҋ@
        ROTSTATE_NONE
    };

    // �ړ��̐i�s�󋵂��Ǘ�����\����
    struct PROGGRESS
    {
        bool bOKL;		//���ւ̐i�s��������邩�ǂ���
        bool bOKR;		//�E�ւ̐i�s��������邩�ǂ���
        bool bOKU;		//��ւ̐i�s��������邩�ǂ���
        bool bOKD;		//���ւ̐i�s��������邩�ǂ���
    };

    CMoveState();
    virtual ~CMoveState() {}

    virtual void Init() {}         // ������
    virtual void Release() {}      // �j��

    // ����
    virtual void ControlRandom(CObjectCharacter* pCharacter) {}        // ����ƃ����_���؂�ւ�
    virtual void ControlAStar(CObjectCharacter* pCharacter) {}         // ����ƒǐՐ؂�ւ�
    virtual void ControlStop(CObjectCharacter* pCharacter) {}          // ����ƒ�~�؂�ւ�

    // �����_�����s
    virtual void RandomAStar(CObjectCharacter* pCharacter) {}           // �����_���ƒǐՐ؂�ւ�
    virtual void RandomStop(CObjectCharacter* pCharacter) {}            // �����_���ƒ�~�؂�ւ�

    // �ǐ�
    virtual void AStarStop(CObjectCharacter* pCharacter) {}             // �ǐՂƒ�~�؂�ւ�
    virtual void RollStop(CObjectX* pObjectX) {}                        // �]����ƒ�~�؂�ւ�

    // �ړ�����
    virtual void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) {}           // �ړ�����(�L�����N�^�[)
    virtual void Move(CItem* pItem, D3DXVECTOR3& pos, D3DXVECTOR3& rot) {}                           // �ړ�����(�A�C�e��)
    virtual void Move(CObjectX* pObjectX, D3DXVECTOR3& pos, D3DXVECTOR3& rot) {}                     // �ړ�����(�I�u�W�F�N�gX)


    CMoveState* GetMoveState(CObjectCharacter* pCharacter);             // �ړ���Ԃ̏��擾

    // �ݒ�E�擾
    virtual void SetRotState(ROTSTATE RotState) {}      // �ړ������̏�Ԃ��擾
    virtual ROTSTATE GetRotState() { return ROTSTATE_NONE; }                       // �ړ������̏�Ԃ��擾

    virtual void SetEnemyType(CEnemy::ENEMY_TYPE Type) {}             // �G�̎�ސݒ�

    void Debug(void);       // �f�o�b�O�\���p

protected:

    // �����o�֐�
    void Rot(CObjectCharacter* pCharacter, D3DXVECTOR3& rotMy);				// �ړ���������(�L�����N�^�[)
    void Rot(CItem* pItem, D3DXVECTOR3& rotMy);						        // �ړ���������(�A�C�e��)

    void UpdatePos(CObjectCharacter* pCharacter, D3DXVECTOR3& pos);	        // �ʒu�X�V����(�L�����N�^�[)
    void UpdatePos(CItem* pItem, D3DXVECTOR3& pos);	                        // �ʒu�X�V����(�A�C�e��)


    // �����o�ϐ�
    STATE m_State;                  // �f�o�b�O�p���
    PROGGRESS m_Progress;			// �ړ��̐i�s����
    PROGGRESS m_ProgressOld;		// �O��̈ړ��̐i�s����

   
};

//============================================
// ����o������
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl();
    ~CStateControl(){}

    void Release() override;      // �j��

    // �؂�ւ�����
    void ControlRandom(CObjectCharacter* pCharacter) override;      // �����_���ɐ؂�ւ�
    void ControlAStar(CObjectCharacter* pCharacter) override;       // �ǐՂɐ؂�ւ�
    void ControlStop(CObjectCharacter* pCharacter) override;        // ��~�ɐ؂�ւ�

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // �L�����N�^�[�̈ړ�����

    // �ݒ�E�擾
    void SetRotState(ROTSTATE RotState) override { m_RotState = RotState; }      // �ړ������̏�Ԃ��擾
    ROTSTATE GetRotState() override { return m_RotState; }                       // �ړ������̏�Ԃ��擾

private:

    // �����o�֐�
    D3DXVECTOR3 InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move, float fSpeed);		//�ړ����̓L�[�{�[�h
    D3DXVECTOR3 MoveInputPadStick(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3 Move, float fSpeed);

    void UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove);        // �v���C���[�̈ړ��X�V����
    void UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove);         // �G�̈ړ��X�V����

    // �����o�ϐ�
    bool m_bInput;				//���͂��s�������ǂ���
    ROTSTATE m_RotState;        // �ړ������̏��

};

//============================================
// �����_�����s���
//============================================
class CStateRandom : public CMoveState
{
public:

    CStateRandom();
    ~CStateRandom() {}

    void Release() override;      // �j��

    // �؂�ւ�����
    void ControlRandom(CObjectCharacter* pCharacter) override;      // ����ɐ؂�ւ�
    void RandomAStar(CObjectCharacter* pCharacter) override;        // �ǐՂɐ؂�ւ�
    void RandomStop(CObjectCharacter* pCharacter) override;         // ��~�ɐ؂�ւ�

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // �L�����N�^�[�̈ړ�����
    void Move(CItem* pItem, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;                      // �ړ�����(�A�C�e��)

private:

    // �����o�֐�
    void SearchWall(CObjectCharacter* pCharacter, D3DXVECTOR3& pos);				// �ǂ̃T�[�`����(�L�����N�^�[)
    void SearchWall(CItem* pItem, D3DXVECTOR3& pos);				                // �ǂ̃T�[�`����(�A�C�e��)

    void MoveSelect(CObjectCharacter* pCharacter);		                            // �ړ������̑I��(�L�����N�^�[)
    void MoveSelect(CItem* pItem);		                                            // �ړ������̑I��(�A�C�e��)

    void MoveAngle(D3DXVECTOR3& moveSave, std::vector<D3DXVECTOR3>& move, std::vector<ROTSTATE>& rotState);      // �e�����̈ړ��ʐݒ�

    // �����o�ϐ�
    CMapSystem::GRID m_GridOld;
    ROTSTATE m_RotState;            // �ړ������̏��
    int m_nSelectCounter;           // �ړ������ύX�J�E���^�[
    bool m_bSwitchMove;             // �ړ�������ς��邩

};

//============================================
// �ǐՏ��
//============================================
class CStateAStar : public CMoveState
{
public:
    CStateAStar();
    ~CStateAStar() {}

    void Init() override;         // ������
    void Release() override;      // �j��

    // �؂�ւ�����
    void ControlAStar(CObjectCharacter* pCharacter) override;       // ����ɐ؂�ւ�
    void RandomAStar(CObjectCharacter* pCharacter) override;        // �����_�����s�ɐ؂�ւ�
    void AStarStop(CObjectCharacter* pCharacter) override;          // ��~�ɐ؂�ւ�

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // �L�����N�^�[�̈ړ�����

    // �ݒ�
    void SetEnemyType(CEnemy::ENEMY_TYPE Type) { m_EnemyType = Type; }             // �G�̎�ސݒ�

private:

    // �����o�֐�
    void Coordinate(CObjectCharacter* pCharacter); // �ŒZ�o�H�T��
    void Route(CObjectCharacter* pCharacter);	// �ŒZ�o�H�����ǂ�

    // �����o�ϐ�
    CMapSystem::GRID* m_pPath;          // �v���C���[�ւ̍ŒZ�o�H
    CEnemy::ENEMY_TYPE m_EnemyType;		//�G�̎��

    int m_nNumCoordinate;       // �ŒZ�o�H�̗v�f��
    int m_nTargetIndex;         // ���Ɍ������ׂ��O���b�h�̃C���f�b�N�X
    float m_fCoordinateTimer;   // �o�H�T���Ԋu

    bool m_bStateFlag;          // �X�e�[�g�t���O
};

//============================================
// ��~���
//============================================
class CStateStop : public CMoveState
{
public:

    CStateStop(){ m_State = STATE_STOP; }
    ~CStateStop() {}

    void Release() override;      // �j��

    // �؂�ւ�����
    void ControlStop(CObjectCharacter* pCharacter) override;        // ����ɐ؂�ւ�
    void RandomStop(CObjectCharacter* pCharacter) override;         // �����_�����s�ɐ؂�ւ�
    void AStarStop(CObjectCharacter* pCharacter) override;          // �ǐՂɐ؂�ւ�
    void RollStop(CObjectX* pObjectX) override;                     // �]����ƒ�~�؂�ւ�

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // �L�����N�^�[�̈ړ�����
};

//============================================
// �]������
//============================================
class CStateRoll : public CMoveState
{
public:

    CStateRoll();
    ~CStateRoll() {}

    void Release() override;      // �j��

    // �؂�ւ�����
    void RollStop(CObjectX* pObjectX) override;                     // �]����ƒ�~�؂�ւ�

    void Move(CObjectX* pObjectX, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // �L�����N�^�[�̈ړ�����

private:
    void SetJudg(int& nGridPosX, int& nGridPosZ, bool& bProgress);                  // �]���邩�m�F
    void RailCheck(CMapSystem::GRID& grid, bool& bProgress);                        // ���[�������邩�m�F

    void Stop(D3DXVECTOR3& pos, D3DXVECTOR3& GridPos, D3DXVECTOR3& move);           // �]����Ȃ��Ƃ��̈ʒu
};

#endif