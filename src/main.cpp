#include"path_worker/path_worker.h"
#include <list>

int main(int argc, char** argv)
{
    std::list<PathWorker::fs::path> pathsContainer;
    pathsContainer = PathWorker::try_to_open_directories(argc, argv);    

    for(auto i : pathsContainer)
        PathWorker::process_directory(i);

    return 0;
}