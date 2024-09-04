#include "Server.hpp"

int	Server::_prepairingPoll() {
	_fds.clear();
	pollfd	poll;

	for (std::map<int, Listen>::iterator l = _listens.begin();
	l != _listens.end(); ++l) {
		_fds.push_back(poll);
		_fds.back().fd = l->first;
		_fds.back().events = l->second.getEvent();
	}
	for (std::map<int, Connection>::iterator c = _connects.begin();
	c != _connects.end(); ++c) {
		_fds.push_back(poll);
		_fds.back().fd = c->first;
		_fds.back().events = c->second.getEvent();
		if (c->second.getConnectionStat() == CGIREAD) {
			_fds.push_back(poll);
			_fds.back().fd = c->second.getCgiReadFd();
			_fds.back().events = c->second.getCgiReadEvent();
		} else if (c->second.getConnectionStat() == CGIWRITE) {
			_fds.push_back(poll);
			_fds.back().fd = c->second.getCgiWriteFd();
			_fds.back().events = c->second.getCgiWriteEvent();
		} 
	}
	return 0;
}
/*
int	Server::_prepairingPoll() {
	std::vector<pollfd*> fds;
	pollfd	*p;

	for (std::map<int, Listen>::iterator l = _listens.begin();
	l != _listens.end(); ++l) {
		p = new pollfd;
		p->fd = l->first;
		p->events = l->second.getEvent();
		p->revents = l->second.getRevent();
		fds.push_back(p);
	}
	for (std::map<int, Connection>::iterator c = _connects.begin();
	c != _connects.end(); ++c) {
		p = new pollfd;
		p->fd = c->first;
		p->events = c->second.getEvent();
		p->revents = c->second.getRevent();
		fds.push_back(p);
		if (c->second.getConnectionStat() == CGIREAD) {
			p = new pollfd;
			p->fd = c->second.getCgiReadFd();
			p->events = c->second.getCgiReadEvent();
			p->revents = c->second.getCgiReadRevent();
			fds.push_back(p);
		} else if (c->second.getConnectionStat() == CGIWRITE) {
			p = new pollfd;
			p->fd = c->second.getCgiWriteFd();
			p->events = c->second.getCgiWriteEvent();
			p->revents = c->second.getCgiWriteRevent();
			fds.push_back(p);
		} 
	}
	return 0;
}
*/