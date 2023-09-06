#include "Server.h"

//==== config ====//
int domain = AF_INET,
	service = SOCK_STREAM,
	protocol = 0,
	port = 85,
	backlog = 16,
	maxUsers = 256,
	maxLobbies = 32;
const char* host = "192.168.0.103";
std::string logPath = "./server.txt";
//================//

int main(int argc, char** argv)
{
	Andrew::FileSharingServer server(
		domain,
		service,
		protocol,
		port,
		host,
		backlog,
		logPath,
		maxUsers,
		maxLobbies
	);

	return 0;
}
