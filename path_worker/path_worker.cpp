#include <exception>
#include <thread>

#include"path_worker/path_worker.h"

namespace PathWorker
{
    std::list<fs::path> try_to_open_directories(int argc, char **argv)
    {
        try
        {
            if(argc == 1)
                throw std::logic_error("No arguments in program. Please, choose witch catalog need to process. p1, p2 or p3.");
            
            const fs::path workdir = fs::current_path() / "proc";

            std::list<fs::path> pathsContainer;

            for(int i = 1 ; i < argc; i++)
                pathsContainer.emplace_back(fs::path( workdir / std::string(argv[i]) ));
            
            return pathsContainer;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::exit(0);
        }
    }

    void process_directory(fs::path const & workingPath)
    {
        std::list<std::string> filesContainer;
        std::vector<std::thread> filesThreads;

        std::cout << workingPath.string() << '\n';
        for(fs::recursive_directory_iterator iter(workingPath), iterEnd; iter != iterEnd; ++iter)
        {
            filesContainer.emplace_back(iter->path().filename().string());
        }

        for(auto i : filesContainer)
        {
            filesThreads.push_back(std::thread(process_file, std::ref(i)));
        }
        for(auto &i : filesThreads)
        {
            i.join();
        }
    }

    void process_file(std::string const & fileName)
    {
        std::cout << "I am here " << fileName << '\n';
    }
}   