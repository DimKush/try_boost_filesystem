#include"path_worker/path_worker.h"
#include <list>
#include <thread>

int main(int argc, char** argv)
{
    std::list<PathWorker::fs::path> pathsContainer;
    std::vector<std::thread> thrd;
    pathsContainer = PathWorker::try_to_open_directories(argc, argv);    
    
    for(auto i : pathsContainer)
        thrd.push_back(std::thread{PathWorker::process_directory, i});

    for(auto &i : thrd)
        i.join();
    
    for(auto i : pathsContainer)
    {
        PathWorker::open_result_file_and_show(i); 
    }

    return 0;
}