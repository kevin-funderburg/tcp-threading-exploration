#include	"unpthread.h"

static void	*doit(void *);		/* each thread executes this function */

void
str_echo(int sockfd)
{
    printf("...str_echo()...\n");
    
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0) {
        printf("FROM CLIENT: %s\n", buf);
		Writen(sockfd, buf, n);
    }

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}

int
main(int argc, char **argv)
{
    printf("...starting server...\n\n");

	int				listenfd, connfd;
	pthread_t		tid;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: tcpserv01 [ <host> ] <service or port>");

	cliaddr = Malloc(addrlen);

	for ( ; ; ) {
		len = addrlen;
		connfd = Accept(listenfd, cliaddr, &len);
        printf("creating thread\n");
		Pthread_create(&tid, NULL, &doit, (void *) connfd);
	}
}

static void *
doit(void *arg)
{
    printf("...doit()...\n");
	Pthread_detach(pthread_self());
	str_echo((int) arg);	/* same function as before */
	Close((int) arg);		/* done with connected socket */
	return(NULL);
}


//static void *
//doit(void *arg)
//{
//    printf("...doit()...\n");
//    
//    int connfd;
//    connfd = *((int *) arg);
//    free(arg);
//
//	Pthread_detach(pthread_self());
//	str_echo(connfd);	/* same function as before */
//
//	Close(connfd);		/* done with connected socket */
//	return(NULL);
//}
