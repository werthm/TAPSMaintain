#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(OSF1) || defined(ULTRIX)
# include <strings.h>
# include <sys/time.h>
# include <sys/types.h>
#elif defined(VMS)
# include <string.h>
# include <time.h>
# include <types.h>
# include <errno.h>
#endif
#include "tcpip.h"

int tcp_timeout = 15;

/******************************************************************************/

int netwrite(int socket, char* buffer, int length, int size)
{
int temp;
int still_written;
int clength;
   
#if defined(VMS)
  errno=0;
#endif
  
clength = size*length;

if (!clength) 
  return 0;

switch(size) {
  case 2 : htons_a( (short*) buffer, length );
	   break;
  case 4 : htonl_a( (int*) buffer, length );
	   break;
  }

still_written = 0;
do {
 if (wait_for_socket_w(socket,tcp_timeout)>0) {
#if defined(VMS)
   if (errno)		{
     			return TCP_ERROR;
   			}
#endif
   temp = write(socket,&buffer[still_written],clength-still_written);
   if 	 (temp<=0)	{
     			return TCP_ERROR;
   			}
   else			still_written += temp;
   }
 else {
   return TCP_ERROR;
   }
 } while (still_written<clength);

switch(size) {
  case 2 : ntohs_a( (short*) buffer, length );
	   break;
  case 4 : ntohl_a( (int*) buffer, length );
	   break;
  }

return TCP_OK;
}

/******************************************************************************/

int netread(int socket, char* buffer, int length, int size)
{
int temp;
int still_read;
int clength;

//printf("Start netread\n");

#if defined(VMS)
  errno=0;
#endif
  
clength = size * length;

if (!clength)
 return 0;

still_read = 0;
do {
 if (wait_for_socket_r(socket,tcp_timeout)>0) {
#if defined(VMS)
   if (errno)		{
     			return TCP_ERROR;
   			}
#endif

//printf("read from socket\n");

   temp = read(socket,&buffer[still_read],clength-still_read);
   if 	 (temp<=0)	{
			return TCP_ERROR;
   			}
   else			still_read += temp;
   }
 else {
   return TCP_ERROR;
   }
 } while (still_read<clength);

switch(size) {
  case 2 : ntohs_a( (short*) buffer, length );
	   break;
  case 4 : ntohl_a( (int*) buffer, length );
	   break;
  }

//printf("Ende netread\n");
   
return TCP_OK;
}

/******************************************************************************/

int getbuffer(int socket, int* buffer)
{
  
//printf("Start getbuffer\n");

  if (netread(socket,(char *)buffer,1,4)!=TCP_OK)
    {
      return TCP_ERROR;
    }

  if (netread(socket,(char *)&buffer[1],(buffer[0]/4)-1,4)!=TCP_OK)
    {
      return TCP_ERROR;
    }

//printf("Ende getbuffer\n");

  return TCP_OK;
}
/******************************************************************************/

int sendbuffer(int socket, int* buffer)
{

  if (netwrite(socket,(char *)buffer,buffer[0]/4,4)!=TCP_OK)
    {
      return TCP_ERROR;
    }

  return TCP_OK;
}


/******************************************************************************/

void htons_a(short* buffer, int length)
{
do {
   length--;
   buffer[length] = htons(buffer[length]);
   } while (length);

return;
}

/******************************************************************************/

void ntohs_a(short* buffer, int length)
{
do {
   length--;
   buffer[length] = ntohs(buffer[length]);
   } while (length);

return;
}

/******************************************************************************/

void htonl_a(int* buffer, int length)
{
do {
   length--;
   buffer[length] = htonl(buffer[length]);
   } while (length);

return;
}

/******************************************************************************/

void ntohl_a(int* buffer, int length)
{
do {
   length--;
   buffer[length] = ntohl(buffer[length]);
   } while (length);

return;
}

/******************************************************************************/

int install_listen(int port, int noblock)
{
struct sockaddr_in	local_address;
int			listen_socket;
int			optval,flags;

                                        /* clear/fill address structure */
memset(&local_address,0 , sizeof(local_address) );
local_address.sin_family        = AF_INET;
local_address.sin_addr.s_addr   = htonl(INADDR_ANY);
local_address.sin_port          = htons(port);

                                        /* get socket */
listen_socket = socket(AF_INET,SOCK_STREAM,0);
if (listen_socket==-1) 	{
	return TCP_ERROR;
	}

                                        /* set option reuseaddress */
optval = 1;
if (setsockopt(listen_socket,SOL_SOCKET,SO_REUSEADDR, (char*) &optval
                                                        , sizeof(optval))<0) {
    close(listen_socket);
    return TCP_ERROR;
    }
                                        /* bind to local address */
if (bind(listen_socket, (struct sockaddr*) &local_address,
                                        sizeof(local_address))<0 ) {
        close(listen_socket);
	return TCP_ERROR;
	}

                                        /* switch listen mode on */
if (listen(listen_socket,5)<0) {
        close(listen_socket);
	return TCP_ERROR;
	}

   flags=fcntl(listen_socket, F_GETFL, 0);
   if (noblock)
     {
	flags |= O_NONBLOCK;
     }
   else
     {
	flags &= (~O_NONBLOCK);
     }
   fcntl(listen_socket, F_SETFL, flags);

 return listen_socket;
}
 
