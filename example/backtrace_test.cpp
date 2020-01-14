#define BOOST_STACKTRACE_USE_BACKTRACE
#include <signal.h>     // ::signal, ::raise
#include <iostream>
#include <boost/stacktrace.hpp>
#include <boost/filesystem.hpp>

using namespace boost;

void test(int n) {
    std::vector<std::string> bt;
    std::cout << stacktrace::stacktrace();
}

void my_signal_handler(int signum) {
    ::signal(signum, SIG_DFL);
    boost::stacktrace::safe_dump_to("./backtrace.dump");
    ::raise(SIGABRT);
}

int main()
{
    ::signal(SIGSEGV, &my_signal_handler);
    ::signal(SIGABRT, &my_signal_handler);

    std::cout << "hello" << std::endl;
    test(5);

    return 0;
}
