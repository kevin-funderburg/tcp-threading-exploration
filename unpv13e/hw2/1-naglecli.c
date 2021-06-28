#include	"unp.h"

int
main(int argc, char **argv)
{
    printf("...starting client...\n");

	int					sockfd;
	struct sockaddr_in	servaddr;

    char sendline[MAXLINE], recvline[MAXLINE];

    FILE *file = fopen("input.txt", "r");

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    printf("enter text to send to server, to quit type 'exit'\n");

    int loop = 1;
    while (loop)
    {
        bzero(sendline, MAXLINE);
        int n = 0;
        while ((sendline[n++] = getchar()) != '\n') //get text from command line
            ;

        //printf("sendline: %s", sendline);
        if (strncmp(sendline, "exit", 4) == 0) {
            printf("exiting\n"); 
            loop = 0;
        }
        Writen(sockfd, sendline, strlen(sendline));

        if (Readline(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");

        printf("recvline is: %s\n", recvline);
        bzero(recvline, MAXLINE);
    }
    
    //this method was to read from the input file and send to server, having some issue receiving though
    //while(fscanf(file, "%[^\n] ", sendline) != EOF) {
    //    printf("SENDING TO SERVER: %s\n", sendline); 
    //    Writen(sockfd, sendline, strlen(sendline));

    //    if (Readline(sockfd, recvline, MAXLINE) == 0)
    //        err_quit("str_cli: server terminated prematurely");

    //    printf("recvline is: %s\n", recvline);
    //}	

    exit(0);
}
