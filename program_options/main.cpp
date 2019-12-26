#include <iostream>

#include "options.hpp"

using namespace std;

int main(int ac, char* av[]) {
    if(options.parse(ac,av))
        cout << "Parsing options worked!" << endl;
    else
        cout << "Parsing options failed!" << endl;

    LOG(trace)   << "Trace level message";
    LOG(debug)   << "Debug level message";
    LOG(info)    << "Informative level message";
    LOG(warning) << "Warning level message";
    LOG(error)   << "Error level message";
    LOG(fatal)   << "Fatal level message";

    return 0;
}

