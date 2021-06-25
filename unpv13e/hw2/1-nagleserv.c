#include	"unp.h"
#include    <netinet/tcp.h>

int
main(int argc, char **argv)
{
    printf("...starting server...\n");

	int					listenfd, connfd, nodelay, len;
	//pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    len = sizeof(nodelay);
    Getsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, &nodelay, &len);
    printf("defaults: TCP_NODELAY = %d\n", nodelay);

    nodelay = 1;
    Setsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
    len = sizeof(nodelay);
    Getsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, &nodelay, &len);
    printf("TCP_NODELAY = %d (after setting to 1)\n", nodelay);


	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

        Close(listenfd);	/* close listening socket */
        str_echo(connfd);	/* process the request */
		Close(connfd);	
        exit(0);
	}
}
