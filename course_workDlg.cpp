
// course_workDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "course_work.h"
#include "course_workDlg.h"
#include "afxdialogex.h"

#define PI 4*atan(1)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma once
//#include <atlbase.h>
//#include "F:\Autodesk\Inventor 2022\Bin\RxInventor.tlb"
#import <D:\Autodesk\Inventor 2022\Bin\RxInventor.tlb> \
rename_namespace("InventorNative") \
named_guids raw_dispinterfaces \
high_method_prefix("Method") \
rename("DeleteFile", "APIDeleteFile") \
rename("CopyFile", "APICopyFile") \
rename("MoveFile", "APIMoveFile") \
rename("SetEnvironmentVariable", "APISetEnvironmentVariable") \
rename("GetObject", "APIGetObject") \
exclude("_FILETIME", "IStream", "ISequentialStream", \
"_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", \
"IEnumUnknown", "IPersistFile", "IPersist", "IPictureDisp")

using namespace InventorNative;

CComPtr<Application> pInvApp; //приложение

PartDocumentPtr pPartDoc;  //документ
PartComponentDefinition* pPartComDef;//компоненты детали
TransientGeometry* pTransGeom; //геометрия детали
Transaction* pTrans; //операции

TransactionManagerPtr pTransManager; //менеджер операций


PlanarSketches* sketches; // эскизы
PartFeatures* ft; //элементы детали 

WorkPlanes* wp; //рабочие плоскости
WorkAxes* wax;//рабочие оси
WorkPoints* wpt;//рабочие точки

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CcourseworkDlg



CcourseworkDlg::CcourseworkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COURSE_WORK_DIALOG, pParent)
	, m_variableA(17)
	, m_variableB(27)
	, m_variableC(13)
	, m_variableD(10)
	, m_variableE(35)
	, m_nZ(8)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcourseworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_A, m_variableA);
	DDX_Text(pDX, IDC_EDIT_B, m_variableB);
	DDX_Text(pDX, IDC_EDIT_C, m_variableC);
	DDX_Text(pDX, IDC_EDIT_D, m_variableD);
	DDX_Text(pDX, IDC_EDIT_E, m_variableE);
	DDX_Text(pDX, IDC_EDIT_TEETH, m_nZ);
}

BEGIN_MESSAGE_MAP(CcourseworkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CcourseworkDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_EDGE, &CcourseworkDlg::OnBnClickedButtonEdge)
	ON_BN_CLICKED(IDC_BUTTON_FACE, &CcourseworkDlg::OnBnClickedButtonFace)
END_MESSAGE_MAP()


// Обработчики сообщений CcourseworkDlg

BOOL CcourseworkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CcourseworkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CcourseworkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CcourseworkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CcourseworkDlg::CheckData()
{
	if (!UpdateData())
		return false;

	m_variableA /= 10;
	m_variableB /= 10;
	m_variableC /= 10;
	m_variableD /= 10;
	m_variableE /= 10;
	return true;
}

