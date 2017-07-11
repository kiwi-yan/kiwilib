#ifndef __KIWI_EPOLL_H__
#define __KIWI_EPOLL_H__    1

/*
 * Filename     : epoll.h
 * Author       : kiwiyan
 * Date         : 2017/07
 * Description  : Inspired by Python's select module
 */
#include <unistd.h>         // for `close()`
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/select.h>

#if !defined(FD_SETSIZE)
#define FD_SETSIZE      512
#endif

namespace kiwi {

// Classname: Epoll
class Epoll
{
public:
    // Default Constructor
    Epoll();
    // Destructor
    virtual ~Epoll();

    // Create the epoll control file descriptor and allocate space for saving
    // events.
    int create(int sizehint = FD_SETSIZE - 1);

    // Close the epoll control file descriptor.
    void close()
    {
        if (_M_epfd >= 0) {
            ::close(_M_epfd);
        }
    }

    // Return the epoll control file descriptor.
    int fileno() const
    {
        return _M_epfd;
    }

    // Registers a new fd.
    int add(int fd, unsigned int events);

    // Modify a fd which is already registered.
    int modify(int fd, unsigned int events);

    // Remove a fd which is already registered.
    int remove(int fd);

    // Wait for events on the epoll file descriptor for a maximum time of
    // timeout in milliseconds. -1 will wait indefinitely.
    int wait(int timeout = -1);

    // Get the events that saved in `wait()'.
    int get_events(int& fd, unsigned int& events);
private:
    int _M_epfd;
    int _M_size;
    int _M_event_num;
    int _M_event_index;
    epoll_event* _M_events;

    int control(int op, int fd, unsigned int events);
};


} // namespace kiwi

#endif // __KIWI_EPOLL_H__