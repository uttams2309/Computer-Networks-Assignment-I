/* f20170683@hyderabad.bits-pilani.ac.in uttam singh */
/*program to extract logo and web page by implementing sockets in C */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
void removeChar(char *str, unsigned int index) {
	char *src;
	for (src = str + index; *src != '\0'; *src = *(src + 1), ++src) ;
	*src = '\0';
}
void delchar(char *x, int a, int b)
{
	if ((a + b - 1) <= strlen(x))
	{
		strcpy(&x[b - 1], &x[a + b - 1]);
		puts(x);
	}
}
void extract_image(char* URL , char* P_IP , char* CREDS , char* LOGO , int PORT) {

	struct sockaddr_in server_address;
	int Socket;
	if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket Creation Failed :( \n");
		exit(1);
	}
	printf("Socket Creation Successful!\n");
	//memset(&server_address,0,sizeof(server_address));
	server_address.sin_port = htons(13128);
	server_address.sin_addr.s_addr = inet_addr("182.75.45.22");
	server_address.sin_family = AF_INET;
	if (connect(Socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		printf("Connection Failed :(\n");
		exit(1);
	}
	printf("Connected!!\n");
	char request[400] = {};
	sprintf(request, "GET http://%s/cc.gif HTTP/1.1\r\nProxy-Authorization:Basic %s\r\nConnection: close\r\n\r\n" , URL , CREDS);
	printf("Get REQUEST : %s", request);
	if (send(Socket, request, strlen(request), 0) < 0)
	{
		printf("Send request failed :(\n");
		exit(1);
	}
	printf("Now receiving\n");
	char temp_char, Reply[1];
	/*while(recv(Socket,Reply,1,0) > 0)
	{
		temp_char = Reply[0];
		printf("%c",temp_char);
	}
	close(Socket);*/
	int state = 0;
	FILE *gif_ptr;
	//FILE *html_ptr;
	//html_ptr = fopen("INDEX", "w+");
	gif_ptr = fopen(LOGO, "w+");
	while ( recv(Socket, Reply , 1, 0) > 0)
	{
		temp_char = Reply[0];
		if (state != 4) {
			//putchar(temp_char);
		}
		if (state == 0) {
			if (temp_char == '\r') {
				state = 1;
			} else {
				state = 0;
			}
		} else if (state == 1) {
			if (temp_char == '\n') {
				state = 2;
			} else {
				state = 0;
			}
		} else if (state == 2) {
			if (temp_char == '\r') {
				state = 3;
			} else {
				state = 0;
			}
		} else if (state == 3) {
			if (temp_char == '\n') {
				state = 4;
			} else {
				state = 0;
			}
		} else {
			//	if(c!='\r'&&c!='\n')
			fputc(temp_char, gif_ptr);
			//fputc(temp_char, html_ptr);
		}
	}
	fclose(gif_ptr);
	//fclose(html_ptr);
	close(Socket);
	return;
}
void extract_html(char* URL , char* P_IP , char* CREDS , char* INDEX , int PORT , int status) {

	struct sockaddr_in server_address;
	int Socket;
	if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket Creation Failed :( \n");
		exit(1);
	}
	printf("Socket Creation Successful!\n");
	//memset(&server_address,0,sizeof(server_address));
	server_address.sin_port = htons(13128);
	server_address.sin_addr.s_addr = inet_addr("182.75.45.22");
	server_address.sin_family = AF_INET;
	if (connect(Socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		printf("Connection Failed :(\n");
		exit(1);
	}
	printf("Connected!!\n");
	char request[400] = {};
	sprintf(request, "GET http://%s/ HTTP/1.1\r\nProxy-Authorization:Basic %s\r\nConnection: close\r\n\r\n" , URL , CREDS);
	printf("Get REQUEST : %s", request);
	if (send(Socket, request, strlen(request), 0) < 0)
	{
		printf("Send request failed :(\n");
		exit(1);
	}
	printf("Now receiving\n");
	char temp_char, Reply[1];
	/*while(recv(Socket,Reply,1,0) > 0)
	{
		temp_char = Reply[0];
		printf("%c",temp_char);
	}
	close(Socket);*/
	int state = 0;
	FILE *html_ptr;
	char sta[10000];
	int ptr = 0;
	int d = 1;
	html_ptr = fopen(INDEX, "w+");
	while ( recv(Socket, Reply , 1, 0) > 0)
	{
		temp_char = Reply[0];
		if (state != 4) {
			//putchar(temp_char);
			sta[ptr++] = temp_char;
			//putchar(temp_char);
		}
		if (state == 0) {
			if (temp_char == '\r') {
				state = 1;
			} else {
				state = 0;
			}
		} else if (state == 1) {
			if (temp_char == '\n') {
				state = 2;
			} else {
				state = 0;
			}
		} else if (state == 2) {
			if (temp_char == '\r') {
				state = 3;
			} else {
				state = 0;
			}
		} else if (state == 3) {
			if (temp_char == '\n') {
				state = 4;
			} else {
				state = 0;
			}
		} else {
			//	if(c!='\r'&&c!='\n')
			fputc(temp_char, html_ptr);
			//fputc(temp_char, html_ptr);
		}
	}
	//puts(sta);
	//printf("hell");


	if (sta[9] == '2') {
		status = 1;
		return;
	}
	else if (sta[9] == '3') {
		status = 2;
		printf("redirecting\n");
	}
	else if (sta[9] == '4' || sta[9] == '5') {
		printf("error\n");
		return;
	}




	char *redir;


	char* loc = "Location";

	redir = strstr(sta, loc);
	redir += 17;
	//puts(redir);
	char *token = strtok(redir, "\n");
	/*int n = strlen(redir);
	int count = 0;;
	char *newURL;
	while (redir[count] != '\n' || redir[count] != '\r') {
		newURL[count] = redir[count];
	}*/
	int n = strlen(token);
	/*removeChar(token, n - 1);*/
	token[n - 1] = '\0';
	puts(token);
	while (token[0] == '/') {
		token++;
	}
	//return;
	strcpy(URL, token);
	//printf("%s\n", URL);
	//return;
	fclose(html_ptr);
	close(Socket);
	return extract_html(URL , P_IP , CREDS , INDEX , PORT , status);


}
int main(int argc, char *argv[])
{

	//bool status = false;
	char URL[strlen(argv[1])], P_IP[strlen(argv[2])], USER[strlen(argv[4])], PASS[strlen(argv[5])], INDEX[(strlen(argv[6]))], LOGO[(strlen(argv[7]))];
	strcpy(URL, argv[1]);
	strcpy(P_IP, argv[2]);
	strcpy(USER, argv[4]);
	strcpy(PASS, argv[5]);
	strcpy(INDEX, argv[6]);
	strcpy(LOGO, argv[7]);

	int PORT = atoi(argv[3]);
	int status = 0;
	//char URL[]="google.com";
	//char domain_name[]="www.articleworld.com";
	//int port=13128;
	char CREDS[] = "Y3NmMzAzOmNzZjMwMw==";
	//char USER[]
	char to_extract_image_also[] = "info.in2p3.fr";

	if (strcmp(URL , to_extract_image_also ) == 0) {
		//extract_only_web_page(URL , P_IP , CREDS , INDEX , PORT);
		extract_image(URL , P_IP , CREDS , LOGO , PORT);
		extract_html(URL , P_IP , CREDS , INDEX , PORT , status);
	}
	else {

		extract_html(URL , P_IP , CREDS , INDEX , PORT , status);
	}

	return 0;
}
