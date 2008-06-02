#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __TCPIPHEADER
#define __TCPIPHEADER

#if defined(OSF1) || defined(ULTRIX)
# include <sys/types.h>          /* network access ... */
# include <netinet/in.h>
# include <netdb.h>
# include <sys/socket.h>         /* sockets ... */
# include <fcntl.h>
# include <errno.h>              /* Error handling */
# include <sys/ioctl.h>		 /* for taping */
# if defined(LINUX)
   int xargc;			 /* for g77 */
   char **xargv;
   int __argc_save;		 /* for pgf77 */
   char** __argv_save;
#  if !defined(LIBC6) 
#   include <linux/time.h> 	 /* You will need this for libc5-Systems */
#  endif
# endif
#elif defined(VMS)
# include <ctype.h>
# include <socket.h>
# include <types.h>
# include <in.h>
# include <netdb.h>
# include <file.h>
#endif

#if defined(VMS)
/************************************************************
 *                             MACROS_BITOP.H
 *
 * description:    defines a few macros for bit-operations 
 * 		   necessary for select() in tapas_testana.c
 *
 *
 * author:         Martin Appenheimer (PIG)
 * date:           12.11.93
 *************************************************************/


#define MAX_NOFILE      4096    		/* This should not exist ! */
#define FD_SETSIZE      MAX_NOFILE

typedef long    fd_mask;

#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif /* howmany */

typedef struct fd_set {
        fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#define NBBY 8
#define NFDBITS (sizeof(fd_mask) *NBBY)         /* bits per mask */
#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)      memset((char *)(p) ,0 , sizeof(*(p)))
#endif

				/* tcp errors */
#define TCP_OK		   0
#define TCP_ERROR	  -1
#define TCP_TIMEOUT	 101
#define TCP_INTERNAL	 102
#define TCP_UNKNOWN_HOST 103
#define TCP_NODTP	 104
#define TCP_NOACP	 105

int    netwrite(int socket, char* buffer, int length, int size);
int    netread(int socket, char* buffer, int length, int size);
void   htons_a(short* buffer, int length);
void   ntohs_a(short* buffer, int length);
void   htonl_a(int* buffer, int length);
void   ntohl_a(int* buffer, int length);
int    install_listen(int port, int noblock);
int    do_connect(char* host, int port, int noblock);
struct sockaddr_in host2addr(char* host, int port);
char*  addr2host(struct sockaddr_in* address, int* port);
char*  addr2hostnew(struct sockaddr_in* address, int* port, char* remote);
int sendbuffer(int socket, int* buffer);
int getbuffer(int socket, int* buffer);

#endif 


#ifdef __cplusplus
}
#endif
