/************************************************/           
/* This header file contains the definitions of */
/* the commands and error messages for the      */
/* Slow Control Example. At this stage it only  */
/* knows how to handle CFD communication. The   */
/* rest is not implemented.       JGM, 17/12/02 */
/* 10.03.03 Implementation of LED, QAC, etc.    */
/************************************************/           

/* buffer length */

#define BUFF_LEN        8*4096  /* bytes */
#define MAX_CHANNELS    594     /* Maximum channels, for baf only 544 needed but for boards_par.... */
/* Error message definitions */
/* In this stage this is rather primitive, only      */
/* success or error are reported back to the client. */
/* This should be more specific for a real server.   */

#define ACCEPT                -1 /* Ok, no error */
#define ERROR_MYSQL           -2 /* Error in MySql Server */
#define ERROR_TAPSSERVER      -3 /* Error in TapsServer */
#define ERROR_HWSERVER        -4 /* Error in HardwareServer */
#define ERROR_HVSERVER        -5 /* Error in HV-Server */

/* command definitions */

#define PING                   1  /* Used to check whether server is alive */
#define TEST_QAC               5  /* Test Qacs and write Pedestals to DB */
#define TEST_VQAC              6  /* Test Qacs and write Pedestals to DB */
#define END                  100  /* End the server with this command */
//#define END_HWS              101  /* End all Hardwareservers

#define BOARD_SET             52  /* for static pars of CAEN-Boards */
#define BOARD_UPLOAD          53
#define BOARD_INIT            54
#define VBOARD_UPLOAD         55
#define PBOARD_UPLOAD         56


#define CFD_TH_READ           30  /* Read threshold of a detector */ 
#define CFD_TH_SET            31  /* Set threshold for a detector */ 
#define CFD_UPLOAD            32  /* Uploads all the relevant info for the hardwareServer */
#define CFD_INIT              33  /* Re-initializes CFD parameters */

#define LEDL_TH_READ          40  /* Read threshold of a detector */ 
#define LEDL_TH_SET           41  /* Set threshold for a detector */ 
#define LEDL_UPLOAD           42  /* Uploads all the relevant info for the hardwareServer */
#define LEDL_INIT             43  /* Re-initializes CFD parameters */
#define LEDH_TH_READ          45   
#define LEDH_TH_SET           46   
#define LEDH_UPLOAD           47  
#define LEDH_INIT             48  

#define QACLG_TH_READ         10  /* QAC Long Gate */
#define QACLG_TH_SET          11
#define QACLG_UPLOAD          12 
#define QACLG_INIT            13  
#define QACLGS_TH_READ        15  /* QAC Long gate sensitive */
#define QACLGS_TH_SET         16  
#define QACLGS_UPLOAD         17 
#define QACLGS_INIT           18 
#define QACSG_TH_READ         20  /* QAC Short Gate */
#define QACSG_TH_SET          21 
#define QACSG_UPLOAD          22  
#define QACSG_INIT            23 
#define QACSGS_TH_READ        25  /* QAC Short Gate sensitive */
#define QACSGS_TH_SET         26  
#define QACSGS_UPLOAD         27 
#define QACSGS_INIT           28 

#define VS_READ               60   /* V set */
#define VS_SET                61   
#define VS_UPLOAD             62  
#define VS_INIT               63  
#define VO_READ               65   /* V off */
#define VO_SET                66   
#define VO_UPLOAD             67  
#define VO_INIT               68  

#define IP_UPLOAD             50 /* Uploads all the IP hostnames of the HardWareServers */

#define HV_BAF_READ          70
#define HV_BAF_SET           71
#define BAF_UPLOAD           72
#define HV_BAF_INIT          73
#define HV_VET_READ          75
#define HV_VET_SET           76
#define VET_UPLOAD           77
#define HV_VET_INIT          78

#define VQAC_TH_READ         80
#define VQAC_TH_SET          81
#define VQAC_UPLOAD          82
#define VQAC_INIT            83
#define VLED_TH_READ         85
#define VLED_TH_SET          86
#define VLED_UPLOAD          87
#define VLED_INIT            88

#define VVS_READ             90   /* V set */
#define VVS_SET              91
#define VVS_UPLOAD           92
#define VVS_INIT             93
#define VVO_READ             95   /* V off */
#define VVO_SET              96
#define VVO_UPLOAD           97
#define VVO_INIT             98





