/*
 * Copyright (c) 2006 Rutgers, The State University of New Jersey
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that (i) the above copyright notice and this permission notice
 * appear in all copies of the software and related documentation. (ii)
 * the name of Rutgers University may not be used in any advertising
 * or publicity relating to the software without the specific, prior
 * written permission of Rutgers University.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL RUTGERS UNIVERSITY BE LIABLE FOR ANY SPECIAL,
 * INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY
 * THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
/*--------------------------------------------------------------- 
 * Copyright (c) 1999,2000,2001,2002,2003                              
 * The Board of Trustees of the University of Illinois            
 * All Rights Reserved.                                           
 *--------------------------------------------------------------- 
 * Permission is hereby granted, free of charge, to any person    
 * obtaining a copy of this software (Iperf) and associated       
 * documentation files (the "Software"), to deal in the Software  
 * without restriction, including without limitation the          
 * rights to use, copy, modify, merge, publish, distribute,        
 * sublicense, and/or sell copies of the Software, and to permit     
 * persons to whom the Software is furnished to do
 * so, subject to the following conditions: 
 *
 *     
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and 
 * the following disclaimers. 
 *
 *     
 * Redistributions in binary form must reproduce the above 
 * copyright notice, this list of conditions and the following 
 * disclaimers in the documentation and/or other materials 
 * provided with the distribution. 
 * 
 *     
 * Neither the names of the University of Illinois, NCSA, 
 * nor the names of its contributors may be used to endorse 
 * or promote products derived from this Software without
 * specific prior written permission. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTIBUTORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 * ________________________________________________________________
 * National Laboratory for Applied Network Research 
 * National Center for Supercomputing Applications 
 * University of Illinois at Urbana-Champaign 
 * http://www.ncsa.uiuc.edu
 * ________________________________________________________________ 
 *
 * PerfSocket.cpp
 * by Mark Gates <mgates@nlanr.net>
 *    Ajay Tirumala <tirumala@ncsa.uiuc.edu>
 * -------------------------------------------------------------------
 * Has routines the Client and Server classes use in common for
 * performance testing the network.
 * Changes in version 2.0
 *     for extracting data from files
 * -------------------------------------------------------------------
 * headers
 * uses
 *   <stdlib.h>
 *   <stdio.h>
 *   <string.h>
 *
 *   <sys/types.h>
 *   <sys/socket.h>
 *   <unistd.h>
 *
 *   <arpa/inet.h>
 *   <netdb.h>
 *   <netinet/in.h>
 *   <sys/socket.h>
 * ------------------------------------------------------------------- */

#include "stdafx.h"

#define HEADERS()

#include "headers.h"

#include "PerfSocket.hpp"
#include "Locale.hpp"
#include "util.h"

bool PerfSocket::sInterupted = false;
Mutex PerfSocket::sReporting;

/* -------------------------------------------------------------------
 * Initialize the count of how many report lines we've output.
 * ------------------------------------------------------------------- */

int PerfSocket::sReportCount = 0;

/* -------------------------------------------------------------------
 * Store socket info.
 * Allocate buffer used for sending and receiving.
 * If input for the stream is from a file,
 * create the Extractor class for getting data from the file
 * ------------------------------------------------------------------- */

PerfSocket::PerfSocket( ext_Settings *inSettings, CReporter *rep,
                        Notify* toNotify )
: Socket( inSettings->mPort, (inSettings->mUDPRate > 0) ) {

    mSettings = inSettings;
    ptr_parent = toNotify;
    mBuf = NULL;
    extractor = NULL;

	m_pReporter=rep;

   // initialize buffer
    mBuf = new char[ mSettings->mBufLen ];
    pattern( mBuf, mSettings->mBufLen );
    sReportCount = 0;
    if ( mSettings->mServerMode == kMode_Client ) {
        if ( mSettings->mFileInput ) {
            if ( !mSettings->mStdin )
                extractor = new Extractor( mSettings->mFileName, mSettings->mBufLen );
            else
                extractor = new Extractor( stdin, mSettings->mBufLen );

            if ( !extractor->canRead() ) {
                mSettings->mFileInput = false;
            }
        }
    }
}
// end PerfSocket

