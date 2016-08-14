
// FaceRecgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FaceRecg.h"
#include "FaceRecgDlg.h"
#include "afxdialogex.h"


#include "Cvvimage.h"
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CascadeClassifier face_cascade;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFaceRecgDlg dialog



CFaceRecgDlg::CFaceRecgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFaceRecgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFaceRecgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFaceRecgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON1, &CFaceRecgDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFaceRecgDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CFaceRecgDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CFaceRecgDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CFaceRecgDlg message handlers

BOOL CFaceRecgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFaceRecgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFaceRecgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDialog::OnPaint();
		CDialog::UpdateWindow();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFaceRecgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFaceRecgDlg::OnBnClickedButton1()
{
	// Read the image file
	Mat image = imread("face.jpg", CV_LOAD_IMAGE_COLOR);;
	// Load Face cascade (.xml file)
	CascadeClassifier face_cascade;
	face_cascade.load("haarcascade_frontalface_alt2.xml");

	// Detect faces
	std::vector<Rect> faces;
	std::vector<Rect> eyes;
	face_cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	// Draw circles on the detected faces
	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(image, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}

	imshow("Detected Face", image);

	waitKey(0);
}


void CFaceRecgDlg::OnBnClickedButton2()
{
	VideoCapture cap(0); // Open default camera
	Mat frame;

	// Load Face cascade (.xml file)
	
	face_cascade.load("haarcascade_frontalface_alt2.xml");

	while (cap.read(frame)) {
		detectFaces(frame); // Call function to detect faces
		if (waitKey(30) >= 0)    // pause
			break;
	}
	
}
void CFaceRecgDlg::detectFaces(Mat frame) {

	std::vector<Rect> faces;
	Mat frame_gray;

	// Convert to gray scale
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

	// Equalize histogram
	equalizeHist(frame_gray, frame_gray);

	// Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3,
		0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Iterate over all of the faces
	for (size_t i = 0; i < faces.size(); i++) {

		// Find center of faces
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);

		// Draw ellipse around face
		ellipse(frame, center, Size(faces[i].width *0.8, faces[i].height *0.8),
			0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

	}

	// Display frame
	imshow("Face Detection", frame);
}


void CFaceRecgDlg::OnBnClickedButton4()
{
	// Read the image file
	Mat image = imread("face.jpg", CV_LOAD_IMAGE_COLOR);;
	// Load Face cascade (.xml file)
	CascadeClassifier face_cascade;
	CascadeClassifier eye_cascade;
	face_cascade.load("haarcascade_frontalface_alt2.xml");
	eye_cascade.load("haarcascade_eye.xml");

	// Detect faces
	std::vector<Rect> faces;
	std::vector<Rect> eyes;
	face_cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	eye_cascade.detectMultiScale(image, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));
	// Draw circles on the detected faces
	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(image, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}
	// Draw circles on the detected faces
	for (int i = 0; i < eyes.size(); i++)
	{
		Point center(eyes[i].x + eyes[i].width*0.5, eyes[i].y + eyes[i].height*0.5);
		ellipse(image, center, Size(eyes[i].width*0.5, eyes[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}

	imshow("Detected Face", image);

	waitKey(0);
}


void CFaceRecgDlg::OnBnClickedButton3()
{
	// Read the image file
	Mat image = imread("face.jpg", CV_LOAD_IMAGE_COLOR);;
	// Load Face cascade (.xml file)
	CascadeClassifier face_cascade;
	face_cascade.load("haarcascade_frontalface_alt2.xml");

	// Detect faces
	std::vector<Rect> faces;
	std::vector<Rect> eyes;
	face_cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	// Draw circles on the detected faces
	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(image, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}

	imshow("Detected Face", image);

	waitKey(0);
}