void CcourseworkDlg::OnBnClickedButton1()
{
	BeginWaitCursor();

	if (!CheckData())
		return;

	if (!pInvApp)
	{
		// Get hold of the program id of Inventor.
		CLSID InvAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Inventor.Application", &InvAppClsid);
		if (FAILED(hRes)) {
			pInvApp = nullptr;
			return;// ReturnAndShowCOMError(hRes, L"ConnectToInventor,CLSIDFromProgID failed");
		}

		CComPtr<IUnknown> pInvAppUnk = nullptr;
		hRes = ::GetActiveObject(InvAppClsid, NULL, &pInvAppUnk);
		if (FAILED(hRes)) {
			// Inventor is not already running, so try to start it...
			TRACE(L"Could not get hold of an active Inventor, will start a new session\n");
			hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pInvAppUnk);
			if (FAILED(hRes)) {
				pInvApp = nullptr;
				return;
			}
		}
		hRes = pInvAppUnk->QueryInterface(__uuidof(Application), (void**)&pInvApp);
		if (FAILED(hRes)) {
			return;
		}
	}
	pInvApp->put_Visible(TRUE);

	pPartDoc = pInvApp->Documents->MethodAdd(kPartDocumentObject, pInvApp->FileManager->MethodGetTemplateFile(kPartDocumentObject, kMetricSystemOfMeasure, kGOST_DraftingStandard), true);

	pPartDoc->DisplayName = _T("Конфигуратор");

	pPartDoc->get_ComponentDefinition(&pPartComDef);

	pInvApp->get_TransientGeometry(&pTransGeom);


	pTransManager = pInvApp->GetTransactionManager();

	Document* Doc = CComQIPtr <Document>(pPartDoc);

	pTransManager->raw_StartTransaction(Doc, _T("Конфигуратор"), &pTrans);

	pPartComDef->get_Sketches(&sketches);



	pPartComDef->get_WorkPlanes(&wp);
	pPartComDef->get_Features(&ft);
	pPartComDef->get_WorkAxes(&wax);
	pPartComDef->get_WorkPoints(&wpt);

	//1 эскиз

	PlanarSketch* pSketch;
	sketches->raw_Add(wp->GetItem(3), false, &pSketch);

	SketchPoints* skPoints;
	SketchLines* skLines;
	Profiles* skProfiles;

	pSketch->get_SketchPoints(&skPoints);
	pSketch->get_SketchLines(&skLines);
	pSketch->get_Profiles(&skProfiles);

	SketchPointPtr point[4];
	SketchLinePtr lines[4];

	point[0] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(0, 0), false);
	point[1] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(0, m_variableE / 2.f), false);
	point[2] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(m_variableD, m_variableE / 2.f), false);
	point[3] = skPoints->MethodAdd(pTransGeom->MethodCreatePoint2d(m_variableD, 0), false);

	lines[0] = skLines->MethodAddByTwoPoints(point[0], point[1]);
	lines[1] = skLines->MethodAddByTwoPoints(point[1], point[2]);
	lines[2] = skLines->MethodAddByTwoPoints(point[2], point[3]);
	lines[3] = skLines->MethodAddByTwoPoints(point[3], point[0]);
	
	Profile* pProfile;
	

	try
	{
		pProfile = skProfiles->MethodAddForSolid(true);
	}
	catch (...)
	{
		AfxMessageBox(L"Ошибочный контур!");
		return;
	}
	
	skProfiles->raw__AddForSolid(&pProfile);

	RevolveFeatures* ftRevolve;
	ft->get_RevolveFeatures(&ftRevolve);

	RevolveFeaturePtr revolve = ftRevolve->MethodAddFull(pProfile, wax->GetItem(1), kJoinOperation);
	
	//создание новой плоскости 4
	wp->MethodAddByPlaneAndOffset(wp->GetItem(1), m_variableD, false);
	wp->GetItem(4)->Visible = false;

	//создание эскиза зуба
	PlanarSketch* pSketch2;
	sketches->raw_Add(wp->GetItem(4), false, &pSketch2);

	SketchPointPtr point2[3];
	SketchLinePtr lines2[2];
	SketchArcPtr arcs2[1];

	SketchPoints* skPoints2;
	SketchLines* skLines2;
	SketchArcs* skArcs2;

	Profiles* skProfiles2;

	pSketch2->get_SketchPoints(&skPoints2);
	pSketch2->get_SketchLines(&skLines2);
	pSketch2->get_SketchArcs(&skArcs2);
	pSketch2->get_Profiles(&skProfiles2);

	point2[0] = skPoints2->MethodAdd(pTransGeom->MethodCreatePoint2d(0, m_variableE / 2.f - 0.2115), false);
	point2[1] = skPoints2->MethodAdd(pTransGeom->MethodCreatePoint2d(-0.2, m_variableE / 2.f), false);
	point2[2] = skPoints2->MethodAdd(pTransGeom->MethodCreatePoint2d(0.2, m_variableE / 2.f), false);
	
	lines2[0] = skLines2->MethodAddByTwoPoints(point2[0], point2[1]);
	lines2[1] = skLines2->MethodAddByTwoPoints(point2[2], point2[0]);

	arcs2[0] = skArcs2->MethodAddByCenterStartEndPoint(point2[0], point2[1], point2[2], false);

	Profile* pProfile2;
	
	try
	{
		pProfile = skProfiles->MethodAddForSolid(true);
	}
	catch (...)
	{
		AfxMessageBox(L"Ошибочный контур!");
		return;
	}


	skProfiles2->raw__AddForSolid(&pProfile2);

	//вырез

	ExtrudeFeatures* ftExtrude;
	ft->get_ExtrudeFeatures(&ftExtrude);

	ExtrudeFeaturePtr extrude2 = ftExtrude->MethodAddByThroughAllExtent(pProfile2, kNegativeExtentDirection, kCutOperation);

	//массив вырезов

	ObjectCollection* pCollection;

	pInvApp->TransientObjects->raw_CreateObjectCollection(vtMissing, &pCollection);
	
	pCollection->MethodAdd(extrude2);

	CircularPatternFeatures* pCircPatFeat;
	
	ft->get_CircularPatternFeatures(&pCircPatFeat);

	CircularPatternFeature* circFeat = pCircPatFeat->MethodAdd(pCollection, wax->GetItem(1), true, m_nZ, "360 град", true, kIdenticalCompute);
	pCollection->MethodClear();

	////оболочка 42 грань

	//ShellFeatures* ftShell;
	//ft->get_ShellFeatures(&ftShell);

	//SurfaceBodyPtr SurfBody;
	//SurfaceBodiesPtr SurfBodies;
	//pPartComDef->get_SurfaceBodies(&SurfBodies);

	//SurfBodies->get_Item(1,&SurfBody);

	//Face* face;
	//Faces* faces;

	//SurfBody->get_Faces(&faces);
	//faces->get_Item(42, &face);

	//ShellDefinitionPtr shptr;
	//ShellFeaturesPtr shells;
	//ft->get_ShellFeatures(&shells);

	//FaceCollectionPtr facecoll;
	//pInvApp->TransientObjects->raw_CreateFaceCollection(vtMissing, &facecoll);
	//facecoll->MethodAdd(face);

	//shptr = shells->MethodCreateShellDefinition(vtMissing, 0.1, kInsideShellDirection);

	//shptr->put_InputFaces(facecoll);
	//shptr->put_Direction(kInsideShellDirection);
	//shptr = shells->MethodAdd(shptr);

	//создание эскиза нижней окружности(большая которая)

	//PlanarSketch* pSketch3;
	//sketches->raw_Add(wp->GetItem(1), false, &pSketch3);

	//SketchCirclePtr circ3;
	//SketchCircles* skCircles3;

	//Profiles* skProfiles3;

	//pSketch3->get_Profiles(&skProfiles3);
	//pSketch3->get_SketchCircles(&skCircles3);


	//skCircles3->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(0, 0), m_variableA / 2.f);

	//Profile* pProfile3;

	//skProfiles3->raw__AddForSolid(&pProfile3);

	//try
	//{
	//	pProfile = skProfiles->MethodAddForSolid(true);
	//}
	//catch (...)
	//{
	//	AfxMessageBox(L"Ошибочный контур!");
	//	return;
	//}

	////выдавливание

	//ExtrudeFeatures* ftExtrude3;
	//ft->get_ExtrudeFeatures(&ftExtrude3);

	//ExtrudeFeaturePtr extrude3 = ftExtrude->MethodAddByDistanceExtent(pProfile3, 0.7, kPositiveExtentDirection, kJoinOperation);

	////создание новой плоскости 5

	//wp->MethodAddByPlaneAndOffset(wp->GetItem(1), 0.7, false);
	//wp->GetItem(5)->Visible = false;

	////создание эскиза для выдавливание диаметра С

	//PlanarSketch* pSketch4;
	//sketches->raw_Add(wp->GetItem(5), false, &pSketch4);

	//SketchCirclePtr circ4;
	//SketchCircles* skCircles4;

	//Profiles* skProfiles4;

	//pSketch4->get_Profiles(&skProfiles4);
	//pSketch4->get_SketchCircles(&skCircles4);


	//skCircles4->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(0, 0), m_variableC / 2.f);

	//Profile* pProfile4;

	//skProfiles4->raw__AddForSolid(&pProfile4);

	//try
	//{
	//	pProfile = skProfiles->MethodAddForSolid(true);
	//}
	//catch (...)
	//{
	//	AfxMessageBox(L"Ошибочный контур!");
	//	return;
	//}

	////выдавливание

	//ExtrudeFeatures* ftExtrude4;
	//ft->get_ExtrudeFeatures(&ftExtrude4);

	//ExtrudeFeaturePtr extrude4 = ftExtrude->MethodAddByDistanceExtent(pProfile4, 1.5, kPositiveExtentDirection, kJoinOperation);

	////отверстие

	////создание плоскости 6
	//wp->MethodAddByPlaneAndOffset(wp->GetItem(1), 2.2, false);
	//wp->GetItem(6)->Visible = false;

	////создание эскиза

	//PlanarSketch* pSketch5;
	//sketches->raw_Add(wp->GetItem(6), false, &pSketch5);

	//SketchCirclePtr circ5;
	//SketchCircles* skCircles5;

	//Profiles* skProfiles5;

	//pSketch5->get_Profiles(&skProfiles5);
	//pSketch5->get_SketchCircles(&skCircles5);


	//skCircles5->MethodAddByCenterRadius(pTransGeom->MethodCreatePoint2d(0, 0), m_variableC / 2.f - 0.1);

	//Profile* pProfile5;

	//skProfiles5->raw__AddForSolid(&pProfile5);

	//try
	//{
	//	pProfile = skProfiles->MethodAddForSolid(true);
	//}
	//catch (...)
	//{
	//	AfxMessageBox(L"Ошибочный контур!");
	//	return;
	//}


	////выдавливание

	//ExtrudeFeatures* ftExtrude5;
	//ft->get_ExtrudeFeatures(&ftExtrude5);

	//ExtrudeFeaturePtr extrude5 = ftExtrude->MethodAddByDistanceExtent(pProfile5, 1.9, kNegativeExtentDirection, kCutOperation);

	////резьба 3 грань
	//
	//ThreadFeaturesPtr pThreadFt;
	//ft->get_ThreadFeatures(&pThreadFt);

	//facecoll->MethodClear();
	//pInvApp->TransientObjects->raw_CreateFaceCollection(vtMissing,&facecoll);

	//pPartComDef->get_SurfaceBodies(&SurfBodies);

	//SurfBodies->get_Item(1, &SurfBody);

	//SurfBody->get_Faces(&faces);
	//faces->get_Item(3, &face);

	//ThreadInfoPtr ftThread;
	//StandardThreadInfoPtr Thread;

	//ftThread = pThreadFt->MethodCreateStandardThreadInfo(false, true, "ANSI метрическая", "M14x2", "6g");


	//ThreadFeaturePtr thread = pThreadFt->MethodAdd(face, NULL, ftThread, false, true, NULL, NULL);

}


