#include <time.h>
#include <unistd.h>
#include <iostream>

int main() {
    time_t time1 = time(NULL);
    std::cout << "Current time: " << ctime(&time1);
    srand(time(NULL));
    sleep(rand() % 10);
    time_t time2 = time(NULL);
    std::cout << "Current time: " << ctime(&time2);
    std::cout << "Time difference: " << time2 - time1 << " seconds" << std::endl;
    return 0;
}