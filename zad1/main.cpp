#include<iostream>
#include<pthread.h>
#include <stdlib.h>     
#include <time.h>
#include <unistd.h>
#define N 3
void *thread_function(void * arg)
{
    long int sum = 0;
    std::cout << "thread "<< (long)arg  << " is working "<<std::endl;
    for(long int i=0; i<=(long)arg; i++)
    {
        sum+=i*i;
        int random =rand()%10 +1 ; 
        if( random % 2 == 0)
        {
            std::cout << "thread "<< (long)arg  << " is sleeping "<<sum <<std::endl;
            sleep(random);
            std::cout << "thread "<< (long)arg  << " is working again "<<std::endl;
        }
        
    }
    std::cout << "function sum " << sum << " thread " << (long)arg << "has finished."<< std::endl;
    pthread_exit(NULL);
}



int main(){
    srand(time(NULL));

    pthread_t thread[N];
    for(long int i= 0; i< N; i++)
        pthread_create(&thread[i], NULL, thread_function, (void *)i);
    std::cout << "Hello worlds" << std::endl;
    for(int i=0; i< N; i++)
        pthread_join(thread[i], NULL);
}