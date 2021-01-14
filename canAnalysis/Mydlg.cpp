// Mydlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "canAnalysis.h"
#include "Mydlg.h"
#include "afxdialogex.h"


// Mydlg �Ի���

IMPLEMENT_DYNAMIC(Mydlg, CDialogEx)

Mydlg::Mydlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Mydlg::~Mydlg()
{
}

void Mydlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Mydlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &Mydlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Mydlg ��Ϣ�������


void Mydlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog openDlg (TRUE, _T("431��������"), NULL,

		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,

		_T("Chart Files (*.txt)|*.bin|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||"), this);

	if (openDlg.DoModal() == IDOK)
	{
		m_Txt = openDlg.GetPathName();
	}

	HandleFile(m_Txt);



	CDialogEx::OnOK();
}



int Mydlg::HandleFile(CString FilePath)
{
	CStdioFile file;
	CStdioFile OutFile;

	CString ToolLine;
	file.Open(m_Txt, CFile::modeRead,NULL);

	OutFile.Open(_T("E:\\LaunchToyota\\����������_ԭʼ\\MorSetDB\\Out.txt"),
		CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);


	while (file.ReadString(ToolLine))
	{
		//tool ���� ����
		CString strToolTmp;
		AfxExtractSubString(strToolTmp, ToolLine, 4, ' ');
		int ToolSid = strtoul(strToolTmp,NULL, 16);

		AfxExtractSubString(strToolTmp, ToolLine, 5, ' ');
		int ToolSid2 = strtoul(strToolTmp, NULL, 16);
		
		//ECU �ص��������
		CString EcuAnsLine;
		if (!file.ReadString(EcuAnsLine))
		{
			printf("������");
			return 1;
		}

		CString strEcuTmp;
		AfxExtractSubString(strEcuTmp, EcuAnsLine, 4, ' ');
		int ECUSid = strtoul(strEcuTmp, NULL, 16);

		AfxExtractSubString(strEcuTmp, EcuAnsLine, 5, ' ');
		int ECUSid2 = strtoul(strEcuTmp, NULL, 16);

		if (ECUSid == ToolSid+0x40) //ֻ�����˷�һ֡��һ֡�����
		{
			//�����ķ�һ֡��һ֡ 
			ToolLine = "Req: " + ToolLine + "\n";
			EcuAnsLine = "Ans: " + EcuAnsLine + "\n\n";

			OutFile.WriteString(ToolLine);
			OutFile.WriteString(EcuAnsLine);
		}
		else if (ECUSid2 == ToolSid2 + 0x40)
		{
			//�����ķ�һ֡��һ֡ 
			ToolLine = "Req: " + ToolLine + "\n";
			EcuAnsLine = "Ans: " + EcuAnsLine + "\n\n";

			OutFile.WriteString(ToolLine);
			OutFile.WriteString(EcuAnsLine);
		}
		else
			continue;

	} 

	file.Close();
	OutFile.Close();
	return 0;
}