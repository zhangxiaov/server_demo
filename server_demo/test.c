#include <stdio.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

int main3()
{
    int kq;
    int n;
    struct kevent event[1];
    struct timespec ts;
    FILE *filep;
    
    filep = fopen("/Users/zhangxinwei/IMG_0141.PNG", "r");
    if (!filep) exit(1);
    
    ts.tv_sec = 0; //设置ts
    ts.tv_nsec = 0;
    
    // 做一个event.
    EV_SET( &event[0], fileno(filep), EVFILT_VNODE,
           EV_ADD,
           NOTE_DELETE | NOTE_ATTRIB | NOTE_WRITE, 0, 0);
    
    //产生一个queue
    if ( (kq=kqueue()) < 0) exit(2);
    
    //在queue中注册event
    if ( (n = kevent(kq, event, 1, NULL, 0, &ts) < 0))
        exit(3);
        
    //ts为NULL, 相当于wait吧, 要poll的话可以还用之前的ts
    n = kevent(kq, NULL, 0, event, 1, NULL);
    if ( n == 0 ) {
        printf("Nothing to do with it\n");
    } else {
        if (event->filter == EVFILT_VNODE && event->fflags == NOTE_DELETE) {
            printf("deleting...\n");
        }
        else if (event->filter == EVFILT_VNODE && event->fflags == NOTE_ATTRIB) {
            printf("attribution changed..\n");
        }
        else if (event->filter == EVFILT_VNODE && event->fflags == NOTE_WRITE) {
            printf("write to it..\n");
        }
    }
    
    return 0;
}