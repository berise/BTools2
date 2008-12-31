/*
Module : PING.H
Purpose: Interface for an MFC wrapper class to encapsulate PING
Created: PJN / 10-06-1998

Copyright (c) 1998 - 2006 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

/////////////////////////// Macros / Defines ////////////////////////

#ifndef __PING_H__
#define __PING_H__

#ifndef CPING_NO_WINSOCK2
  #ifndef _WINSOCK2API_
    #pragma message("To avoid this message place you need to include Winsock2.h in your PCH (normally stdafx.h)")
    #include <winsock2.h>
    #include <ws2tcpip.h>
  #endif
#else
  #ifndef _WINSOCKAPI_
    #pragma message("You need to include Winsock.h, Winsock2.h or AfxSock.h in your PCH (normally stdafx.h) to compile CPing")
    #include <winsock.h>
  #endif
#endif

#ifndef IP_EXPORT_INCLUDED
#pragma message("To avoid this message please put IPExport.h in your PCH (normally stdafx.h)")
#include <IPExport.h>
#endif

#ifndef CPING_EXT_CLASS
#define CPING_EXT_CLASS
#endif

#ifndef CPING_EXT_API
#define CPING_EXT_API
#endif


/*
enum ICMPStatus
{
    IP_STATUS_BASE           = 11000,
    IP_SUCCESS               = 0,
    IP_BUF_TOO_SMALL         = IP_STATUS_BASE + 1,
    IP_DEST_NET_UNREACHABLE  = IP_STATUS_BASE + 2,
    IP_DEST_HOST_UNREACHABLE = IP_STATUS_BASE + 3,
    IP_DEST_PROT_UNREACHABLE = IP_STATUS_BASE + 4,
    IP_DEST_PORT_UNREACHABLE = IP_STATUS_BASE + 5,
    IP_NO_RESOURCES          = IP_STATUS_BASE + 6,
    IP_BAD_OPTION            = IP_STATUS_BASE + 7,
    IP_HW_ERROR              = IP_STATUS_BASE + 8,
    IP_PACKET_TOO_BIG        = IP_STATUS_BASE + 9,
    IP_REQ_TIMED_OUT         = IP_STATUS_BASE + 10,
    IP_BAD_REQ               = IP_STATUS_BASE + 11,
    IP_BAD_ROUTE             = IP_STATUS_BASE + 12,
    IP_TTL_EXPIRED_TRANSIT   = IP_STATUS_BASE + 13,
    IP_TTL_EXPIRED_REASSEM   = IP_STATUS_BASE + 14,
    IP_PARAM_PROBLEM         = IP_STATUS_BASE + 15,
    IP_SOURCE_QUENCH         = IP_STATUS_BASE + 16,
    IP_OPTION_TOO_BIG        = IP_STATUS_BASE + 17,
    IP_BAD_DESTINATION       = IP_STATUS_BASE + 18,
    IP_ADDR_DELETED          = IP_STATUS_BASE + 19,
    IP_SPEC_MTU_CHANGE       = IP_STATUS_BASE + 20,
    IP_MTU_CHANGE            = IP_STATUS_BASE + 21,
    IP_UNLOAD                = IP_STATUS_BASE + 22,
    IP_GENERAL_FAILURE       = IP_STATUS_BASE + 50,
    MAX_IP_STATUS            = IP_GENERAL_FAILURE,
    IP_PENDING               = IP_STATUS_BASE + 255
};
This is defined in ipexport.h
*/

typedef struct
{
	int			id;
	char		status[256];
} IDSTRING;

//----------------------------------------------------------------------------
// ICMPSendEcho Error String
// The values in the status word returned in the ICMP Echo 
// Reply buffer after calling IcmpSendEcho() all have a
// base value of 11000 (IP_STATUS_BASE).  At times,
// when IcmpSendEcho() fails outright, GetLastError() will 
// subsequently return these error values also.
// Two Errors value defined in ms_icmp.h are missing from 
// this string table (just to simplify use of the table):
// "IP_GENERAL_FAILURE (11050)"
// "IP_PENDING (11255)"
//----------------------------------------------------------------------------
const IDSTRING ICMPStatus2String[] = 
{
    {IP_STATUS_BASE,           "IP_STATUS_BASE (11000)"},
    {IP_BUF_TOO_SMALL,         "IP_BUF_TOO_SMALL (11001)"},
    {IP_DEST_NET_UNREACHABLE,  "IP_DEST_NET_UNREACHABLE (11002)"}, 
    {IP_DEST_HOST_UNREACHABLE, "IP_DEST_HOST_UNREACHABLE (11003)"}, 
    {IP_DEST_PROT_UNREACHABLE, "IP_DEST_PROT_UNREACHABLE (11004)"}, 
    {IP_DEST_PORT_UNREACHABLE, "IP_DEST_PORT_UNREACHABLE (11005)"}, 
    {IP_NO_RESOURCES,          "IP_NO_RESOURCES (11006)"}, 
    {IP_BAD_OPTION,            "IP_BAD_OPTION (11007)"}, 
    {IP_HW_ERROR,              "IP_HW_ERROR (11008)"}, 
    {IP_PACKET_TOO_BIG,        "IP_PACKET_TOO_BIG (11009)"}, 
    {IP_REQ_TIMED_OUT,         "IP_REQ_TIMED_OUT (11010)"}, 
    {IP_BAD_REQ,               "IP_BAD_REQ (11011)"}, 
    {IP_BAD_ROUTE,             "IP_BAD_ROUTE (11012)"}, 
    {IP_TTL_EXPIRED_TRANSIT,   "IP_TTL_EXPIRED_TRANSIT (11013)"}, 
    {IP_TTL_EXPIRED_REASSEM,   "IP_TTL_EXPIRED_REASSEM (11014)"}, 
    {IP_PARAM_PROBLEM,         "IP_PARAM_PROBLEM (11015)"}, 
    {IP_SOURCE_QUENCH,         "IP_SOURCE_QUENCH (11016)"}, 
    {IP_OPTION_TOO_BIG,        "IP_OPTION_TOO_BIG (11017)"}, 
    {IP_BAD_DESTINATION,       "IP_BAD_DESTINATION (11018)"}, 
    {IP_ADDR_DELETED,          "IP_ADDR_DELETED (11019)"}, 
    {IP_SPEC_MTU_CHANGE,       "IP_SPEC_MTU_CHANGE (11020)"}, 
    {IP_MTU_CHANGE,            "IP_MTU_CHANGE (11021)"}, 
    {IP_UNLOAD,                "IP_UNLOAD (11022)"}, 
    {IP_GENERAL_FAILURE,       "IP_GENERAL_FAILURE"}, 
    {-1,                       NULL}
};



