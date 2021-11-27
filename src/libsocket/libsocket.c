#include<poll.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/poll.h>
#include<arpa/inet.h>
#include"libsocket.h"
#include<netinet/in.h>
#include<sys/socket.h>
struct LibsocketSocket libsocket_socket_init(int port){struct LibsocketSocket new_socket={0};new_socket.fd=socket(AF_INET,SOCK_STREAM,0);new_socket.address.sin_family=AF_INET;new_socket.address.sin_port=htons(port);if(new_socket.fd==-1){fprintf(stderr,"libsocket_socket_init:could not initialize socket(%s)\n",strerror(errno));exit(EXIT_FAILURE);}return new_socket;}void libsocket_socket_bind(struct LibsocketSocket*socket_container,int queue){socket_container->address.sin_addr.s_addr=htonl(INADDR_ANY);if(bind(socket_container->fd,(struct sockaddr*)&socket_container->address,sizeof(socket_container->address))==-1){fprintf(stderr,"libsocket_server_bind:could not bind to socket(%s)\n",strerror(errno));exit(EXIT_FAILURE);}listen(socket_container->fd,queue);}void libsocket_socket_connect(struct LibsocketSocket*socket_container,const char*address){int status=0;if(inet_pton(AF_INET,address,&socket_container->address.sin_addr)<=0){fprintf(stderr,"libsocket_socket_connect:failed to generate address '%s'(%s)\n",address,strerror(errno));exit(EXIT_FAILURE);}status=connect(socket_container->fd,(struct sockaddr*)&socket_container->address,sizeof(socket_container->address));if(status < 0){fprintf(stderr,"libsocket_socket_connect:failed to connect to address '%s'(%s)\n",address,strerror(errno));exit(EXIT_FAILURE);}}int libsocket_socket_accept(struct LibsocketSocket*socket_container,int timeout,void(*on_accept)(struct LibsocketSocket*socket_container,int connector)){int connection_fd=0;struct pollfd descriptor={0};descriptor.events=POLLRDNORM;descriptor.fd=socket_container->fd;if(poll(&descriptor,1,timeout)<=0){return-1;}if((descriptor.revents&POLLRDNORM)==0){return-1;}connection_fd=accept(socket_container->fd,0,0);if(on_accept!=0){on_accept(socket_container,connection_fd);}return connection_fd;}int libsocket_socket_has_input(int descriptor,int timeout){struct pollfd target={0};target.events=POLLIN;target.fd=descriptor;if(poll(&target,1,timeout)<=0){printf("no events\n");return 0;}if((target.revents&POLLIN)==0){printf("incorrect event\n");return 0;}return 1;}void libsocket_socket_set_data(struct LibsocketSocket*socket_container,void*metadata){socket_container->metadata=metadata;}