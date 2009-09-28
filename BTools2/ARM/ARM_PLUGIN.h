// Request Type 
//----------------------------------------------------------------------
//1.개발자가 ARM_PLUGIN을 포함하여 APP을 디버깅할 경우 
//2.개발자가 테스트용 APP을 개발할 경우
//3.개발자가 ARM_PLUGIN을 포함하여 무료로 배포할 APP을 개발할 경우
//default = NOT_SUPPORTED(OFF)
//CASE SUPPORTED : 모든 함수들이 성공을 리턴하지만 실제 동작하지 않음
//CASE NOT_SUPPORTED : 모든 함수들이 ARM PLUGIN 기능을 정상적으로 수행함
//**********************************************************************
//#define TEST_MODE NOT_SUPPORTED
//**********************************************************************

#define ID_MAX 128 // APPLICATION ID의 최대 크기
#define ARM_SUCCESS 1
#define ARM_FAILURE 0

// Response Type
#define RESPONSE_TYPE_NULL       0

#define NOT_OPENED_CHANNEL 0xF0000001
#define WRONG_LENGTH 0xF0000002
#define INSUFFICIENT_MEMORY 0xF0000003
#define CLICENSE_DENY 0xF0000004
#define CLICENSE_EXPIRED 0xF0000005
#define CLICENSE_NOT_EXIST 0xF0000006
#define CLICENSE_INVALID 0xF0000007
#define QLICENSE_OBTAIN_FAIL 0xF0000008
#define QLICENSE_NOT_PURCHASE 0xF0000009
#define QLICENSE_NOT_FIND_USER 0xF000000A
#define QLICENSE_FOBIDDEN 0xF000000B
#define QLICENSE_ERROR_CHECK_TIMESTAMP 0xF000000C
#define QLICENSE_NOT_REGISTERED_APP 0xF000000D
#define WRONG_PARAMETER 0xF000000F


typedef char TAPPID;

typedef struct _Ret {
	int code;
	int flag;
	char *pMsg;
} T_RETURN;


#ifdef __cplusplus
extern "C" {
#endif

	/*********************************************************/
	/*
	/* ARM PLUGIN APIs
	/*
	/*********************************************************/
	//1. ARM PLUG-IN 초기화 함수
	void ARM_PLUGIN_Initialize (IN OUT T_RETURN * ret, IN TAPPID * pAppID);

	//2. RO 체크 요구 함수
	void ARM_PLUGIN_CheckLicense (IN OUT T_RETURN * ret);

	//3. RO 획득 요구 함수
	void ARM_PLUGIN_RequestLicense (IN OUT T_RETURN * ret);

	//4. ARM PLUG-IN 해제 함수
	void ARM_PLUGIN_Uninitialize (IN OUT T_RETURN * ret);

#ifdef __cplusplus
}
#endif	/* __cplusplus */