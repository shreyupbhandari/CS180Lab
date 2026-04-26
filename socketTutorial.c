#include <sdtdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int create_IPv4_socket();
struct sockaddr_in * create_IPv4_address(char ip[], int port);
int main()
{
	int socket_fd=create_IPv4_socket();
	char ip[]="142.250.190.238";
	int port = 80;
	struct sockaddr_in *address = create_IPv4_address(ip, port);
	int result = connect(socket_fd, address, size(*address));

	if (result==0)
	{
		printf("Connection was successful!\n");
	} 
	else
	{
		printf("Connection failed!\n");	
	}
	return 0;
}

int create_IPv4_socket()
{

	return socket(AF_INET, SOCK_STREAM, 0/*default protocol*/);

}
struct sockaddr_in * create_IPv4_address(char ip[], int port)
{

	struct sock_addr_in * address = malloc(sizeof(struct sockaddr_in));
	address->sin_family = AF_INTET;
	address->sin_port=htons(port);
	


}
