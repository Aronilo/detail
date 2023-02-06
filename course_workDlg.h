
// course_workDlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CcourseworkDlg
class CcourseworkDlg : public CDialogEx
{
// Создание
public:
	CcourseworkDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COURSE_WORK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	bool CheckData();
	double m_variableA;
	double m_variableB;
	double m_variableC;
	double m_variableD;
	double m_variableE;
	int m_nZ;
	afx_msg void OnBnClickedButtonEdge();
	afx_msg void OnBnClickedButtonFace();
};
