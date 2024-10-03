//==============================================================
//
//�V�[������[scene.h]
//Author:����������
//
//==============================================================
#ifndef _SCENE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SCENE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�O���錾
class CTitle;			//�^�C�g�����
class CTutorial;		//�`���[�g���A�����
class CGame;			//�Q�[�����
class CResult;			//���U���g���
class CRanking;			//�����L���O���

//�V�[���N���X�̒�`
class CScene : public CObject
{
public:

	//�Q�[�����[�h
	enum MODE
	{
		MODE_TITLE = 0,		//�^�C�g�����
		MODE_TUTORIAL,		//�`���[�g���A�����
		MODE_GAME,			//�Q�[�����
		MODE_RESULT,		//���U���g���
		MODE_RANKING,		//�����L���O���
		MODE_MAX
	};

	CScene();
	CScene(MODE mode);
	~CScene();

	static CScene *Create(MODE mode);		//��������

	virtual HRESULT Init(void);		//����������
	virtual void Uninit(void);			//�I������
	virtual void Update(void);			//�X�V����
	virtual void Draw(void);			//�`�揈��

	MODE GetMode(void) { return m_mode; }			//���[�h�擾
	void SetMode(MODE mode) { m_mode = mode; }		//���[�h�ݒ�

	static CGame *GetGame(void) { return m_pGame; }		//�Q�[����ʎ擾
	static CRanking *GetRanking(void) { return m_pRanking; }		//�����L���O��ʎ擾

private:
	MODE m_mode;			//�Q�[�����[�h

	static CTitle *m_pTitle;			//�^�C�g����ʂ̏��
	static CTutorial *m_pTutorial;		//�`���[�g���A�����
	static CGame *m_pGame;				//�Q�[����ʂ̏��
	static CResult *m_pResult;			//���U���g��ʂ̏��
	static CRanking *m_pRanking;		//�����L���O��ʂ̏��
};


#endif