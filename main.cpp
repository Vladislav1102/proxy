#include "proxy.hpp"

int main()
{
    Proxy proxy;
    try
    {
        proxy.run();
    }
    catch(const std::exception& e)
    {
        std::cout << "exception!!!" << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}