/* -------------------------------------------------------------------
 * Delete memory (buffer used for sending and receiving).
 * ------------------------------------------------------------------- */

PerfSocket::~PerfSocket() {
    DELETE_ARRAY( mBuf );
    DELETE_PTR( extractor );
}
// end ~PerfSocket

/* -------------------------------------------------------------------
 * Initialization done before any transfer.
 * ------------------------------------------------------------------- */

void PerfSocket::InitTransfer( void ) {
    assert( mSock >= 0   );
    assert( mBuf != NULL );
    assert( mSettings->mBufLen > 0  );

    ReportPeer( mSock );

    // cummulative bytes written
    mTotalLen = 0;

    // UDP jitter calculations
    mJitter = 0.0;

    // for periodic reports of bandwidth and lost datagrams
    mPInterval.set( mSettings->mInterval );
    mPReporting      = (mSettings->mInterval > 0.0);
    mPLastErrorcnt   = 0;
    mPLastDatagramID = 0;
    mPLastTotalLen   = 0;

    // start timers
    mStartTime.setnow();

    if ( ptr_parent != NULL ) {
        ptr_parent->StartTime(mStartTime);
    }

    mPLastTime = mStartTime;
    mPNextTime = mStartTime;
    mPNextTime.add( mPInterval );

    // setup termination variables
    mMode_time = ( mSettings->mAmount < 0 );
    if ( mMode_time ) {
        mEndTime = mStartTime;
        mEndTime.add( (-mSettings->mAmount) / 100.0 );
    } else {
        mAmount = mSettings->mAmount;
    }
}
// end InitTransfer

/* -------------------------------------------------------------------
 * Periodically report the bandwidth.
 * ------------------------------------------------------------------- */

void PerfSocket::ReportPeriodicBW( void ) {
    if ( mPReporting  &&
         mPacketTime.after( mPNextTime ) ) {

        double inStart = mPLastTime.subSec( mStartTime );
        double inStop = mPNextTime.subSec( mStartTime );

        sReporting.Lock();
        ReportBW( mTotalLen - mPLastTotalLen,
                  inStart,
                  inStop );

        if ( ptr_parent ) {
            ptr_parent->PeriodicUpdate( inStart,
                                        inStop,
                                        mTotalLen - mPLastTotalLen );
        }
        sReporting.Unlock();

        mPLastTime = mPNextTime;
        mPNextTime.add( mPInterval );

        mPLastTotalLen   = mTotalLen;

        if ( mPacketTime.after( mPNextTime ) ) {
            ReportPeriodicBW();
        }

    }
}

/* -------------------------------------------------------------------
 * Periodically report the bandwidth, jitter, and loss.
 * Used by the UDP server only.
 * ------------------------------------------------------------------- */

void PerfSocket::ReportPeriodicBW_Jitter_Loss( int32_t errorCnt,
                                               int32_t outofOrder,
                                               int32_t datagramID ) {
    if ( mPReporting  &&
         mPacketTime.after( mPNextTime ) ) {

        double inStart = mPLastTime.subSec( mStartTime );
        double inStop = mPNextTime.subSec( mStartTime );

        sReporting.Lock();
        ReportBW_Jitter_Loss( mTotalLen - mPLastTotalLen,
                              inStart,
                              inStop,
                              errorCnt - mPLastErrorcnt,
                              outofOrder - mPLastOutofOrder,
                              datagramID - mPLastDatagramID );
        if ( ptr_parent ) {
            ptr_parent->PeriodicUpdate( inStart,
                                        inStop,
                                        mTotalLen - mPLastTotalLen );
        }
        sReporting.Unlock();

        mPLastTime = mPNextTime;
        mPNextTime.add( mPInterval );

        mPLastTotalLen   = mTotalLen;
        mPLastDatagramID = datagramID;
        mPLastErrorcnt   = errorCnt;
        mPLastOutofOrder = outofOrder;

        if ( mPacketTime.after( mPNextTime ) ) {
            ReportPeriodicBW_Jitter_Loss( errorCnt, outofOrder, datagramID );
        }

    }
}

