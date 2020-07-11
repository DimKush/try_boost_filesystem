#include <exception>
#include <thread>
#include <fstream>
#include <streambuf>

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

    void process_directory(const fs::path &workingPath)
    {
        std::list<std::string> filesContainer;
        std::vector<std::thread> filesThreads;
        std::vector<Packer<float>> packersVector;

        if(fs::exists(workingPath / "result.out"))
        {   
            fs::remove(workingPath / "result.out");
        }

        size_t atCount = 0;

        for(fs::recursive_directory_iterator iter(workingPath), iterEnd; iter != iterEnd; ++iter)
        {
            filesContainer.emplace_back(std::string(iter->path().string()));
        }
        packersVector.resize(filesContainer.size());

        for(auto i = filesContainer.begin(); i != filesContainer.end() && atCount < filesContainer.size() ; i++, atCount++)
        {
            filesThreads.push_back(std::thread(process_file, *i, std::ref(packersVector.at(atCount))));
        }    

        for(auto &i : filesThreads)
            i.join();
        filesThreads.clear();
        
        write_in_file(workingPath / "result.out", packersVector);
    }

    void process_file(const std::string &fileName, Packer<float> &packer_)
    {      
        try
        {   
            std::fstream file(fileName);
            std::vector<std::string> vect;
            
            if(!file)
                throw std::logic_error("Can't open file.");

            std::string strTmp{std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};

            if(strTmp.empty())
                throw std::logic_error("No arguments in file.");
            
            std::replace(strTmp.begin(), strTmp.end(), '\n', ' ');
            vect = split(strTmp, ' ');

            packer_.setOperation(std::stoi(vect.at(0)));
            
            for(auto i = vect.begin() + 1 ; i != vect.end(); i++)
                packer_.setContent(std::stof(*i));

            file.close();
            packer_.count();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    
    std::vector<std::string> split(std::string const & str, char symbol)
    {
        std::vector<std::string> tmpContainer;

        std::string::size_type  beginPos = 0, endPos = str.find_first_of(symbol);

        while(endPos != std::string::npos)
        {
            tmpContainer.push_back(str.substr(beginPos,endPos - beginPos));

            beginPos = endPos + 1;
            endPos = str.find_first_of(symbol, beginPos);
        }

        tmpContainer.push_back(str.substr(beginPos));
        
        return tmpContainer;
    }

    void write_in_file(const fs::path &workingPathForRes, const std::vector<Packer<float>> & packerVector_)
    {
        std::ofstream fileOut;
        
        fileOut.open((workingPathForRes).string());
        
        for(auto i : packerVector_)
        {
            fileOut << i << '\n';
        }

        fileOut.close();
    }
}   