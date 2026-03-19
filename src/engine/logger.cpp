#include <iostream>
#include <string>
#include "logger.hpp"

using namespace std;

namespace logger
{
    void log(const string &s)
    {
        cout << "Info: " << s << "\n";
    }
    void logError(const string &s)
    {
        cout << "Error: " << s << "\n";
    }
};