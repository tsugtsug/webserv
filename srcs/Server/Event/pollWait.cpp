#include "Server.hpp"

// static void	fdsPrint(const std::vector<pollfd> &fds) {
// 	std::cout << "fdの各reventsの状態\n";
// 	for (std::vector<pollfd>::const_iterator f = fds.begin();
// 	f != fds.end(); ++f) {
// 		std::cout << f->fd << " : " << f->events << " : " << f->revents << std::endl;
// 	}
// }

int	Server::_pollWait() {
	int		ret;

	//std::cout << "poll size: " << _fds.size() << std::endl;

	ret = poll(_fds.data(), _fds.size(), 0);


	if (ret < 0) {
		if (errno == EINTR) {
			return -1;
		}
		errMessage("poll");
		return -1;
	// } else if (ret == 0) {
	// 	std::cout << "Wait..." << std::endl;
	}
	this->_updateSocketStatus(ret);
	//this->_updateSocketStatus();
	return ret;
}

int	Server::_updateSocketStatus(int count) {
//int	Server::_updateSocketStatus() {
	if (count == 0) { return 0;}
	std::vector<pollfd>::iterator f = _fds.begin();
	for (std::map<int, Listen>::iterator l = _listens.begin(); l != _listens.end(); ++l) {
		l->second.setRevent(f->revents);
		++f;
	}
	for (std::map<int, Connection>::iterator c = _connects.begin();c != _connects.end(); ++c) {
		c->second.setRevent(f->revents);
		if (c->second.getConnectionStat() == CGIREAD) {
			++f;
			c->second.setCgiReadRevent(f->revents);
		} else if (c->second.getConnectionStat() == CGIWRITE) {
			++f;
			c->second.setCgiWriteRevent(f->revents);
		} 
		++f;
	}
/*
	for (std::vector<pollfd>::iterator p = _fds.begin(); p != _fds.end(); ++p) {
		std::map<int, Listen>::iterator l = _listens.begin();
		for (; l != _listens.end(); ++l) {
			if (p->fd == l->first) {
				l->second.setRevent(p->revents);
				break;
			}
		}
		if (l != _listens.end()) {
			continue;
		}
		for (std::map<int, Connection>::iterator c = _connects.begin();
		c != _connects.end(); ++c) {
			if (p->fd == c->first) {
				c->second.setRevent(p->revents);
				break;
			} else if (p->fd == c->second.getCgiReadFd()) {
				c->second.setCgiReadRevent(p->revents);
				break;
			} else if (p->fd == c->second.getCgiWriteFd()) {
				c->second.setCgiWriteRevent(p->revents);
				break;
			}
		}
	}
*/
	return 0;
}