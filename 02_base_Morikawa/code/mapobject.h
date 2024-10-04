//========================================
//
// �}�b�v�I�u�W�F�N�g[mapobject.h]
// Author�F�X��x��
//
//========================================
#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

#include "main.h"
#include "model.h"

//========================================
// �}�b�v�I�u�W�F�N�g�N���X
//========================================
class CMapObject : public CObject
{
public:
	CMapObject(int nPriority = 7);		//�R���X�g���N�^
	~CMapObject();	//�f�X�g���N�^

	//�����o�֐�
	static CMapObject* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	//�����o�ϐ�
	CModel* m_pModel;		// ���f���̃|�C���^
};

#endif
