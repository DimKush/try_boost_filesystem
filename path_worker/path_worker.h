#pragma once 

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <unordered_map>
#include <iostream>
#include <thread>

#include "packer/Packer.h"

namespace PathWorker
{
    namespace fs = boost::filesystem;
    std::list<fs::path> try_to_open_directories(int argc, char **argv);
    void process_directory(const fs::path &workingPath);
    [[deprecated]]void process_file(const std::string &fileName,Packer<float> &packer_);
    std::vector<std::string> split(std::string const & str, char symbol);
    void write_in_file(const fs::path &workingPath, const std::vector<Packer<float>> & packerVector_);
    void open_result_file_and_show(const fs::path &workingPath);
    void try_to_open_file_and_get_info(std::string const &fileName, std::unordered_multimap<int, const std::fstream> & filesMap);
    
    void capture_data_from_file(std::pair<const int, const std::fstream> &element);
    
    void process_big_data(std::pair<const int, const std::fstream> &element);
    //void process_file(std::shared_ptr<std::pair<const int, const std::fstream>> sPtr);
}