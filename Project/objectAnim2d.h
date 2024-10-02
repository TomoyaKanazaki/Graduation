//============================================
//
//	�I�u�W�F�N�g�A�j���[�V����2D [objectAnim2d.h]
//	Author:�O�엳�V��
//
//============================================
#ifndef _OBJECTANIM2D_H_
#define _OBJECTANIM2D_H_

#include "main.h"
#include "object.h"
#include "object2d.h"
//�I�u�W�F�N�g2D�N���X
class CObjectAnim2D : public CObject2D
{
public:
	CObjectAnim2D(int nPriority);
	~CObjectAnim2D();

	static CObjectAnim2D* Create(D3DXVECTOR3 pos,int nU,int nV,int nAnim,bool bLoop,float fRate,int nPriority = 6);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetU(int n) { m_nU = n; }
	int GetU() { return m_nU; }
	void SetV(int n) { m_nV = n; }
	int GetV() { return m_nV; }
	void SetMaxAnim(int n) { m_nAnim = n; }
	int GetMaxAnim() { return m_nAnim; }
	void SetCurrent(int n) { m_nCurrent = n; }
	int GetCurrent() { return m_nCurrent; }
	void SetFramerate(float rate) { m_fFramerate = rate; }
	float GetFramerate() { return m_fFramerate; }
	void SetLoop(bool b) { m_bLoop = b; }
	bool GetLoop() { return m_bLoop; }
	void SetReverse(bool b) { m_bReverse = b; }
	bool GetReverse() { return m_bReverse; }
private:
	int m_nU;//�������̖���
	int m_nV;//�c�����̖���
	int m_nAnim;//�A�j���[�V�����̍ő喇��
	int m_nCurrent;//���݂̖���
	float m_fFramerate; //�X�V���x
	float m_fCurrentFrame;
	bool m_bLoop;//���[�v�̗L��
	bool m_bReverse; //�t�Đ��̗L��
};
#endif