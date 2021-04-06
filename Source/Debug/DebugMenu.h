/**
 * @brief �f�o�b�O���j���[
 * @author ryota.ssk
 * @detail ���j���[�̊K�w�ɂ���
 *	- �K�w�\����'/'��p���ĕ\������
 *		��) "Debug/Game/Enemy/IsShow"
 *		- Debug > Game > Enemy�̏��̊K�w��\��
 *		- �Ō��IsShow�͐ݒ�l������
 */
#ifndef __DEBUG_MENU_H__
#define __DEBUG_MENU_H__

#include <DirectX/WinFont.h>
#include <vector>

//----------
// �}�N����`
//----------
#define DbgMenu DebugMenu::Instance()

/**
 * @brief �f�o�b�O���j���[
 */
class DebugMenu
{
	//----------
	// �\���́A�񋓌^��`
	//----------
public:
	//! @brief �R�[���o�b�N�֐��|�C���^
	typedef void(*CallBack)(void *arg);

private:
	struct DataSet;
	typedef std::vector<std::string> StringList;
	typedef std::vector<DataSet> MenuList;

	//! @brief �f�[�^�̎��
	enum DataType {
		DT_BOOL,
		DT_INT,
		DT_FLOAT,
		DT_TABLE,
		DT_CALLBACK,
		DT_MENU
	};
	//! @brief �ŏ��l�A�ő�l���f�[�^
	template <typename T>
	struct ValueData {
		T value;	///< �l
		T min;		///< �ŏ�
		T max;		///< �ő�
	};
	//! @brief �e�[�u���f�[�^
	struct TableData {
		int index;			///< �I�𒆂̃f�[�^
		int start;			///< ���j���[�\���J�n�ʒu
		StringList list;	///< �������X�g
	};
	//! @brief �R�[���o�b�N���
	struct CallBackData {
		CallBack func;	///< �֐��|�C���^
		void *arg;		///< ����
	};
	//�I @brief ���j���[���[�g���
	struct MenuData {
		int index;			///< �I�𒆂̃��j���[
		int start;			///< �\���J�n�ʒu
		MenuData *parent;	///< �e���j���[
		MenuList list;		///< ���j���[�ꗗ
	};
	//! @brief ���j���[���蓖�ăf�[�^
	union Data {
		bool bValue;			///< bool
		ValueData<int> iData;	///< int
		ValueData<float> fData;	///< float
		TableData *pTable;		///< �e�[�u��
		CallBackData callback;	///< �R�[���o�b�N
		MenuData *pMenu;		///< ���j���[
	};
	//! @brief ���j���[���ڏ��
	struct DataSet {
		std::string name;	///< �\����
		DataType dType;		///< ���
		Data uData;			///< �f�[�^
	};


	//----------
	// �֐���`
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


	//! @name �f�o�b�O���j���[�֓o�^
	/// @{
	/**
	 * @param[in] name �o�^��
	 * @param[in] value �l
	 */
	void EntryBool(const char *name, bool value = false);
	/**
	 * @param[in] name �o�^��
	 * @param[in] value �l
	 * @param[in] max �ő�l
	 * @param[in] min �ŏ��l
	 */
	void EntryInt(const char *name, int value = 0, int max = 0, int min = 0);
	/**
	 * @param[in] name �o�^��
	 * @param[in] value �l
	 * @param[in] max �ő�l
	 * @param[in] min �ŏ��l
	 */
	void EntryFloat(const char *name, float value = 0, float max = 0, float min = 0);
	/**
	 * @param[in] name �o�^��
	 * @param[in] value �e�[�u�����X�g
	 * @param[in] num �e�[�u����
	 * @param[in] index �����I���ʒu
	 */
	void EntryTable(const char *name, const char *value[], int num, int index = 0);
	/**
	* @param[in] name �o�^��
	* @param[in] func �֐��|�C���^
	* @param[in] arg �R�[���o�b�N����
	*/
	void EntryCallBack(const char *name, CallBack func, void *arg = NULL);
	/// @}


	//! @brief ���j���[�\�����
	bool IsOpenMenu();
	//! @brief �w�肳�ꂽ���j���[���o�^����Ă��邩
	bool IsFindName(const char *name);

	//! @name �o�^�f�[�^�擾
	/// @{
	bool IsBool(const char *name);
	int GetInt(const char *name);
	float GetFloat(const char *name);
	//! @return �I�𒆂̃e�[�u���C���f�b�N�X,������Ȃ����-1
	int GetTableIndex(const char *name);
	//! @return �I�𒆂̃e�[�u����,������Ȃ����""
	const char *GetTableName(const char *name);
	/// @}

private:
	//! @brief �ċA�폜
	void RecursionDelete(MenuData *menu);
	//! @brief �e�[�u���X�V
	void UpdateTable();
	//! @brief ���j���[�X�V
	void UpdateMenu();
	//! @brief �e�[�u���`��
	void DrawTable();
	//! @brief ���j���[�`��
	void DrawMenu();
	//! @brief ���j���[�쐬
	DataSet *CreateMenu(const char *name);
	//! @brief �p�X�擾
	StringList GetMenuPath(const char *name);
	//! @brief �f�[�^�T��
	DataSet *FindData(const char *name);

	//----- �����o�ϐ�
private:
	static DebugMenu *m_pInstance;
private:
	MenuData	m_menu;			///< ���[�g���j���[
	MenuData	*m_pCurrent;	///< ���݂̃��j���[
	TableData	*m_pTable;		///< �e�[�u���I��
	int			m_tempTable;	///< ����O�̃e�[�u���C���f�b�N�X
	WinFont		*m_pFont;		///< �\���t�H���g
	bool		m_isOpen;		///< ���j���[��������
	int			m_inputCount;	///< �A�����̓J�E���g
};

#endif // __DEBUG_MENU_H__