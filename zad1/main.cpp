// cpp libs
#include <iostream>
#include <stdexcept>
#include <string>


//C libraries
#include <pthread.h>
#include <stdlib.h>     
#include <time.h>
#include <unistd.h>

// constants
//#define N 2
void *thread_function(void * arg)
{
    long int sum = 0;
    long int passed_arg = *((long int*) arg);
    std::cout << "thread "<< passed_arg  << " is working "<<std::endl;
    for(long int i=0; i<=passed_arg; i++)
    {
        sum+=i*i;
        int random =rand()%10 +1 ; 
        if( random % 2 == 0)
        {
            std::cout << "thread "<< passed_arg  << " is sleeping "<<sum <<std::endl;
            sleep(random);
            std::cout << "thread "<< passed_arg << " is working again "<<std::endl;
        }
        
    }
    std::cout << "Thread id: "<<  passed_arg << "has finished."  <<" Counted function sum: " << sum << std::endl;
    pthread_exit(NULL);
}
int convert_argument_to_int(std::string arg){
    int x = -1;
    try {
        std::size_t pos;
        x = std::stoi(arg, &pos);
        
    } catch (std::invalid_argument const &ex) {
        std::cerr << "Invalid number: " << arg << '\n';
    } catch (std::out_of_range const &ex) {
        std::cerr << "Number out of range: " << arg << '\n';
    }
    return x;
}



int main(int argc, char **argv){
    
    if(argc != 2 ){
        std::cout<< "Insuficient ammount of commandline arguments. Please add a number of threads that you wish to launch" << std::endl;
        return -1;
    }
    std::string arg = argv[1];
    int n_threads = convert_argument_to_int(arg);
    if(n_threads == -1){
        return -1;
    }
    srand(time(NULL));
    pthread_t thread[n_threads];
    for(long int i= 0; i< n_threads; i++)
        pthread_create(&thread[i], NULL, thread_function, (void *)&i);
    std::cout << "Hello worlds" << std::endl;
    for(int i=0; i< n_threads; i++)
        pthread_join(thread[i], NULL);

}