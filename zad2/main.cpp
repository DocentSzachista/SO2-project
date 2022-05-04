#include <mutex>
#include <thread>
#include <vector>
#include <iostream> 
#include <sstream>
#include <string>
#include <array>
#include <chrono>

// constant ammount of threads to be created
#define N 5
struct Fork {
    std::mutex mutex;
};



class Philosopher {
    public:

    Philosopher( int id, Fork & left, Fork & right): 
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
        // lock mutexes
        std::lock(leftHand.mutex, rightHand.mutex);
        std::lock_guard<std::mutex> left_lock(leftHand.mutex, std::adopt_lock);
        std::lock_guard<std::mutex> right_lock(rightHand.mutex, std::adopt_lock);   
        action("eating");
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
    
    Fork& leftHand;
    Fork& rightHand;
    int phId ;
};

// functions to get forks on hands 

int lefHand(int i){
    return i;
}

int rightHand(int i, int n_forks){
    return (1 + i)%n_forks;
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

int main(int argc, char **argv) {
    
    if(argc != 3 ){
        std::cout<< "Insuficient ammount of commandline arguments. Please add a number of threads that you wish to launch" << std::endl;
        return -1;
    }
    std::string arg = argv[1];
    std::string arg_2 = argv[2];
    int n_threads = convert_argument_to_int(arg);
    int n_forks = convert_argument_to_int(arg_2);;
    if(n_threads == -1 || n_forks == -1){
        return -1;
    } else if(n_forks < n_threads){
        return -1;
    }



    // create vector of threads
    std::vector<std::thread> threads;
    
    // vector for our forks and philosophers 
    std::vector<Philosopher> philosophers;
    std::vector<Fork> forks(n_forks);


    // initialization
    for(int i = 0 ; i < n_threads; i++) {
        philosophers.push_back( Philosopher(i, 
                                            forks[lefHand(i)],
                                            forks[rightHand(i, n_forks)]
                                                  ));
        // std::cout << lefHand(i) << " " << rightHand(i, n_forks) << " " << philosophers.size() << std::endl; 
    }
    // start diner 
    for(int i = 0 ; i<  n_threads ; i++ )
         threads.push_back( std::thread(&Philosopher::dine,   philosophers[i]) );
    
    // finish 
    for(std::thread & th : threads)
        th.join();
    
}