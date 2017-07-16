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
    Buffer(size_t len = (size_t)-1);
    
    // Destructor
    ~Buffer();
    
    // Constructor: bind an external memory as buffer.
    Buffer(void *mem, size_t len);
    
    // Return the capacity of this buffer.
    size_t capacity() const
    {
        return _M_capacity;
    }
    
    // Return the size used of this buffer. 
    size_t size() const
    {
        return _M_size;
    }
    
    // Put the data into buffer.
    // Return the number of bytes you put, len, or 0.
    size_t put(const void *data, size_t len, bool strict = false);
    
    // Take the data from buffer.
    // The parameter `len' is the number of bytes length you expecting to 
    // take. Return the actual number of bytes.
    size_t take(void *data, size_t len = 0);
    
    // Not remove the data from buffer, but only get data.
    size_t get(void *data, size_t len = 0);
    
    // Get the actual pointer to put data. 
    // The parameter `len' is the max number of bytes you could put.
    // Must match with `put_commit'.
    void put_prepare(void *& ptr, size_t& len);
    
    // Commit the data you put in buffer.
    int push_commit(size_t len);
    
    // Clear data and reset buffer.
    void reset();
    
protected:
    
private:
    inline size_t remain() const
    {
        return _M_capacity - _M_size;
    }
    
    void _put(const void *data, size_t len);
    
    const bool _M_bind;
    const size_t _M_capacity;
    size_t _M_size;
    static size_t _DEFAULT_SIZE;
    
    struct _Buffer {
        byte_t _data[0];
    };
    _Buffer *_M_data;
    byte_t *_M_head;
    byte_t *_M_tail;
    byte_t *_M_start;
    byte_t *_M_end;
};


} // namespace kiwi

#endif // __KIWI_BUFFER_H__