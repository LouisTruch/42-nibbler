#include <arpa/inet.h> // inet_ntop
#include <ifaddrs.h>   // getifaddrs()
#include <string>      // std::string

const std::string getLocalIp()
{
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void *tmpAddrPtr = NULL;
    std::string localIp;
    if (getifaddrs(&ifAddrStruct) == -1)
    {
        if (ifAddrStruct != NULL)
            freeifaddrs(ifAddrStruct);
        return "getLocalIp() failed";
    }
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        std::string ifaNameStr(ifa->ifa_name);
        if (ifa->ifa_addr->sa_family == AF_INET && ifaNameStr == "eno2")
        {
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            localIp += addressBuffer;
        }
    }
    freeifaddrs(ifAddrStruct);
    return localIp;
}