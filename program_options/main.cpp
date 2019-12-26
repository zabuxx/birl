#include <iostream>

#include "options.hpp"

using namespace std;

int main(int ac, char* av[]) {
    Options options;

    if(options.parse(ac,av))
        cout << "Parsing options worked!" << endl;
    else
        cout << "Parsing options failed!" << endl;

    cout << endl << "All options:" << endl;
    cout << options.DumpVariableMap() << endl;

    return 0;
}

