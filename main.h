#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#define FNameMax 32       /* Max length of File Name */ 
#define FileMax  32 /* Max number of Files */ 
#define baseM 6 /* base number */ 
#define ringSize 64 /* ringSize = 2^baseM */ 
#define fBufSize 1024 //1024 /* file buffer size */
#define PRIME_MULT 1717
#define PRIME_DOUBLE 13	/*For double hashing*/
#define MAX_QUEUESIZE 5
#define nodefmax 10 //한 노드가 참조할 수 있는 최대 파일 개수 

#define WAIT_TIME 10	//pp메시지 10초 제한.

#define REQUEST 0
#define RESPONSE 1
#define SUCCESS 0
#define FAILURE -1

#define JOIN_INFO 1
#define MOVE_KEYS 2
#define PING_PONG 3
#define PREDECESSOR_INFO 4
#define PREDECESSOR_UPDATE 5
#define SUCCESSOR_INFO 6
#define SUCCESSOR_UPDATE 7
#define FIND_PREDCESSOR 8
#define LEAVE_KEYS 9
#define FILE_REF_ADD 10
#define FILE_REF_DEL 11
#define FILE_DOWN 12
#define FILE_REF_INFO 13

typedef struct {           /* Node Info Type Structure */
	int ID;                 /* ID */
	struct sockaddr_in addrInfo;  /* Socket address */
} nodeInfoType;

typedef struct {     /* File Type Structure */
	char Name[FNameMax]; /* File Name */
	int  Key; /* File Key */
	nodeInfoType owner; /* Owner's Node */
	nodeInfoType refOwner; /* Ref Owner's Node */
} fileRefType;

typedef struct {  /* Global Information of Current Files */
	unsigned int fileNum;  /* Number of files */
	fileRefType  fileRef[FileMax]; /* The Number of Current Files */
} fileInfoType;

typedef struct {   /* Finger Table Structure */
	nodeInfoType Pre;   /* Predecessor pointer */
	nodeInfoType finger[baseM]; /* Fingers (array of pointers) */
} fingerInfoType;

typedef struct {              /* Chord Information Structure */
	fileInfoType   FRefInfo; /* File Ref Own Information */
	fingerInfoType fingerInfo;  /* Finger Table Information */
} chordInfoType;

typedef struct { /* Node Structure */
	nodeInfoType  nodeInfo;   /* Node's IPv4 Address */
	fileInfoType  fileInfo;   /* File Own Information */
	chordInfoType chordInfo;  /* Chord Data Information */
} nodeType;


typedef struct {
	unsigned short msgID;   // message ID 
	unsigned short msgType; // message type (0: request, 1: response) 
	nodeInfoType   nodeInfo;// node address info 
	short           moreInfo; // more info 
	fileRefType    fileInfo; // file (reference) info 
	unsigned int   bodySize; // body size in Bytes 
} chordHeaderType;           // CHORD message header type

void error_message(char *);

int create(nodeType *Usr_node);
int join(char* HasingKey);
void test_join(void);
int leave(void);
void addfile(void);
void deletefile(void);
void stabilize_leave(int);
//void notify(nodeType *askNode, nodeType *targetNode);
void FHelp();
void SHelp();
void TestMode(void);
unsigned int str_hash(const char *str);  /* Hash: String to Key */
unsigned int str_double_hash(const char *str);
int move_keys(nodeType *toNode, nodeInfoType *fromNode);
int lookup(nodeType *curNode, int fileKey, nodeInfoType *foundNode);


/*Thread*/
void procRecvMsg(void *arg);
void procPingPong(void *arg);
void procFixfinger(void *arg);
void File_Send_Thread(char *arg);
void File_Recieve_Thread(fileRefType *arg);
void Join_Info_Thread(nodeInfoType *arg);
void Find_Pred_Thread(nodeInfoType *arg);

/*Message*/
void PingPong_Request(chordHeaderType *);
void PingPong_Response(chordHeaderType *);
void Join_Info_Request(chordHeaderType *);
void Join_Info_Response(chordHeaderType *);
void MoveKeys_Request(chordHeaderType *);
void MoveKeys_Response(chordHeaderType *);
void Predecessor_Info_Request(chordHeaderType *);
void Predecessor_Info_Response(chordHeaderType *);
void Predecessor_Update_Request(chordHeaderType *);
void Predecessor_Update_Response(chordHeaderType *);
void Successor_Info_Request(chordHeaderType *);
void Successor_Info_Response(chordHeaderType *);
void Successor_Update_Request(chordHeaderType *);
void Successor_Update_Response(chordHeaderType *);
void Find_Predecessor_Request(chordHeaderType *, int key);
void Find_Predecessor_Response(chordHeaderType *, nodeInfoType pred);
void LeaveKeys_Request(chordHeaderType *);
void LeaveKeys_Response(chordHeaderType *);
void File_Reference_Add_Request(chordHeaderType *);
void File_Reference_Add_Response(chordHeaderType *);
void File_Reference_Delete_Request(chordHeaderType *,int);
void File_Reference_Delete_Response(chordHeaderType *);
void FileDown_Request(chordHeaderType *,char*);
void FileDown_Response(chordHeaderType *,int);
void File_Reference_Info_Request(chordHeaderType *,int);
void File_Reference_Info_Response(chordHeaderType *,int);








/*Node Fuction*/
int twoPow(int power);
// For getting a power of 2 
int modMinus(int modN, int minuend, int subtrand);
// For modN modular operation of "minend - subtrand"
int modPlus(int modN, int addend1, int addend2);
// For modN modular operation of "addend1 + addend2"
int modIn(int modN, int targNum, int range1, int range2, int leftmode, int rightmode);
// For checking if targNum is "in" the range using left and right modes 
// under modN modular environment 


nodeInfoType find_successor(nodeType *curNode, int IDKey);
// For finding successor of IDKey for a node curNode

nodeInfoType find_predecessor(nodeType *curNode, int IDKey);
// For finding predecessor of IDKey for a node curNode

nodeInfoType find_closest_predecessor(nodeType *curNodeID, int IDKey);
// For finding closest predecessor of IDKey for a node curNode
int fix_finger();