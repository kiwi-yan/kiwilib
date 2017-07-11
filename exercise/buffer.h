#ifndef __KIWI_BUFFER_H__
#define __KIWI_BUFFER_H__    1

/*
 * Filename     : buffer.h
 * Author       : kiwiyan
 * Date         : 2017/07
 * Description  : Memory buffer.
 */
#include <unistd.h>         // for `close()`

typedef unsigned char   byte_t;

namespace kiwi {

// Classname: Buffer
class Buffer
{
public:
    // Default Constructor
    Buffer(size_t size = (size_t)-1);
    ~Buffer();
    Buffer(void *mem, size_t size);
    
    
private:
    const bool _M_bind;
    size_t _M_size;
    static size_t _DEFAULT_SIZE;
    
    struct _Buffer {
        byte_t _data[0];
    };
    _Buffer* _M_data;
    byte_t* _M_head;
    byte_t* _M_tail;
};


} // namespace kiwi

#endif // __KIWI_EPOLL_H__