void CcourseworkDlg::OnBnClickedButtonEdge()
{
	// TODO: добавьте свой код обработчика уведомлений
	SelectSetPtr pSelect;
	pPartDoc->get_SelectSet(&pSelect);


	if (pSelect->GetCount() > 0)
	{
		EdgePtr Seekedge = pSelect->GetItem(1);

		int seeknumber = -1;
		for (int i = 1; i <= pPartComDef->SurfaceBodies->GetCount(); i++)
		{
			SurfaceBodyPtr SurfBody;
			SurfaceBodiesPtr SurfBodies;

			pPartComDef->get_SurfaceBodies(&SurfBodies);

			SurfBodies->get_Item(i, &SurfBody);


			EdgePtr edge;
			EdgesPtr edges;

			SurfBody->get_Edges(&edges);

			int N = SurfBody->Edges->GetCount();
			for (int j = 1; j <= SurfBody->Edges->GetCount(); j++)
			{
				edges->get_Item(j, &edge);

				if (Seekedge == edge)
				{
					seeknumber = j;
					CString str;
					str.Format(L"%i", j);
					MessageBox(str);

				}
			}
		}
	}

}


void CcourseworkDlg::OnBnClickedButtonFace()
{
	// TODO: добавьте свой код обработчика уведомлений
	SelectSetPtr pSelect;
	pPartDoc->get_SelectSet(&pSelect);


	if (pSelect->GetCount() > 0)
	{
		FacePtr Seekface = pSelect->GetItem(1);

		int seeknumber = -1;
		for (int i = 1; i <= pPartComDef->SurfaceBodies->GetCount(); i++)
		{
			SurfaceBodyPtr SurfBody;
			SurfaceBodiesPtr SurfBodies;

			pPartComDef->get_SurfaceBodies(&SurfBodies);

			SurfBodies->get_Item(i, &SurfBody);

			FacePtr face;
			FacesPtr faces;

			SurfBody->get_Faces(&faces);

			int N = SurfBody->Faces->GetCount();
			for (int j = 1; j <= SurfBody->Faces->GetCount(); j++)
			{
				faces->get_Item(j, &face);

				if (Seekface == face)
				{
					seeknumber = j;
					CString str;
					str.Format(L"%i", j);
					MessageBox(str);

				}
			}
		}
	}
}