/******************************************************************************/

int do_connect(char* hostname, int port, int noblock)
{
struct sockaddr_in temp_address;
int temp_socket;
int i,sum;
int optval, flags;

temp_address = host2addr(hostname,port);

sum = 0;
for (i=0;i<sizeof(temp_address);sum += ((char*) &temp_address)[i++]);
if (sum==0) {
	return TCP_ERROR;
	}

if ( (temp_socket=socket(AF_INET,SOCK_STREAM,0)) < 0) {
	return TCP_ERROR;
	}

                                        /* set option reuseaddress */
optval = 1;
if (setsockopt(temp_socket,SOL_SOCKET,SO_REUSEADDR, (char*) &optval
                                                        , sizeof(optval))<0) {
    close(temp_socket);
    return TCP_ERROR;
    }

   flags=fcntl(temp_socket, F_GETFL, 0);
   if (noblock)
     {
	flags |= O_NONBLOCK;
     }
   else
     {
	flags &= (~O_NONBLOCK);
     }
   fcntl(temp_socket, F_SETFL, flags);

if ( connect(temp_socket, (struct sockaddr*) &temp_address,
                                sizeof(temp_address)) < 0 ) {
        close(temp_socket);
	return TCP_ERROR;
	}

return temp_socket;
}

/******************************************************************************/

struct sockaddr_in host2addr(char* hostname, int port)
{
static	struct sockaddr_in	sa;
struct hostent*			host;

				/* clear address */
memset( &sa,0 , sizeof(sa) );

				/* get address */
if ( (host=gethostbyname(hostname))==NULL ) {
	return sa;
	}
				/* fill address */
sa.sin_family		= AF_INET;
sa.sin_port		= htons(port);
sa.sin_addr.s_addr	= *( (u_long*) (host->h_addr) );

return sa;
}

/******************************************************************************/

char* addr2host(struct sockaddr_in* address, int* port)
{
struct hostent*			host;
					/* get hostname */
host = gethostbyaddr( (char*) &(address->sin_addr) ,
					sizeof(address->sin_addr), AF_INET);
if (host == NULL) {
	return NULL;
	}

*port = ntohs(address->sin_port);
return host->h_name;
}

/******************************************************************************/

char* addr2hostnew(struct sockaddr_in* address, int* port, char* remote)
{

struct hostent*                 host;
                                        /* get hostname */

host = gethostbyaddr( (char*) &(address->sin_addr) ,
  sizeof(address->sin_addr), AF_INET);

//struct in_addr *ad;   /* and this to hold an IP adresse     */
//char *test = inet_ntoa(address->sin_addr.s_addr);
	printf("I-Netadresse:%i\n",address->sin_addr.s_addr);
//ad=(struct in_addr*)*host->h_addr_list++;
//sprintf(remote,"%s", inet_ntoa(*ad));


if (host == NULL) {
        return NULL;
        }

*port = ntohs(address->sin_port);



//char * test_ip_address = remote;
return remote; //test_ip_address;        
}

/******************************************************************************/

int  wait_for_socket_r(int socket, int seconds)
{
fd_set help;
struct timeval tm;

tm.tv_sec  = seconds;
tm.tv_usec = 0; 

FD_ZERO(&help);
FD_SET(socket,&help);

if (seconds>0) 
	return select(socket+1,&help,(fd_set*) 0, (fd_set*) 0, &tm);
else
	return select(socket+1,&help,(fd_set*) 0, (fd_set*) 0, NULL);
}

/******************************************************************************/

int  wait_for_socket_w(int socket, int seconds)
{
fd_set help;
struct timeval tm;

tm.tv_sec  = seconds;
tm.tv_usec = 0; 

FD_ZERO(&help);
FD_SET(socket,&help);

if (seconds>0) 
	return select(socket+1,(fd_set*) 0,&help, (fd_set*) 0, &tm);
else
	return select(socket+1,(fd_set*) 0,&help, (fd_set*) 0, NULL);
}

/******************************************************************************/
