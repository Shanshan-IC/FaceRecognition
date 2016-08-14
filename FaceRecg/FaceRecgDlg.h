
// FaceRecgDlg.h : header file
//

#pragma once
#include "Cvvimage.h"
#include "opencv\cv.h"
#include "opencv\highgui.h"

using namespace std;
using namespace cv;

// CFaceRecgDlg dialog
class CFaceRecgDlg : public CDialogEx
{
// Construction
public:
	CFaceRecgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FACERECG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	//void ShowImage(IplImage* img, UINT ID);
	//void ResizeImage(IplImage* img);
	//void detectAndDisplay(Mat frame);
	IplImage* TheImage;
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void detectFaces(Mat frame);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
};
