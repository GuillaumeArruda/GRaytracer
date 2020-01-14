
#include <iostream>

#include "gthread\job_manager.h"

int main(int argc, const char** argv)
{
    gthread::job_manager jobs(8);
    for (int i = 0; i < 10000000; ++i)
    {
        jobs.submit([]{});
    }

    return 0;
}