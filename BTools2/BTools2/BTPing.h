#ifndef __BTPING_INCLUDED__
#define __BTPING_INCLUDED__

#include "ut_icmp.h"
#include "BTools2View.h"

class BTPing: public CUT_ICMP
{
public:
	BTPing(CPingPage *pWnd) :m_pWnd(pWnd) { }
	
	void OnReceiveICMP();
    
    // OnError is a member of the CUT_WSClient class, from which
    // CUT_ICMP inherits...
    int OnError(int error);

	BOOL IsAborted();

private:
	CPingPage *m_pWnd;
	BOOL m_bStop;
};

#endif