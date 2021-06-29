#include	"unp.h"
#include	"unpthread.h"
// #include    "strclithread.c"

void	*copyto(void *);


static pthread_key_t	rl_key, diff_key;
static pthread_once_t	rl_once = PTHREAD_ONCE_INIT;
static pthread_once_t	diff_once = PTHREAD_ONCE_INIT;

static int	sockfd;		/* global for both threads to access */
static FILE	*fp;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//these functions were created to test the creation of a different 
//thread specific data item
//
static void
diff_destructor(void *ptr)
{
    free(ptr);
}

static void
different_once(void)
{
 	Pthread_key_create(&diff_key, diff_destructor);
}

typedef struct {
    char buff[MAXLINE];
} diff_data;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static void
readline_destructor(void *ptr)
{
	free(ptr);
}

static void
readline_once(void)
{
	Pthread_key_create(&rl_key, readline_destructor);
}

typedef struct {
  int	 rl_cnt;			/* initialize to 0 */
  char	*rl_bufptr;			/* initialize to rl_buf */
  char	 rl_buf[MAXLINE];
} Rline;
/* end readline1 */

/* include readline2 */
//ssize_t
static ssize_t
my_read(Rline *tsd, int fd, char *ptr)
{
    //printf("...my_read()...\n");
	if (tsd->rl_cnt <= 0) {
again:
		if ( (tsd->rl_cnt = read(fd, tsd->rl_buf, MAXLINE)) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (tsd->rl_cnt == 0) {
			return(0);
        }
		tsd->rl_bufptr = tsd->rl_buf;
	}

	tsd->rl_cnt--;
	*ptr = *tsd->rl_bufptr++;

	return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
    //printf("...readline()...\n");

	size_t		n, rc;
	char	c, *ptr;
	Rline	*tsd;

    //printf("\tpreparing to execute Pthread_once\n");
    //printf("rl_key before Pthread_once: %d\n", rl_key);

	Pthread_once(&rl_once, readline_once);
	if ( (tsd = pthread_getspecific(rl_key)) == NULL) {
        //since the value of the pointer at rl_key is NULL, we will malloc
        //the memory needed
		tsd = Calloc(1, sizeof(Rline));		/* init to 0 */
        //now we set the thread-specific data pointer for this key to point 
        //to the memory just allocated
		Pthread_setspecific(rl_key, tsd);
	}

    printf("rl_key: %d\n", rl_key);

    ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(tsd, fd, &c)) == 1) {
            *ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
            printf("EOF\n");
			*ptr = 0;
			return(n - 1);		/* EOF, n - 1 bytes read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;
    //printf("...done...\n");
	return(n);
}
/* end readline2 */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
    //printf("...Readline()...\n");

	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}

void
str_cli(FILE *fp_arg, int sockfd_arg)
{
    //printf("...str_cli()...\n\n");

	char		recvline[MAXLINE];
	pthread_t	tid;

	sockfd = sockfd_arg;	/* copy arguments to externals */
	fp = fp_arg;

    //printf("creating a thread to start in copyto()\n");
	Pthread_create(&tid, NULL, copyto, NULL);

	while (Readline(sockfd, recvline, MAXLINE) > 0) {
        //printf("recvline: %s\n", recvline);
		Fputs(recvline, stdout);
    }
}

void *
copyto(void *arg)
{
    printf("...copyto()...\n");

	char	sendline[MAXLINE];

	while (Fgets(sendline, MAXLINE, fp) != NULL) {
        //printf("SENDING TO SERVER: %s\n", sendline);
		Writen(sockfd, sendline, strlen(sendline));
    }
    printf("about to shutdown\n");
	Shutdown(sockfd, SHUT_WR);	/* EOF on stdin, send FIN */

	return(NULL);
		/* 4return (i.e., thread terminates) when EOF on stdin */
}


int
main(int argc, char **argv)
{
    printf("...starting client...\n");

	diff_data	*diff_tsd;

	int		sockfd;

	if (argc != 3)
		err_quit("usage: tcpcli <hostname> <service>");

	sockfd = Tcp_connect(argv[1], argv[2]);

    //////////////////////////////////////////////////////////////////////
    //this is an attempt to make another thread specific data item 
    //using a different key than the rl_key
    //
    //this thread specific data item will not perform any function, it
    //will just contain a string for testing purposes
    //
    Pthread_once(&diff_once, different_once);
	if ( (diff_tsd = pthread_getspecific(diff_key)) == NULL) {
        //since the value of the pointer at diff_key is NULL, we will malloc
        //the memory needed
		diff_tsd = Calloc(1, sizeof(diff_data));		/* init to 0 */
        //now we set the thread-specific data pointer for this key to point 
        //to the memory just allocated
		Pthread_setspecific(diff_key, diff_tsd);
	}

    printf("diff_key: %d\n", diff_key);
    //setting the value of the second thread specific data item
    strcpy(diff_tsd->buff, "a different string");
    //////////////////////////////////////////////////////////////////////

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
