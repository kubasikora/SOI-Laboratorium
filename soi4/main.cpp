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
        try{
            new_msg = new Message(msg_content);
        } catch(std::bad_alloc& ba){
            std::cout << "Bad alloc from producer " << num << std::endl;
            exit(1);
        }
        mb->push(new_msg);

        sleep(1);
    }
}

void *privileged_producer(void *num){
    Message *new_msg;
    char msg_content[128];
    long tid = (long)num;
    for(int i = 0; i < 100; i++){
        sprintf(msg_content, "Priority message number %d from producer %d", i,(int*)num);
        try{
            new_msg = new Message(msg_content);
        } catch(std::bad_alloc& ba){
            std::cout << "Bad alloc from producer " << num << std::endl;
            exit(1);
        }
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
        std::cout << "4. Problem zachowania kolejnosci wiadomosci priorytetowych" << std::endl;
        std::cout << "5. Problem pelnego bufora wiadomosci priorytetowych" << std::endl;
        std::cout << "6. Problem pustego bufora wiadomosci priorytetowych" << std::endl;
        std::cout << "7. Zapelnienie bufora po polowie wiadomosciami roznego typu" << std::endl;
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
        pthread_t cons, pri0, pri1, pri2, pri3;

        pthread_create(&pri0, NULL, privileged_producer, (void*)1000L);
        usleep(100000);
        
        pthread_create(&pri1, NULL, privileged_producer, (void*)1001L);
        usleep(100000);
        
        pthread_create(&pri2, NULL, privileged_producer, (void*)1002L);
        usleep(100000);

        pthread_create(&pri3, NULL, privileged_producer, (void*)1003L);
        usleep(1000000);

        pthread_create(&cons, NULL, consumer, &mb);
        usleep(100000);


        pthread_join(cons, NULL);

    }

    if(*argv[1] == '5'){
        pthread_t pri0, pri1, pri2, pri3, pri4;
        
        pthread_create(&pri0, NULL, privileged_producer, (void*)0L);
        usleep(100000);

        pthread_create(&pri1, NULL, privileged_producer, (void*)1L);
        usleep(100000);

        pthread_create(&pri2, NULL, privileged_producer, (void*)2L);
        usleep(100000);

        pthread_create(&pri3, NULL, privileged_producer, (void*)3L);
        usleep(100000);

        pthread_create(&pri4, NULL, privileged_producer, (void*)4L);
        
        pthread_join(pri0, NULL);
    }

    if(*argv[1] == '6'){
        pthread_t cons0, cons1;
        
        Message *new_msg;
        char msg_content[128] = "Priority message number 0 from producer 1000";
        try{
            new_msg = new Message(msg_content);
        } catch(std::bad_alloc& ba){
            std::cout << "Bad alloc from producer 0" << std::endl;
            exit(1);
        }
        mb->push_pri(new_msg);
        sleep(2);

        pthread_create(&cons0, NULL, consumer, (void*)0L);
        usleep(100000);
        pthread_create(&cons1, NULL, consumer, (void*)1L);
        
        pthread_join(cons1, NULL);
    }
    
    if(*argv[1] == '7'){
        pthread_t pri0, prod0, cons0;
        char msg_content[128];
        Message *new_msg;

        for(int i = 0; i < 10; i++){
            sprintf(msg_content, "Message number %d from producer %d", i, 0);  
            try{
                new_msg = new Message(msg_content);
            } catch(std::bad_alloc& ba){
                std::cout << "Bad alloc from producer 0" << std::endl;
                exit(1);
            }
            mb->push(new_msg);
        }

        for(int i = 0; i < 10; i++){
            sprintf(msg_content, "Priority message number %d from producer %d", i, 0);  
            try{
                new_msg = new Message(msg_content);
            } catch(std::bad_alloc& ba){
                std::cout << "Bad alloc from producer 0" << std::endl;
                exit(1);
            }
            mb->push_pri(new_msg);
        }
        sleep(2);

        pthread_create(&cons0, NULL, consumer, (void*)0L);
        usleep(100000);
        pthread_create(&prod0, NULL, privileged_producer, (void*)1000L);
        usleep(100000);
        pthread_create(&pri0, NULL, producer, (void*)1L);
        
        pthread_join(cons0, NULL);
    }

    return 0;
}