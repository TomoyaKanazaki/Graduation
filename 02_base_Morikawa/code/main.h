//========================================
//
// ���C��[main.h]
// Author�F�X��x��
//
//========================================
#ifndef _MAIN_H_
#define _MAIN_H_

//========================================
//���C�u�����̃����N
//========================================
#pragma comment(lib,"d3d9.lib")     //�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")    //[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")   //DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")    //�V�X�e�������擾�ɕK�v

//========================================
//�}�N����`
//========================================
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//========================================
// �萔��`
//========================================
namespace
{
const int SCREEN_WIDTH = 1280;	// �E�B���h�E�̕�
const int SCREEN_HEIGHT = 720;	// �E�B���h�E�̍���
const int MAX_PARTS = 15;	// //�v���C���[���f��(�p�[�c)�̍ő吔
}

//========================================
//���_���[2D]�̍\���̂��`
//========================================
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

//=======================================
//���_���[3D]�̍\���̂��`
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_3D;

#endif