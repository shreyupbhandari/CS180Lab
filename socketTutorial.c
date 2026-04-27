#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define RESPONSE_BUFFER_SIZE 4096

int create_IPv4_socket();
struct sockaddr_in * create_IPv4_address(char ip[], int port);
int main()
{
	int socket_fd=create_IPv4_socket();
	char ip[]="142.250.190.238";
	int port = 80;
	struct sockaddr_in *address = create_IPv4_address(ip, port);
	int result = connect(socket_fd, (struct sockaddr *) address, sizeof(*address));

	if (result==0)
	{
		printf("Connection was successful!\n");
	} 
	else
	{
		printf("Connection failed!\n");	
	}

	char *message;
	message="GET/ HTTP/1.1\r\nHost: www.google.com\r\n\r\n";

	ssize_t message_size=send(socket_fd,message,strlen(message),0);
	printf("Sent %ldb request.\n",message_size);
	
	char buffer[RESPONSE_BUFFER_SIZE];
	memset(buffer,'\0',RESPONSE_BUFFER_SIZE);

	ssize_t response_size= recv(socket_fd, buffer, RESPONSE_BUFFER_SIZE, 0);
	printf("Received  %ldb response.\n%s\n",response_size,buffer);

	return 0;
}

int create_IPv4_socket()
{

	return socket(AF_INET, SOCK_STREAM, 0/*default protocol*/);

}
struct sockaddr_in * create_IPv4_address(char ip[], int port)
{

	struct sockaddr_in * address = malloc(sizeof(struct sockaddr_in));
	address->sin_family = AF_INET;
	address->sin_port=htons(port);
	if (strlen(ip)==0)
	{
		address->sin_addr.s_addr=INADDR_ANY;
	}
	else
	{
		inet_pton(AF_INET,ip,&address->sin_addr.s_addr);
	}

	return address;
	
}