/////////////////////////// Classes /////////////////////////////////

struct CPING_EXT_CLASS CPingReply
{
//Constructors / Destructors
  CPingReply();

//Member variables
	in_addr	 Address;              //The IP address of the replier
	unsigned long RTT;             //Round Trip time in Milliseconds
  unsigned long EchoReplyStatus; //here will be status of the last ping if successful
};

class CPING_EXT_CLASS CPing
{
public:
//Constructors / Destructors
  CPing();
  ~CPing();

//Methods
#ifndef CPING_NO_ICMP
	BOOL PingUsingICMP(LPCTSTR pszHostName, CPingReply& pr, UCHAR nTTL = 10, DWORD dwTimeout = 5000, WORD wDataSize = 32, UCHAR nTOS = 0, BOOL bDontFragment = FALSE) const;
#endif
#ifndef CPING_NO_WINSOCK2
	BOOL PingUsingWinsock(LPCTSTR pszHostName, CPingReply& pr, UCHAR nTTL = 10, DWORD dwTimeout = 5000, WORD wDataSize = 32, UCHAR nTOS = 0, BOOL bDontFragment = FALSE, LPCTSTR pszLocalBoundAddress = NULL, USHORT nSequenceNumber = 0) const;
#endif

protected:
// Macros / Defines
#ifndef CPING_NO_WINSOCK2
  //IP header
  #pragma pack(push, 1) //The IP_HEADER and ICMP_HEADER should be alligned on 1 byte boundaries
  typedef struct tagIP_HEADER 
  {
	  unsigned char  h_len:4;        // length of the header
	  unsigned char  version:4;      // Version of IP
	  unsigned char  tos;            // Type of service
	  unsigned short total_len;      // total length of the packet
	  unsigned short ident;          // unique identifier
	  unsigned short frag_and_flags; // flags
	  unsigned char  ttl; 
	  unsigned char  proto;          // protocol (TCP, UDP etc)
	  unsigned short checksum;       // IP checksum
	  unsigned long  sourceIP;
	  unsigned long  destIP;
  } IP_HEADER;
  typedef IP_HEADER FAR* LPIP_HEADER;

  // ICMP header
  typedef struct tagICMP_HEADER 
  {
    BYTE   i_type;
    BYTE   i_code; // type sub code
    USHORT i_cksum;
    USHORT i_id;
    USHORT i_seq;
    ULONG  timestamp; // This is not the std header, but we reserve space for time data field
  } ICMP_HEADER;
  typedef ICMP_HEADER FAR* LPICMP_HEADER;
  #pragma pack(pop)
#endif

//Methods
#ifndef CPING_NO_WINSOCK2
  static BOOL Bind(SOCKET socket, LPCTSTR pszLocalBoundAddress);
  static void FillIcmpData(LPICMP_HEADER pIcmp, int nPacketSize, USHORT nSequenceNumber);
  static BOOL DecodeResponse(LPIP_HEADER pIPHeader, int nBytes, sockaddr_in* from, USHORT nExpectedSequenceNumber, BOOL& bTryAgain);
  static USHORT GenerateIPChecksum(USHORT* pBuffer, int nSize);
#endif

#ifndef CPING_NO_ICMP
//Function Pointers
  typedef HANDLE (WINAPI ICMPCREATEFILE)(void);
  typedef ICMPCREATEFILE* LPICMPCREATEFILE;
  typedef BOOL (WINAPI ICMPCLOSEHANDLE)(HANDLE);
  typedef ICMPCLOSEHANDLE* LPICMPCLOSEHANDLE;
  typedef DWORD (WINAPI ICMPSENDECHO)(HANDLE, IPAddr, LPVOID, WORD, IP_OPTION_INFORMATION*, LPVOID, DWORD, DWORD);
  typedef ICMPSENDECHO* LPICMPSENDECHO;

//Member variables
  HMODULE           m_hIcmp;         //HINSTANCE of Iphlapi.DLL or as a fall back ICMP.DLL
  LPICMPCREATEFILE  m_pIcmpCreateFile;
  LPICMPSENDECHO    m_pIcmpSendEcho;
  LPICMPCLOSEHANDLE m_pIcmpCloseHandle;
#endif

  static BOOL IsSocketReadible(SOCKET socket, DWORD dwTimeout, BOOL& bReadible);
};

#endif //__PING_H__
