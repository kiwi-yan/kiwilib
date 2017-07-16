/*
 * Filename     : buffer.cpp
 * Author       : kiwiyan
 * Date         : 2017/07
 * Description  : Implementation of `buffer.h'
 */

#include <buffer.h>
#include <cstring>
// #include <cassert>          // for `assert`

namespace kiwi {

size_t Buffer::_DEFAULT_SIZE = 4096;

Buffer::Buffer(size_t len) :
    _M_capacity((len == (size_t)-1)? _DEFAULT_SIZE: len),
    _M_bind(false), _M_size(0)
{
    _M_data = (_Buffer *)new byte_t[_M_capacity];
    _M_head = _M_tail = _M_data->_data;
    _M_start = _M_data->_data;
    _M_end = _M_start + _M_capacity;
}

Buffer::~Buffer()
{
    if (!_M_bind) {
        delete[] (byte_t *)_M_data;
    }
}

Buffer::Buffer(void *mem, size_t len) :
    _M_capacity(len), _M_bind(true), _M_size(0)
{
    _M_data = (_Buffer *)mem;
    _M_head = _M_tail = _M_data->_data;
    _M_start = _M_data->_data;
    _M_end = _M_start + _M_capacity;
}

size_t
Buffer::put(const void *data, size_t len, bool strict/* = false */)
{
    if (len > remain()) {
        if (!strict) {
            size_t copy_len = remain();
            _put(data, copy_len);
            return copy_len;
        }
        return 0;
    }
    _put(data, len);
    return len;
}

size_t
Buffer::take(void *data, size_t len/* = 0 */)
{
    byte_t *ptr = (byte_t *)data;
    size_t copy_len = len;
    if (copy_len == 0 || copy_len >= _M_size) {
        copy_len = _M_size;
    }
    if (copy_len == 0) return 0;
    if (_M_head < _M_tail) {
        memcpy(ptr, _M_head, copy_len);
        _M_head += copy_len;
    }
    else {
        memcpy(ptr, _M_head, _M_end - _M_head);
        ptr += _M_end - _M_head;
        size_t remain_len = copy_len - (_M_end - _M_head);
        memcpy(ptr, _M_start, remain_len);
        _M_head = _M_start + remain_len;
    }
    _M_size -= copy_len;
    return copy_len;
}

size_t
Buffer::get(void *data, size_t len/* = 0 */)
{
    byte_t *ptr = (byte_t *)data;
    size_t copy_len = len;
    if (copy_len == 0 || copy_len >= _M_size) {
        copy_len = _M_size;
    }
    if (copy_len == 0) return 0;
    if (_M_head < _M_tail) {
        memcpy(ptr, _M_head, copy_len);
    }
    else {
        memcpy(ptr, _M_head, _M_end - _M_head);
        ptr += _M_end - _M_head;
        memcpy(ptr, _M_start, _M_tail - _M_start);
    }
    return copy_len;
}

void
Buffer::put_prepare(void *& ptr, size_t& len)
{
    ptr = _M_tail;
    if (remain() == 0) {
        len = 0;
        return;
    }
    if (_M_head <= _M_tail) {
        len = _M_end - _M_tail;
    }
    else {
        len = _M_head - _M_tail;
    }
}

int
Buffer::push_commit(size_t len)
{
    if (remain() == 0) return -1;
    if (_M_head <= _M_tail) {
        if (_M_end - _M_tail < len) return -1;
    }
    else {
        if (_M_head - _M_tail< len) return -1;
    }
    _M_tail += len;
    _M_size += len;
    return 0;
}

void
Buffer::reset()
{
    _M_head = _M_tail = _M_data->_data;
    _M_start = _M_data->_data;
    _M_end = _M_start + _M_capacity;
}

void
Buffer::_put(const void *data, size_t len)
{
    const byte_t *ptr = (const byte_t *)data;
    size_t remain_len = len;
    // assert(remain_len <= remain());
    
    if (_M_head <= _M_tail) {
copy_start:
        if (remain_len == 0) return;
        size_t copy_len = (_M_end - _M_tail < remain_len)?
                        _M_end - _M_tail: remain_len;
        memcpy(_M_tail, ptr, copy_len);
        _M_size += copy_len;
        _M_tail += copy_len;
        if (_M_tail == _M_end) _M_tail = _M_start;
        remain_len -= copy_len;
        ptr += copy_len;
        goto copy_start;
    }
    goto copy_start;
}

} // namespace kiwi
