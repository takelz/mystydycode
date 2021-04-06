/**
 * @brief デバッグメニュー
 * @author ryota.ssk
 * @detail メニューの階層について
 *	- 階層構造は'/'を用いて表現する
 *		例) "Debug/Game/Enemy/IsShow"
 *		- Debug > Game > Enemyの順の階層を表す
 *		- 最後のIsShowは設定値を示す
 */
#ifndef __DEBUG_MENU_H__
#define __DEBUG_MENU_H__

#include <DirectX/WinFont.h>
#include <vector>

//----------
// マクロ定義
//----------
#define DbgMenu DebugMenu::Instance()

/**
 * @brief デバッグメニュー
 */
class DebugMenu
{
	//----------
	// 構造体、列挙型定義
	//----------
public:
	//! @brief コールバック関数ポインタ
	typedef void(*CallBack)(void *arg);

private:
	struct DataSet;
	typedef std::vector<std::string> StringList;
	typedef std::vector<DataSet> MenuList;

	//! @brief データの種類
	enum DataType {
		DT_BOOL,
		DT_INT,
		DT_FLOAT,
		DT_TABLE,
		DT_CALLBACK,
		DT_MENU
	};
	//! @brief 最小値、最大値つきデータ
	template <typename T>
	struct ValueData {
		T value;	///< 値
		T min;		///< 最小
		T max;		///< 最大
	};
	//! @brief テーブルデータ
	struct TableData {
		int index;			///< 選択中のデータ
		int start;			///< メニュー表示開始位置
		StringList list;	///< 文字リスト
	};
	//! @brief コールバック情報
	struct CallBackData {
		CallBack func;	///< 関数ポインタ
		void *arg;		///< 引数
	};
	//！ @brief メニュールート情報
	struct MenuData {
		int index;			///< 選択中のメニュー
		int start;			///< 表示開始位置
		MenuData *parent;	///< 親メニュー
		MenuList list;		///< メニュー一覧
	};
	//! @brief メニュー割り当てデータ
	union Data {
		bool bValue;			///< bool
		ValueData<int> iData;	///< int
		ValueData<float> fData;	///< float
		TableData *pTable;		///< テーブル
		CallBackData callback;	///< コールバック
		MenuData *pMenu;		///< メニュー
	};
	//! @brief メニュー項目情報
	struct DataSet {
		std::string name;	///< 表示名
		DataType dType;		///< 種類
		Data uData;			///< データ
	};


	//----------
	// 関数定義
	//----------
public:
	// 
	static DebugMenu *Instance();
	static void Create();
	static void Delete();

public:
	DebugMenu();
	~DebugMenu();

	void Update();
	void Draw();


	//! @name デバッグメニューへ登録
	/// @{
	/**
	 * @param[in] name 登録名
	 * @param[in] value 値
	 */
	void EntryBool(const char *name, bool value = false);
	/**
	 * @param[in] name 登録名
	 * @param[in] value 値
	 * @param[in] max 最大値
	 * @param[in] min 最小値
	 */
	void EntryInt(const char *name, int value = 0, int max = 0, int min = 0);
	/**
	 * @param[in] name 登録名
	 * @param[in] value 値
	 * @param[in] max 最大値
	 * @param[in] min 最小値
	 */
	void EntryFloat(const char *name, float value = 0, float max = 0, float min = 0);
	/**
	 * @param[in] name 登録名
	 * @param[in] value テーブルリスト
	 * @param[in] num テーブル数
	 * @param[in] index 初期選択位置
	 */
	void EntryTable(const char *name, const char *value[], int num, int index = 0);
	/**
	* @param[in] name 登録名
	* @param[in] func 関数ポインタ
	* @param[in] arg コールバック引数
	*/
	void EntryCallBack(const char *name, CallBack func, void *arg = NULL);
	/// @}


	//! @brief メニュー表示状態
	bool IsOpenMenu();
	//! @brief 指定されたメニューが登録されているか
	bool IsFindName(const char *name);

	//! @name 登録データ取得
	/// @{
	bool IsBool(const char *name);
	int GetInt(const char *name);
	float GetFloat(const char *name);
	//! @return 選択中のテーブルインデックス,見つからなければ-1
	int GetTableIndex(const char *name);
	//! @return 選択中のテーブル名,見つからなければ""
	const char *GetTableName(const char *name);
	/// @}

private:
	//! @brief 再帰削除
	void RecursionDelete(MenuData *menu);
	//! @brief テーブル更新
	void UpdateTable();
	//! @brief メニュー更新
	void UpdateMenu();
	//! @brief テーブル描画
	void DrawTable();
	//! @brief メニュー描画
	void DrawMenu();
	//! @brief メニュー作成
	DataSet *CreateMenu(const char *name);
	//! @brief パス取得
	StringList GetMenuPath(const char *name);
	//! @brief データ探索
	DataSet *FindData(const char *name);

	//----- メンバ変数
private:
	static DebugMenu *m_pInstance;
private:
	MenuData	m_menu;			///< ルートメニュー
	MenuData	*m_pCurrent;	///< 現在のメニュー
	TableData	*m_pTable;		///< テーブル選択中
	int			m_tempTable;	///< 決定前のテーブルインデックス
	WinFont		*m_pFont;		///< 表示フォント
	bool		m_isOpen;		///< メニュー処理中か
	int			m_inputCount;	///< 連続入力カウント
};

#endif // __DEBUG_MENU_H__