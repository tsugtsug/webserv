#include "Server.hpp"

int	Server::_inputSection(int fd, Connection &c) {
	int	ret;

	if (c.getRequestInfo().getBuf().empty()) {
		c.setConnectingTime(std::time(NULL));
	}

	ret = _Recv(fd, c);
	// _Recv関数でrecvを失敗している。recvの失敗はサーバー自体の落ち度のため、500
	if (ret <= 0) {
		return 0;
	}
	// 429 Too Many Requests クライアントの接続の上限を超えた場合にエラーを返し、closeする
	// if (_connects.size() > ) {
	// 	c.setConnectionStat(ERROR);
	// 	c.setErrStatus(429);
	// }

	//std::cout << c.getRequestInfo().getBuf().substr(0, c.getRequestInfo().getBuf().find("\r\n\r\n")) << std::endl;


	_Parse(c);

	_Method(c);

	_CgiExec(c);

	_Redirect(c);


	_Error(c);

	//この時点で各イベント関数でeventをPOLLINにstatusをSENDにしておく。

	return 0;
}