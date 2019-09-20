
// ChatDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Client.h"

// CChatDlg �Ի���
class CMainDlg : public CDialogEx
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINDLG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCtrl;
	CDialogEx *m_tabWnds[2] = { 0 };
	CString m_curChatValue;

	/*
	*	tab�Ӵ�����ʾ
	*/
	void showTabWnd(int index);
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	/*
	*	��Ӻ��ѻ�Ⱥ�İ�ť
	*/
	afx_msg void OnBnClickedAddbtn();
	/*
	*	3�����������Ϣ�ĺ���
	*/

	/*
	*	�ں����б�����ʾ����
	*	���������ݰ��ṹ��
	*/
	void addFriend(DataPack *pDataPack);
	
	/*
	*	ͬ��/�ܾ���������
	*	���������ݰ��ṹ��
	*/
	void agreeFriend(DataPack *pDataPack);
	
	/*
	*	���պ�����Ϣ
	*	���������ݰ��ṹ��
	*/
	void recvFriendData(DataPack *pDataPack);
//////////////////////////////////////////////////////////////////////
	/*
	*	3������Ⱥ����Ϣ�ĺ���
	*/
	/*
	*	ͬ��/�ܾ�����Ⱥ��
	*	���������ݰ��ṹ��
	*/
	void agreeJoinGroup(DataPack * pDataPack);

	///*
	//*	Ⱥ���б�����ʾȺ��
	//*	���������ݰ��ṹ��
	//*/
	//void addGroup(DataPack * pDataPack);

	/*
	*	����Ⱥ����Ϣ
	*	���������ݰ��ṹ��
	*/
	void recvGroupData(DataPack *pDataPack);

	
};

/*
*	��������Ϣ�����߳�
*/
DWORD WINAPI chatRecvProc(LPVOID arg);



