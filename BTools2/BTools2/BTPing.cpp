#include "stdafx.h"
#include "BTPing.h"

void BTPing::OnReceiveICMP()
{
	double nResponseDuration = GetResponseDuration();
	static double  minY = 0.0, maxY = 100.0;
	if(nResponseDuration < minY)
	{
		minY = nResponseDuration;
		m_pWnd->m_OScopeCtrl.SetRange(minY, maxY,  1) ;
	}

	if(nResponseDuration > maxY)
	{
		maxY = nResponseDuration;
		m_pWnd->m_OScopeCtrl.SetRange(minY, maxY,  1) ;
	}

	m_pWnd->m_OScopeCtrl.AppendPoint(nResponseDuration);

	TCHAR wszLog[256], wszHost[256];
	size_t hostlen;
	

	GetResponseAddress(wszHost, 256, &hostlen);
	wsprintf(wszLog, L"%d, %s, %d ms", 		
		GetResponseSequence(),
		wszHost, //GetResponseAddress(),
		GetResponseDuration());

	m_pWnd->Log(wszLog);


	
	/*
    cout << endl << GetResponseSequence() << ". " 
        << GetResponseDuration() << "ms" << " \t " 
        << GetResponseAddress() << "\t" 
        << GetResponseName() << endl 
        << "Type: " << GetResponseType() 
        << "  Code: " << GetResponseCode() << " "
        << GetResponseMessage() << endl;
    if(GetResponseFinished())
        cout << endl << "Data finshed." << endl;
		*/
}

// OnError is a member of the CUT_WSClient class, from which
// CUT_ICMP inherits...
int BTPing::OnError(int error)
{	
	//TCHAR wszLog[256];
	//wsprintf(wszLog, L"%s, CUT_ERR::GetErrorString(error)); m_pWnd->Log(wszLog);

    if(UTE_SUCCESS != error && UTE_ERROR != error)
	{
		TCHAR wszLog[256];
		wsprintf(wszLog, L"%s", 		
		CUT_ERR::GetErrorString(error));
		m_pWnd->Log(wszLog);
	}
	
        
		
    return error;
}


//
BOOL BTPing::IsAborted()
{
	return m_pWnd->m_bStartThread == 1 ? FALSE : TRUE;
}