/* -------------------------------------------------------------------
 * Report the bandwidth (inBytes / inSeconds).
 * ------------------------------------------------------------------- */

void PerfSocket::IPerfWinCEReport(char *buffer,char *speed, double fBW)
{
	if (m_pReporter!=0)
		m_pReporter->PrintBuffer(buffer,speed);
	if (m_pReporter!=0 && fBW > 0)
		m_pReporter->CallbackBW(fBW);
}

void PerfSocket::ReportBW( max_size_t inBytes,
                           double inStart,
                           double inStop ) {
    // print a field header every 20 lines

	char reportBuffer[256];

    if ( --sReportCount <= 0 ) 
	{
		sprintf(reportBuffer,report_bw_header);

		IPerfWinCEReport(reportBuffer,0);
		//        printf( report_bw_header );
        sReportCount = 20;
    }

    char bytes[ 32 ];
    char speed[ 32 ];

    byte_snprintf( bytes, sizeof(bytes), (double) inBytes,
                   toupper( mSettings->mFormat));


	// inStop - inStart != 0 can be happen when debug server side code.
	// In client side, server doesn't work & time doesn't goes.
	// Server side stopped while client trying to calculate BW
	if(inStop - inStart != 0)
	{
		byte_snprintf( speed, sizeof(speed),
                   inBytes / (inStop - inStart), mSettings->mFormat);
		
		sprintf( reportBuffer, report_bw_format, mSock, inStart, inStop, bytes, speed );
		IPerfWinCEReport(reportBuffer, speed, inBytes / (inStop - inStart));
	}

	//IPerfWinCEReport(reportBuffer,speed);

	/*
    printf( report_bw_format,
            mSock, inStart, inStop, bytes, speed );
    fflush( stdout );
	*/
}
// end ReportBW

/* -------------------------------------------------------------------
 * Report the bandwidth (inBytes / inSeconds).
 * ------------------------------------------------------------------- */

void PerfSocket::ReportBW_Jitter_Loss( max_size_t inBytes,
                                       double inStart,
                                       double inStop,
                                       int32_t inErrorcnt,
                                       int32_t inOutofOrder,
                                       int32_t inDatagrams ) {

	char reportBuffer[256];

    // print a field header every 20 lines
    if ( --sReportCount <= 0 ) {
        sprintf(reportBuffer, report_bw_jitter_loss_header );
		IPerfWinCEReport(reportBuffer, 0);
        sReportCount = 20;
    }

    assert( inErrorcnt >= 0 );
    assert( inDatagrams >= 0 );

    char bytes[ 32 ];
    char speed[ 32 ];

    byte_snprintf( bytes, sizeof(bytes), (double) inBytes,
                   toupper( mSettings->mFormat));
    byte_snprintf( speed, sizeof(speed),
                   inBytes / (inStop - inStart), mSettings->mFormat);

    // assume most of the time out-of-order packets are not
    // duplicate packets, so subtract them from the lost packets.
    inErrorcnt -= inOutofOrder;


    sprintf(reportBuffer,report_bw_jitter_loss_format,
            mSock, inStart, inStop, bytes, speed,
            mJitter*1000.0, inErrorcnt, inDatagrams,
            (100.0 * inErrorcnt) / inDatagrams );
	IPerfWinCEReport(reportBuffer,0);

    if ( inOutofOrder > 0 ) {
        sprintf(reportBuffer, report_outoforder,
                mSock, inStart, inStop, inOutofOrder );

		IPerfWinCEReport(reportBuffer,0);
    }
    fflush( stdout );
}
// end ReportBW_Jitter_Loss

/* -------------------------------------------------------------------
 * Report a socket's peer IP address.
 * ------------------------------------------------------------------- */

