#pragma once 

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iostream>
#include <thread>

#include "packer/Packer.h"

namespace PathWorker
{
    namespace fs = boost::filesystem;
    std::list<fs::path> try_to_open_directories(int argc, char **argv);
    void process_directory(const fs::path &workingPath);
    void process_file(const std::string &fileName,Packer<float> &packer_);
    std::vector<std::string> split(std::string const & str, char symbol);
}