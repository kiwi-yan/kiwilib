#include <epoll.h>

/*class m: public kiwi::epoll
{
    
};*/
int main()
{
    kiwi::Epoll m;
    m.create();
    m.close();
}