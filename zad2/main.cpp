#include <mutex>
#include <thread>
#include <vector>
#include <iostream> 
#include <sstream>
#include <string>
#include <array>
#include <chrono>

// constant ammount of threads to be created
#define N 10

class Philosopher {
    public:

    Philosopher( int id, std::mutex* left, std::mutex* right): 
        phId(id), leftHand(left), rightHand(right){}

    void dine() {
        int c = 0;
        while (c < 5)
        {
            think();
            eat();
            c++;
        }
        
    }
    
    void think(){
        action("thinking");
    }

    void eat(){

        if(leftHand && rightHand){
            std::unique_lock<std::mutex> left(*leftHand,  std::defer_lock );
            std::unique_lock<std::mutex> right(*rightHand, std::defer_lock );
            
            std::lock(left, right);
            
            action("eating");
            
            left.unlock();
            right.unlock();
        }
        else{
            action("Sleeping");
        }

        
    }
    
    void action(std::string action){
            std::stringstream message_pre, message_post;

            message_pre << "Philosopher " << phId << " starts action: " << action << std::endl;
            std::cout <<  message_pre.str();
            // do action
            std::this_thread::sleep_for(std::chrono::seconds(1));
    
            // report finish
            message_post << "Philosopher " << phId << " finished action: " << action << std::endl;
            std::cout <<  message_post.str();

    }
    private:
    std::mutex *leftHand;
    std::mutex *rightHand;
    int phId ;
};

// functions to get forks on hands 

int lefHand(int i){
    return i;
}

int rightHand(int i){
    return (1 + i)%N;
}

int main() {
    
    // create vector of threads
    std::vector<std::thread> threads;

    // vector for our forks and philosophers 
    std::vector<Philosopher> philosophers;
    std::vector<std::mutex*> forks;
    std::vector<std::mutex*> list(N);

    forks.swap(list);

    // initialization
    for(int i = 0 ; i < N-1 ; i++) {
        philosophers.push_back( Philosopher(i, 
                                            forks[lefHand(i)],
                                            forks[rightHand(i)]
                                                  ));
    }
    for(int i = 0 ; i< N-1 ; i++ )
        threads.push_back( std::thread(&Philosopher::dine,   philosophers[i]) );
    
    // // finish 
    for(std::thread & th : threads)
        th.join();
    
}