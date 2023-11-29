#include "sender.h"

int UDPSender::connect__(std::string dst_ip, int port)
{
	client_socket = socket(PF_INET, SOCK_DGRAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str());
	server_addr.sin_port = htons(port);
	return 0;
}

int UDPSender::send__(std::string id, int len)
{
	std::string payload = id;
	payload.append(len - 36, '1');

	set_timestamp(id, SL_log);
	int bytes_sent = sendto(client_socket, payload.c_str(), payload.size(), 0,
							(struct sockaddr *)&server_addr, sizeof(server_addr));
	set_timestamp(id, SR_log);
	return bytes_sent;
}

void UDPSender::disconnect__()
{
	close(client_socket);
}