void PerfSocket::ReportPeer( int inSock ) {
    assert( inSock >= 0 );

    SocketAddr local  = getLocalAddress();
    SocketAddr remote = getRemoteAddress();

    // copy the inet_ntoa into temp buffers, to avoid overwriting
    char local_addr[ REPORT_ADDRLEN ];
    local.getHostAddress( local_addr, sizeof(local_addr));

    char remote_addr[ REPORT_ADDRLEN ];
    remote.getHostAddress( remote_addr, sizeof(remote_addr));

    sReporting.Lock();

	char reportBuffer[256];

    sprintf(reportBuffer, report_peer,
            inSock,
            local_addr,  local.getPort(),
            remote_addr, remote.getPort());
	IPerfWinCEReport(reportBuffer,0);

    fflush( stdout );

    sReporting.Unlock();
}
// end ReportPeer

/* -------------------------------------------------------------------
 * Report the MSS and MTU, given the MSS (or a guess thereof)
 * ------------------------------------------------------------------- */

// compare the MSS against the (MTU - 40) to (MTU - 80) bytes.
// 40 byte IP header and somewhat arbitrarily, 40 more bytes of IP options.

inline bool checkMSS_MTU( int inMSS, int inMTU );

inline bool checkMSS_MTU( int inMSS, int inMTU ) {
    return(inMTU-40) >= inMSS  &&  inMSS >= (inMTU-80);
}

void PerfSocket::ReportMSS( int inMSS ) {
    sReporting.Lock();

    if ( inMSS <= 0 ) {
        printf( report_mss_unsupported, mSock );
    } else {
        char* net;
        int mtu = 0;

        if ( checkMSS_MTU( inMSS, 1500 ) ) {
            net = "ethernet";
            mtu = 1500;
        } else if ( checkMSS_MTU( inMSS, 4352 ) ) {
            net = "FDDI";
            mtu = 4352;
        } else if ( checkMSS_MTU( inMSS, 9180 ) ) {
            net = "ATM";
            mtu = 9180;
        } else if ( checkMSS_MTU( inMSS, 65280 ) ) {
            net = "HIPPI";
            mtu = 65280;
        } else if ( checkMSS_MTU( inMSS, 576 ) ) {
            net = "minimum";
            mtu = 576;
            printf( warn_no_pathmtu );
        } else {
            mtu = inMSS + 40;
            net = "unknown interface";
        }

        printf( report_mss,
                mSock, inMSS, mtu, net );
    }
    fflush( stdout );

    sReporting.Unlock();
}
// end ReportMSS

/* -------------------------------------------------------------------
 * Report the TCP window size/UDP buffer size and warn if not
 * the same as that requested.
 * ------------------------------------------------------------------- */

void PerfSocket::ReportWindowSize( void ) {
    // sReporting already locked from ReportClient/ServerSettings
/*
    int win = get_tcp_windowsize( mSock );
    int win_requested = mSettings->mTCPWin;

    char window[ 32 ];
    byte_snprintf( window, sizeof(window), win,
                   toupper( mSettings->mFormat));
    printf( "%s: %s", (mUDP ? udp_buffer_size : tcp_window_size), window );

    if ( win_requested == 0 ) {
        printf( " %s", window_default );
    } else if ( win != win_requested ) {
        char request[ 32 ];
        byte_snprintf( request, sizeof(request), win_requested,
                       toupper( mSettings->mFormat));
        printf( warn_window_requested, request );
    }
    printf( "\n" );
    fflush( stdout );
	*/
}
// end ReportWindowSize

/* -------------------------------------------------------------------
 * Report all the client settings: port, UDP/TCP, local and remote
 * addresses and ports, multicast TTL, TCP window size.
 * ------------------------------------------------------------------- */

