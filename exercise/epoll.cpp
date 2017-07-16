/*
 * Filename     : epoll.cpp
 * Author       : kiwiyan
 * Date         : 2017/07
 * Description  : Implementation of `epoll.h'
 */

#include <epoll.h>

#include <cerrno>          // for `errno`

namespace kiwi {

Epoll::Epoll() :
    _M_epfd(-1), _M_size(-1), _M_event_num(-1),
    _M_event_index(0), _M_events(NULL)
{ }

Epoll::~Epoll()
{
    close();
    if (_M_events) {
        delete[] _M_events;
    }
}

int
Epoll::create(int sizehint/* = FD_SETSIZE - 1 */)
{
    _M_epfd = epoll_create(sizehint);
    if (_M_epfd < 0) {
        return -1;
    }
    _M_size = sizehint;
    _M_events = new epoll_event[sizehint];
    return 0;
}

int
Epoll::control(int op, int fd, unsigned int events)
{
    if (_M_epfd < 0 || fd < 0) {
        errno = EINVAL;
        return -1;
    }
    epoll_event ev;
    int ret = 0;

    switch (op) {
    case EPOLL_CTL_ADD:
    case EPOLL_CTL_MOD:
        ev.events = events;
        ev.data.fd = fd;
        ret = epoll_ctl(_M_epfd, op, fd, &ev);
        break;
    case EPOLL_CTL_DEL:
        ret = epoll_ctl(_M_epfd, op, fd, &ev);
        if (errno == EBADF) {
            // fd already closed
            ret = 0;
            errno = 0;
        }
        break;
    default:
        ret = -1;
        errno = EINVAL;
    }
    return ret;
}

int
Epoll::add(int fd, unsigned int events)
{
    return control(EPOLL_CTL_ADD, fd, events);
}

int
Epoll::modify(int fd, unsigned int events)
{
    return control(EPOLL_CTL_MOD, fd, events);
}

int
Epoll::remove(int fd)
{
    return control(EPOLL_CTL_DEL, fd, 0);
}

int
Epoll::wait(int timeout)
{
    if (_M_epfd < 0 || _M_events == NULL) {
        errno = EINVAL;
        return -1;
    }
    _M_event_num = epoll_wait(_M_epfd, _M_events, _M_size, timeout);
    _M_event_index = 0;
    return _M_event_num;
}

int
Epoll::get_events(int& fd, unsigned int& events)
{
    if (_M_event_index >= _M_event_num) {
        return 0;
    }

	epoll_event *current_event = &_M_events[_M_event_index++];
	fd = current_event->data.fd;
	events = current_event->events;
	return 1;
}

} // namespace kiwi
