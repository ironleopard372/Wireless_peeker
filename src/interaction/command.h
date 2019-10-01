#ifdef  INTERCATION_COMMAN_H
#define INTERCATION_COMMAN_H

#define COMMAND_BUFFER_LEN 50

#define	MITM_CTRL_CONNECT_REQUEST "MITM-CTRL-CONNECT-REQUEST"
#define	MITM_CTRL_CONNECT_REPLY "MITM-CTRL-CONNECT-REPLY"
#define	MITM_CTRL_DISCONNECT_REQUEST "MITM-CTRL-DISCONNECT-REQUEST,"
#define	MITM_CTRL_DISCONNECT_REPLY "MITM-CTRL-DISCONNECT-REPLY"
#define	MITM_GET_AP_LIST_REQUEST "MITM-GET-AP-LIST-REQUEST"
#define	MITM_GET_AP_LIST_REPLY "MITM-GET-AP-LIST-REPLY"
#define	MITM_SET_AP_REQUEST "MITM-SET-AP-REQUEST"
#define	MITM_SET_AP_REPLY "MITM-SET-AP-REPLY"
#define	MITM_SET_VICTIM_REQUEST "MITM-SET-VICTIM-REQUEST"
#define	MITM_SET_VICTIM_REPLY "MITM-SET-VICTIM-REPLY"
#define	MITM_GET_STATUS_REQUEST "MITM-GET-STATUS-REQUEST"
#define	MITM_GET_STATUS_REPLY "MITM-GET-STATUS-REPLY"
#define	MITM_STATUS_CHANGED "MITM-STATUS-CHANGED"
#define	MITM_START_ATTACK_REQUEST "MITM-START-ATTACK-REQUEST"
#define	MITM_START_ATTACK_REPLY "MITM-START-ATTACK-REPLY"
#define	MITM_KEEP_ALIVE_REQUSET "MITM-KEEP-ALIVE-REQUSET"
#define	MITM_KEEP_ALIVE_REPLY "MITM-KEEP-ALIVE-REPLY"
#define	MITM_INVAILD_MESSAGE_FORMAT "MITM-INVAILD-MESSAGE-FORMAT"


struct MITM_MSG{
	enum MITM_COMMAND commnad;
	char* data;
	size_t data_len;
};
#endif /* INTERACTION_COMMAN_H */