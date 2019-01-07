#include <iostream>
#include "messagebuffer.hpp"
#include <ctime>

//#define DEBUG

Message::Message(char *text) {
    strcpy(this->text, text);
    this->next = NULL;
}

Message::~Message() {}

MessageBuffer::~MessageBuffer() {}

Message* MessageBuffer::pop() {
    enter();
    Message *tmp;
    
    if(head == NULL){
        std::time_t result = std::time(nullptr);
        std::cout << "RX " << result << ": Nie znalazlem wiadomosci" << std::endl;
        wait(cons);    
    }      
    
    tmp = head;
    if(pri_tail == head) pri_tail = NULL;
    head = head->next;
    buffer_occupancy--;

    std::time_t result = std::time(nullptr);
    std::cout << "\033[1;31m" << "RX " << result << ": " << tmp->text << "\033[0m" <<std::endl;
    delete tmp;
    
    signal(prod);
    leave();
}

void MessageBuffer::push(Message* m) {
    enter();
    
    if(buffer_occupancy == CAPACITY){
        std::time_t result = std::time(nullptr);
        std::cout << "TX " << result << ": BRAK MIEJSCA W BUFORZE" << std::endl;
        wait(prod);    
    }

    std::time_t result = std::time(nullptr);
    std::cout << "TX " << result << ": " << m->text << std::endl;    
    
    if(head) { //some msgs already exist
        tail->next = m;
        tail = m;
        m->next = NULL;
    } else { // empty buffer
        head = m;
        m->next = NULL;
        pri_tail = NULL;
        tail = m;
    }   
    buffer_occupancy++;
    signal(cons);
    leave();
}

void MessageBuffer::push_pri(Message *m){
    enter();
    if(buffer_occupancy == CAPACITY){
        std::time_t result = std::time(nullptr);
        std::cout << "PX " << result << ": BRAK MIEJSCA W BUFORZE" << std::endl;
        wait(prod);    
    } 

    std::time_t result = std::time(nullptr);
    std::cout << "PX " << result << ": " << m->text << std::endl;    
    
    if(head){ //some msgs already exists
        if(pri_tail){ //some pri_msgs already exists
            m->next = pri_tail->next;
            pri_tail->next = m;
            pri_tail = m;
        }
        else { //no pri msg
            m->next = head;
            head = m;
            pri_tail = m;
        }
    }
    else { // empty buffer
        head = m;
        m->next = NULL;
        pri_tail = m;
        tail = m;
    }
    buffer_occupancy++;
    signal(cons);
    leave();
}