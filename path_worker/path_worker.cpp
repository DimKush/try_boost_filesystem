#include <exception>
#include <thread>
#include <fstream>
#include <streambuf>
#include <condition_variable>

#include "path_worker/path_worker.h"

std::condition_variable cv;
std::mutex cv_m;

namespace PathWorker
{
    std::list<fs::path> try_to_open_directories(int argc, char **argv)
    {
        try
        {
            if(argc == 1)
                throw std::logic_error("No arguments in program. Please, choose witch catalog need to process. p1, p2 or p3.");
            
            const fs::path workdir = fs::current_path() / "resources";

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
        std::vector<std::thread> filesThreads;
        std::vector<Packer<float>> packersVector;
        std::unordered_multimap <int, const std::fstream> filesMap;

        if(fs::exists(workingPath / "result.txt"))
        {   
            fs::remove(workingPath / "result.txt");
        }

        size_t atCount = 0;

        for(fs::recursive_directory_iterator iter(workingPath), iterEnd; iter != iterEnd; ++iter)
        {
            filesThreads.emplace_back(std::thread{try_to_open_file_and_get_info, std::string{iter->path().string()}, std::ref(filesMap)} );
        } 

        for(auto &i : filesThreads)
            i.join();
        filesThreads.clear();
        
        for(auto &i : filesMap)
            capture_data_from_file(i);

    }

    void try_to_open_file_and_get_info(std::string const &fileName, std::unordered_multimap<int, const std::fstream> & filesMap)
    {
        std::fstream file;
        int fileSize = 0;

        file.open(fileName);
        
        if(!file)
        {
            std::cerr << "File " << fileName << " doesn't exist.\n";
            file.close();

            return;
        }
        
        fileSize = fs::file_size(fileName);

        if(fileSize == 0)
        {
            std::cerr << "File " << fileName << " is empty.\n";
            return;  
        }

        std::lock_guard<std::mutex> lg(cv_m);
    
        filesMap.emplace(std::piecewise_construct 
                        ,std::forward_as_tuple(fileSize)
                        ,std::forward_as_tuple(fileName));
        
        file.close();
    }

    void capture_data_from_file(std::pair<const int, const std::fstream> &element)
    {
        if(element.first >= 1000)
            process_big_data(element);
    }

    void process_big_data(std::pair<const int, const std::fstream> &element)
    {  
        std::string container1, container2, container3, container4;

        std::fstream &f = const_cast<std::fstream&>(element.second);
        std::string mainContainer{std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};
    }


    [[deprecated]]
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
    
    [[deprecated]]
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

    void open_result_file_and_show(const fs::path &workingPath)
    {
        fs::path workingPathFile = workingPath / "result.txt";
        std::fstream fileIn;
        std::string resultContent; 

        fileIn.open(workingPathFile.string());
        
        if(!fileIn)
            throw std::logic_error("Cannot open result file.");
        
        while(std::getline(fileIn,resultContent))
        {
            std::cout << resultContent << '\n';
        }
            
        fileIn.close();
    }
}   

