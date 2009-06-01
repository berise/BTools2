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
	/*
    if(UTE_SUCCESS != error && UTE_ERROR != error) 
        cout << endl << CUT_ERR::GetErrorString(error) << endl;
		*/
    return error;
}


/// user pressed stop button.
BOOL BTPing::IsAborted()
{
	return !m_pWnd->m_bStartThread;
}
