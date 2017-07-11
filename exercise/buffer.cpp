/*
 * Filename     : buffer.cpp
 * Author       : kiwiyan
 * Date         : 2017/07
 * Description  : Implementation of `buffer.h'
 */

#include <buffer.h>
#include <string.h>
#include <errno.h>          // for `errno`

namespace kiwi {

size_t Buffer::_DEFAULT_SIZE = 4096;

Buffer::Buffer(size_t size) :
    _M_bind(false)
{
    _M_size = (size == (size_t)-1)? _DEFAULT_SIZE: size;
    _M_data = (_Buffer *)new byte_t[_M_size];
    _M_head = _M_tail = _M_data->_data;
}

Buffer::~Buffer()
{
    if (!_M_bind) {
        delete[] (byte_t *)_M_data;
    }
}

Buffer::Buffer(void *mem, size_t size) :
    _M_bind(true)
{
    _M_size = size;
    _M_data = (_Buffer *)mem;
    _M_head = _M_tail = _M_data->_data;
}

} // namespace kiwi
