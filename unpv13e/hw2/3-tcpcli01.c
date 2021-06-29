#include	"unp.h"
#include	"unpthread.h"
// #include    "strclithread.c"

void	*copyto(void *);


static pthread_key_t	rl_key;
static pthread_once_t	rl_once = PTHREAD_ONCE_INIT;

static int	sockfd;		/* global for both threads to access */
static FILE	*fp;


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
static ssize_t
my_read(Rline *tsd, int fd, char *ptr)
{
    printf("...my_read()...\n");

    //strcpy(tsd->rl_buf, "a test line");
    //tsd->rl_cnt = 1;
    //tsd->rl_bufptr = tsd->rl_buf;
    
    printf("tsd->rl_cnt: %d\n", tsd->rl_cnt);
    printf("tsd->rl_buf: %s\n", tsd->rl_buf);

	if (tsd->rl_cnt <= 0) {
        printf("tsd->rl_cnt <= 0\n");
again:
		if ( (tsd->rl_cnt = read(fd, tsd->rl_buf, MAXLINE)) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (tsd->rl_cnt == 0)
            printf("==>BREAKPOINT==>\n");
			return(0);
		tsd->rl_bufptr = tsd->rl_buf;
	}

	tsd->rl_cnt--;
	*ptr = *tsd->rl_bufptr++;

    printf("...done...\n");
	return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
    printf("...readline()...\n");

	size_t		n, rc;
	char	c, *ptr;
	Rline	*tsd;

    printf("\tpreparing to execute Pthread_once\n");
    printf("rl_key before Pthread_once: %d\n", rl_key);

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
    printf("...done...\n");
	return(n);
}
/* end readline2 */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
    printf("...Readline()...\n");

	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}

//void
//str_cli(FILE *fp, int sockfd)
//{
//    char sendline[MAXLINE], recvline[MAXLINE];
//
//    while (Fgets(sendline, MAXLINE, fp) != NULL) {
//
//        Writen(sockfd, sendline, strlen(sendline));
//
//        if (Readline(sockfd, recvline, MAXLINE) == 0)
//            err_quit("str_cli: server terminated prematurely");
//
//        Fputs(recvline, stdout);
//    }
//}

void
str_cli(FILE *fp_arg, int sockfd_arg)
{
    printf("...str_cli()...\n\n");

	char		recvline[MAXLINE];
	pthread_t	tid;

	sockfd = sockfd_arg;	/* copy arguments to externals */
	fp = fp_arg;

    printf("creating a thread to start in copto()\n");
	Pthread_create(&tid, NULL, copyto, NULL);

	while (Readline(sockfd, recvline, MAXLINE) > 0)
		Fputs(recvline, stdout);
}

void *
copyto(void *arg)
{
    printf("...copyto()...\n");

	char	sendline[MAXLINE];

    printf("should stop here for input\n");
	while (Fgets(sendline, MAXLINE, fp) != NULL) {
        printf("in while loop\n");
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

	int		sockfd;

	if (argc != 3)
		err_quit("usage: tcpcli <hostname> <service>");

	sockfd = Tcp_connect(argv[1], argv[2]);

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
