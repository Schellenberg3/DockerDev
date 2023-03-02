#include <iostream>
#include <string>

#include <Eigen/Geometry>

/// @brief  Identifies what operating system was present at compile time.
/// @return String OS name. 
std::string getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}

int main()
{
    std::cout << "Project compiled on a " << getOsName() << " operating system!" << std::endl;

    Eigen::Vector3d vec1(0, 1.5, -9.456);
    Eigen::Vector3d vec2 = vec1 * 2;
   
    std::cout << "The result of multiplying " << vec1.transpose() << " by 2 is " << vec2.transpose() << std::endl;

    return 0;
}
