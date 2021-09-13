#include <iostream>
#include "log.h"
#include "func.h"
using namespace std;

int main()
{
    LOG_INIT;
    LOG_WARN("This is a warning");
    cout << "Hello World!   " << func() << endl;
    return 0;
}