void PerfSocket::ReportClientSettings( const char* inHost,
                                       const char* inLocalhost ) {
    sReporting.Lock();

    // print settings
	/*
    printf( seperator_line );
    printf( client_port, inHost, (mUDP ? "UDP" : "TCP"), mPort );

	sprintf(buffer, seperator_line);
	IPerfWinCEReport(buffer, 0);
	*/

	char buffer[256];
	sprintf(buffer, seperator_line);
	IPerfWinCEReport(buffer, 0);

	sprintf(buffer, client_port, inHost, (mUDP ? "UDP" : "TCP"), mPort );
	IPerfWinCEReport(buffer, 0);

    if ( inLocalhost != NULL ) {
        SocketAddr local = getLocalAddress();
        char addr[ REPORT_ADDRLEN ];
        local.getHostAddress( addr, sizeof(addr));
        //printf( bind_address, addr );
		sprintf(buffer,bind_address, addr );
		IPerfWinCEReport(buffer, 0);
    }

    if ( mUDP ) {
        printf( client_datagram_size, mSettings->mBufLen );

        SocketAddr remote = getRemoteAddress();
        if ( remote.isMulticast() ) {
            sprintf(buffer, multicast_ttl, mSettings->mTTL); ///<
			IPerfWinCEReport(buffer, 0);
			
        }
    }

    ReportWindowSize();
    /*
	printf( seperator_line );
    fflush( stdout );
	*/
    sReporting.Unlock();
}

/* -------------------------------------------------------------------
 * Report all the server settings (prior to accept): port, UDP/TCP,
 * local address, multicast group, TCP window size.
 * ------------------------------------------------------------------- */

void PerfSocket::ReportServerSettings( const char* inLocalhost ) {
    sReporting.Lock();

    // print settings
	char buffer[256];
    //printf( seperator_line );
    //printf( server_port, (mUDP ? "UDP" : "TCP"), mPort );
	sprintf(buffer, seperator_line);
	IPerfWinCEReport(buffer, 0);

    sprintf(buffer, server_port, (mUDP ? "UDP" : "TCP"), mPort );
	IPerfWinCEReport(buffer, 0);
	


    if ( inLocalhost != NULL ) {
        SocketAddr local = getLocalAddress();
        char addr[ REPORT_ADDRLEN ];
        local.getHostAddress( addr, sizeof(addr));

        sprintf(buffer,  bind_address, addr );
		IPerfWinCEReport(buffer, 0);

        if ( local.isMulticast() ) {
            sprintf(buffer,  join_multicast, addr );
			IPerfWinCEReport(buffer, 0);
        }
    }
    if ( mUDP ) {
        sprintf(buffer, server_datagram_size, mSettings->mBufLen );
		IPerfWinCEReport(buffer, 0);
    }
    ReportWindowSize();
    sprintf(buffer, seperator_line);
	IPerfWinCEReport(buffer, 0);
    fflush( stdout );

    sReporting.Unlock();
}

/* -------------------------------------------------------------------
 * Signal handler sets the sInterupted flag, so the object can
 * respond appropriately.. [static]
 * ------------------------------------------------------------------- */

void PerfSocket::Sig_Interupt( int inSigno ) {
    sInterupted = true;
}

/* -------------------------------------------------------------------
 * Set socket options before the listen() or connect() calls.
 * These are optional performance tuning factors.
 * TODO should probably reporting setting these.
 * ------------------------------------------------------------------- */

void PerfSocket::SetSocketOptions( void ) {
    // set the TCP window size (socket buffer sizes)
    // also the UDP buffer size
    // must occur before call to accept() for large window sizes
    set_tcp_windowsize( mSock, mSettings->mTCPWin );

#ifdef IP_TOS

    // set IP TOS (type-of-service) field
    if ( mSettings->mTOS > 0 ) {
        int  tos = mSettings->mTOS;
        Socklen_t len = sizeof(tos);
        int rc = setsockopt( mSock, IPPROTO_IP, IP_TOS,
                             (char*) &tos, len );
        FAIL_errno( rc == SOCKET_ERROR, "setsockopt IP_TOS" );
    }
#endif

    if ( ! mUDP ) {
        // set the TCP maximum segment size
        setsock_tcp_mss( mSock, mSettings->mMSS );

#ifdef TCP_NODELAY

        // set TCP nodelay option
        if ( mSettings->mNodelay ) {
            int nodelay = 1;
            Socklen_t len = sizeof(nodelay);
            int rc = setsockopt( mSock, IPPROTO_TCP, TCP_NODELAY,
                                 (char*) &nodelay, len );
            FAIL_errno( rc == SOCKET_ERROR, "setsockopt TCP_NODELAY" );
        }
#endif
    }
}
// end SetSocketOptions

