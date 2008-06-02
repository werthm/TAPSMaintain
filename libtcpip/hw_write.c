#include "stdio.h"
#include "string.h"

#include "commands.h"
#include "tcpip.h"
#include "netdef.h"


void Set_BAF2_HV(char* server, int det, int value)
{
    int  port = TAPS_SERVER_PORT;
    int  cmd[BUFF_LEN];
    int  i, status;
    int  socket_id;  
    
    cmd[0] = 5*4;
    cmd[1] = HV_BAF_SET;
    cmd[2] = det;
    cmd[3] = value;
    cmd[4] = 0;
      
    //printf("<I> %-8s: Port %5d Command %3d %3d %3d %3d %3d\n", server, port, cmd[0], cmd[1], cmd[2], cmd[3], cmd[4]);

    // connect to server
    if((socket_id = do_connect(server, port, 0)) < 0) 
    {
        printf("ERROR: Cannot connect to TapsServer %s:%i\n", server, port);
        return;
    }
    status = sendbuffer(socket_id, cmd);
    status = getbuffer(socket_id,cmd);
    close(socket_id);

    // output return code
    if (status) printf("ERROR IN TCP CONNECTION !!! :%d:\n", status);
    else
    {
        //printf("    Returned:");
	//for (i = 0; i < (cmd[0]/4); i++)
        //{
	//    printf(" %3d", (cmd[i]));
        //}
	//printf("\n");
    }
}

