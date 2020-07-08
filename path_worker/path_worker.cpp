#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include <list>

#include"path_worker/path_worker.h"

namespace PathWorker
{
    namespace fs = boost::filesystem;
    void try_to_open_directories(int argc, char **argv)
    {
        const boost::filesystem::path workdir = fs::current_path() / "proc";

        std::list<fs::path> pathsContainer; 
        
        for(int i = 1 ; i < argc; i++)
            pathsContainer.emplace_back(fs::path(std::string(argv[i])));
        
        for(auto i : pathsContainer)
            std::cout << i.string() << '\n';

        try
        {
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }


}   