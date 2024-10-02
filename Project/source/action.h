////============================================
////
////	�A�N�V�����̏��� [action.h]
////	Author:���� �쏟
////
////============================================
//
//#ifndef _ACTION_H_
//#define _ACTION_H_
//
//class CEnemy;
//
//// �A�N�V�����̊��N���X
//class CAction
//{
//public:
//
//	// �A�N�V�����̎��
//	enum TYPE
//	{
//		TYPE_NONE = 0,
//		TYPE_SEARCH,
//		TYPE_BATTLE,
//		TYPE_MAX
//	};
//
//	CAction();
//	~CAction();
//
//	virtual void Uninit(void) = 0;
//	virtual void Update(void) = 0;
//
//private:
//
//	TYPE m_type;
//};
//
//// �T���A�N�V�����̊��N���X
//class CActionSearch : public CAction
//{
//public:
//
//	CActionSearch();
//	~CActionSearch();
//
//	static CActionSearch* Create();
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//
//private:
//
//};
//
//// �퓬�A�N�V�����̊��N���X
//class CActionBattle : public CAction
//{
//public:
//
//	CActionBattle();
//	~CActionBattle();
//
//	static CActionBattle* Create();
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//
//	void Tracking(D3DXVECTOR3* pos, D3DXVECTOR3* move, D3DXVECTOR3* rot, D3DXVECTOR3* rotDest);
//
//	void CollisionEnemy(D3DXVECTOR3* pos);
//
//private:
//
//	CEnemy* m_pTgtEnemy;		// �^�[�Q�b�g�̓G�|�C���^
//
//};
//
//#endif