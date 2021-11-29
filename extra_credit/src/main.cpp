#include <pthread.h>
#include <cstdint>
#include <string>
#include <iostream>
#include <deque>

using namespace std;

constexpr int INITIAL_OPS = 3;

class Operator {
public:
    pthread_mutex_t op;
};

class Customer {
    pthread_t customer_thread;
    pthread_attr_t customer_attr;
    string name;
public:
    Customer(string name) : name(name) {
        cout << this->name << " is waiting to speak to the operator...\n";
    }
    void speak(Operator* op) {

    }
};


class CallCenter {
    std::deque<Customer> customers;
    Operator operators[INITIAL_OPS];
    int available_operators = 3;
public:
    CallCenter() {
        cout << "Welcome to the call center. Please wait for the setup to start\n";
        cout << "Initial operator availability = " << INITIAL_OPS << endl;
    }
    void new_customer(string name) {
        Customer customer = Customer(name);
        this->customers.push_back(customer);
    }
};

int main(int, char**) {

    CallCenter cc;

    cc.new_customer("Alice");
    cc.new_customer("Bob");
    cc.new_customer("John");
    cc.new_customer("Mark");
    cc.new_customer("Alex");
    
}
