// Mydlg.cpp : 实现文件
//

#include "stdafx.h"
#include "canAnalysis.h"
#include "Mydlg.h"
#include "afxdialogex.h"


// Mydlg 对话框

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


// Mydlg 消息处理程序


void Mydlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog openDlg (TRUE, _T("431采数数据"), NULL,

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

	OutFile.Open(_T("E:\\LaunchToyota\\卡罗拉数据_原始\\MorSetDB\\Out.txt"),
		CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);


	while (file.ReadString(ToolLine))
	{
		//tool 命令 解析
		CString strToolTmp;
		AfxExtractSubString(strToolTmp, ToolLine, 4, ' ');
		int ToolSid = strtoul(strToolTmp,NULL, 16);

		AfxExtractSubString(strToolTmp, ToolLine, 5, ' ');
		int ToolSid2 = strtoul(strToolTmp, NULL, 16);
		
		//ECU 回的命令解析
		CString EcuAnsLine;
		if (!file.ReadString(EcuAnsLine))
		{
			printf("读完了");
			return 1;
		}

		CString strEcuTmp;
		AfxExtractSubString(strEcuTmp, EcuAnsLine, 4, ' ');
		int ECUSid = strtoul(strEcuTmp, NULL, 16);

		AfxExtractSubString(strEcuTmp, EcuAnsLine, 5, ' ');
		int ECUSid2 = strtoul(strEcuTmp, NULL, 16);

		if (ECUSid == ToolSid+0x40) //只处理了发一帧收一帧的情况
		{
			//正常的发一帧回一帧 
			ToolLine = "Req: " + ToolLine + "\n";
			EcuAnsLine = "Ans: " + EcuAnsLine + "\n\n";

			OutFile.WriteString(ToolLine);
			OutFile.WriteString(EcuAnsLine);
		}
		else if (ECUSid2 == ToolSid2 + 0x40)
		{
			//正常的发一帧回一帧 
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