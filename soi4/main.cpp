#include<iostream>
#include<unistd.h>
#include<ctime>
#include "messagebuffer.hpp"

// #define DEBUG
MessageBuffer *mb;

void *consumer(void* num){
    long tid = (long)num;
    for(int i = 0; i < 1000; i++){
        mb->pop();
        usleep(3000000);
    }
}

void *producer(void *num){
    Message *new_msg;
    char msg_content[128];
    long tid = (long)num;
    for(int i = 0; i < 100; i++){
        sprintf(msg_content, "Message number %d from %d thread ", i, (int*)num);
        new_msg = new Message(msg_content);
        mb->push(new_msg);

        sleep(1);
    }
}

void *privileged_producer(void *num){
    Message *new_msg;
    char msg_content[128];
    long tid = (long)num;
    for(int i = 0; i < 100; i++){
        sprintf(msg_content, "Priority message number %d from producer %d", i, (int*)num);
        new_msg = new Message(msg_content);
        mb->push_pri(new_msg);

        sleep(2);
    }
}

int main(int argc, char** argv){
    mb = new MessageBuffer();

    if(argc != 2 || *argv[1] == 'h') {
        std::cout << "Systemy operacyjne - laboratorium" << std::endl;
        std::cout << "Autor: Jakub Sikora" << std::endl;
        std::cout << "Aby uruchomic dany test uruchom program z odpowiednim argumentem wywolania" << std::endl;
        std::cout << "0. Problem zachowania kolejnosci" << std::endl;
        std::cout << "1. Problem pelnego bufora" << std::endl;
        std::cout << "2. Problem pustego bufora" << std::endl;
        std::cout << "3. Problem wiadomosci priorytetowych" << std::endl;
        exit(0);
    }

    if(*argv[1] == '0') {
        pthread_t cons, prod0, prod1, prod2;

        pthread_create(&cons, NULL, consumer, &mb);
        usleep(100000);

        pthread_create(&prod0, NULL, producer, (void*)0L);
        usleep(100000);
        
        pthread_create(&prod1, NULL, producer, (void*)1L);
        usleep(100000);
        
        pthread_create(&prod2, NULL, producer, (void*)2L);
        usleep(100000);

        pthread_join(cons, NULL);
    }

    if(*argv[1] == '1'){
        pthread_t prod0, prod1, prod2, prod3, prod4;
        
        pthread_create(&prod0, NULL, producer, (void*)0L);
        usleep(100000);

        pthread_create(&prod1, NULL, producer, (void*)1L);
        usleep(100000);

        pthread_create(&prod2, NULL, producer, (void*)2L);
        usleep(100000);

        pthread_create(&prod3, NULL, producer, (void*)3L);
        usleep(100000);

        pthread_create(&prod4, NULL, producer, (void*)4L);
        
        pthread_join(prod0, NULL);
    }

    if(*argv[1] == '2'){
        pthread_t cons0, cons1;
        
        pthread_create(&cons0, NULL, consumer, (void*)0L);
        usleep(100000);
        pthread_create(&cons1, NULL, consumer, (void*)1L);
        
        pthread_join(cons1, NULL);
    }

    if(*argv[1] == '3'){
        pthread_t cons, prod0, prod1, prod2, pri0;

        pthread_create(&prod0, NULL, producer, (void*)0L);
        usleep(100000);
        
        pthread_create(&prod1, NULL, producer, (void*)1L);
        usleep(100000);
        
        pthread_create(&prod2, NULL, producer, (void*)2L);
        usleep(100000);

        pthread_create(&pri0, NULL, privileged_producer, (void*)1000L);
        usleep(1000000);

        pthread_create(&cons, NULL, consumer, &mb);
        usleep(100000);


        pthread_join(cons, NULL);
    }
    if(*argv[1] == '4'){
        int num = 0;
        char msg_content[128];
        Message* new_msg;

        sprintf(msg_content, "Message from %d thread", (int*)0);
        new_msg = new Message(msg_content);
        mb->push(new_msg);

        sprintf(msg_content, "Message from %d thread", (int*)1);
        new_msg = new Message(msg_content);
        mb->push(new_msg);

        sprintf(msg_content, "Message from %d thread", (int*)2);
        new_msg = new Message(msg_content);
        mb->push(new_msg);

        sprintf(msg_content, "Priority message from producer %d", (int*)1000);
        new_msg = new Message(msg_content);
        mb->push_pri(new_msg);

        sprintf(msg_content, "Message from %d thread", (int*)3);
        new_msg = new Message(msg_content);
        mb->push(new_msg);

        sprintf(msg_content, "Priority message from producer %d", (int*)1001);
        new_msg = new Message(msg_content);
        mb->push_pri(new_msg);
        
        mb->pop();
        mb->pop();
        mb->pop();
        mb->pop();

    }




    return 0;
}