//============================================
//
//	エディット [Edit.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "object.h"

//前方宣言
class CObjmeshCube;
class CMapModel;
class CStair;
class CGimmick;

//マップクラス
class CEdit
{
public:
//-------------------------------------------------------------------------
// オブジェクト一覧
//-------------------------------------------------------------------------
	enum OBJ_HIT
	{
		OBJ_HIT_DARUMA = 0,			// だるま
		OBJ_HIT_KOUSI,				// 格子
		OBJ_HIT_TORO,				// 灯籠
		OBJ_HIT_WOOD,				// 木
		OBJ_HIT_ANDON,				// 行灯
		OBJ_HIT_ARMOR,				// 鎧立て
		OBJ_HIT_BARREL,				// 樽
		OBJ_HIT_BUCKET,				// 風呂桶
		OBJ_HIT_CASTLE,				// 城
		OBJ_HIT_SCREEN,				// 屏風
		OBJ_HIT_GATE,				// 門
		OBJ_HIT_CANDLE,				// 手燭
		OBJ_HIT_CUSHION,			// 座布団
		OBJ_HIT_KAKEJIKUTURU,		// 掛け軸（鶴）
		OBJ_HIT_KAKEJIKUKAME,		// 掛け軸（亀）
		OBJ_HIT_KETTLE,				// やかん
		OBJ_HIT_TOWER,				// 見張り台の砦
		OBJ_HIT_SWORDRACK,			// 刀掛け
		OBJ_HIT_SWORDRACKWALL,		// 刀壁掛け
		OBJ_HIT_VASETHIN,			// 細い壺
		OBJ_HIT_WELL,				// 井戸
		OBJ_HIT_VASEBIG,			// 大きい壺
		OBJ_HIT_PILLAR,				// 柱
		OBJ_HIT_WOODDESK,			// 机
		OBJ_HIT_WOOD_MANY_BG,		// たくさんの木
		OBJ_HIT_GLASSDESK,			// ガラスの机
		OBJ_HIT_GLASSDESK_WEAPON,	// ガラスの机の中の武器
		OBJ_HIT_TATAMI_FLOOR,		// 小上がり畳
		OBJ_HIT_RICEBALES,			// 米俵
		OBJ_HIT_BOSSFENCE,			// ボス用フェンス
		OBJ_HIT_CHEST,				// 棚
		OBJ_HIT_HEARTH,				// 囲炉裏
		OBJ_HIT_FUSUMA00,			// 襖（１枚）
		OBJ_HIT_FUSUMA01,			// 襖（４枚）4（端）
		OBJ_HIT_FUSUMA02,			// 襖（４枚）2（中）
		OBJ_HIT_FUSUMA03,			// 襖（４枚）1（端）
		OBJ_HIT_FUSUMA04,			// 襖（４枚）3（中）
		OBJ_HIT_KAKEJIKUONI,		// 掛け軸（鬼）
		OBJ_HIT_KAKEJIKUYAMA,		// 掛け軸（山）
		OBJ_HIT_TAIMATU,			// 松明
		OBJ_HIT_ONNI_KAMEN,			// 鬼の仮面
		OBJ_HIT_FOUNDATION,			// 土台
		OBJ_HIT_SCAFFOLDING,		// 足場
		OBJ_HIT_SUPPORTPOST,		// 支え柱
		OBJ_HIT_MAX					// この列挙型の総数
	};

	enum BLOCK_INIT_TYPE
	{
		BLOCK_INIT_TYPE_NORMAL = 0,	// 通常の初期型
		BLOCK_INIT_TYPE_WALL_Z,		// Z壁の初期型
		BLOCK_INIT_TYPE_WALL_X,		// X壁の初期型
		BLOCK_INIT_TYPE_MAX			// この列挙型の総数
	};

	enum BLOCK_TEXTURE
	{
		BLOCK_TEXTURE_00 = 0,		// 通常の初期型
		BLOCK_TEXTURE_01,			// 
		BLOCK_TEXTURE_02,			// 
		BLOCK_TEXTURE_03,			// 
		BLOCK_TEXTURE_MAX			// この列挙型の総数
	};

	struct OBJINFO
	{
		char* pFilename;	// ファイル名
		char* pCommentname;	// コメント名
	};

	CEdit();
	~CEdit();

	enum EDITTYPE
	{
		EDITTYPE_BLOCK = 0,
		EDITTYPE_MODELHIT,
		EDITTYPE_MODELNOT,
		EDITTYPE_ENEMY,
		EDITTYPE_MAX,
	};

	static CEdit* Create();
	void Uninit(void);
	void Update(void);

protected:

private:
	void Move(void);			//移動処理
	void CollisionBlock(void);	//ブロックとの当たり判定
	void CollisionXModel(void);	//Xモデルとの当たり判定
	void SaveData(void);		//エディット情報の保存
	void SaveBlock(void);		//ブロックの保存
	void SaveXModel(void);		//Xモデルの保存
	void UpdateBlock(void);		//ブロックの更新
	void UpdateXModel(void);	//Xモデルの更新
	void AppearCollision(void);	//当たり判定の可視化

	int m_nPressCount;			//ボタン入力カウント
	int m_nBlockTextureIdx;		//ブロックのテクスチャ番号
	D3DXVECTOR3 m_EditPos;		//設置位置
	D3DXVECTOR3 m_EditRot;		//設置角度
	D3DXVECTOR3 m_EditSize;		//設置サイズ
	float MoveWidth;			//移動幅
	float m_fRotValue;			//回転幅
	EDITTYPE m_EditType;		//エディットで配置するオブジェクトの種類
	CObjmeshCube* m_pEditBlock;	//ブロックのポインタ
	CMapModel* m_pEditModel;	//マップモデルのポインタ
	
	int m_nModelNumber;			//移動幅
	int m_nGimmickNumber;		//ギミック番号
	int m_nBlockNumber;			//ブロック番号
	bool m_bModelCollision;		//当たり判定を付与するかどうか
	bool m_bAppearCollision;	//当たり判定を可視化するかどうか

	static OBJINFO m_aModelInfo[OBJ_HIT_MAX];	// テクスチャの情報
};
#endif