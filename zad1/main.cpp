// cpp libs
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

//C libraries
#include <pthread.h>
#include <stdlib.h>     
#include <time.h>
#include <unistd.h>

// constants
//#define N 2



void *thread_function(void * arg)
{
     int sum = 0;
    int passed_arg = *(( int*) arg);
   
    std::cout << "thread "<< passed_arg  << " is working "<<std::endl;
    for( int i=0; i<=passed_arg; i++)
    {
        sum+=i*i+ i*10;
        int random =rand()%10 +1 ; 
    
        std::cout << "thread "<< passed_arg  << " is sleeping. Counted sum: "<< sum << std::endl;
        sleep(random);
        std::cout << "thread "<< passed_arg  << " is working again "<< std::endl;
        
        
    }
    std::cout << "Thread id: "<<  passed_arg << " has finished."  <<" Counted function sum: " << sum << std::endl;
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
    std::cout << "Simulation is starting. Numb of simulated threads: "<< n_threads << std::endl;
    srand(time(NULL));
    std::vector<pthread_t> threadsVector(n_threads);
    int* table = new int[n_threads];
    // pthread_t thread[n_threads];
    for(int i= 0; i< n_threads; i++)
    {
        table[i]  = i;
        pthread_create(&threadsVector[i], NULL, thread_function, (void *)&table[i]);
    }
    for(int i=0; i< n_threads; i++)
        pthread_join(threadsVector[i], NULL);
    std::cout <<"Simulation has ended" << std:: endl;
    delete []table;
}