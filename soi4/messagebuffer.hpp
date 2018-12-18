#ifndef __messagebuffer_hpp_
#define __messagebuffer_hpp_

#include"monitor.hpp"

#define CAPACITY 20

class Message {
    public:
        char text[128];
        Message *next;
        Message(char *text);
        ~Message();
};

class MessageBuffer : private Monitor {
    private:
        Message *head, *pri_tail, *tail;
        Condition prod, cons;
        int buffer_occupancy;

    public:
        MessageBuffer()  {
            head = pri_tail = tail = NULL;
            buffer_occupancy = 0;
        };
        ~MessageBuffer();
        
        Message* pop();
        void push(Message *m);
        void push_pri(Message *m);
};


#endif //__messagebuffer_hpp_