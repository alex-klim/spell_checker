#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>


std::string log(std::string text) {
    std::ostringstream stamp;
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    stamp << "[" << std::put_time(&bt, "%H:%M:%S") << '.'
        << std::setfill('0') << std::setw(3) << ms.count()
        << " tid=" << std::this_thread::get_id() << "]: "
        << text << '\n';
    return stamp.str();
}
