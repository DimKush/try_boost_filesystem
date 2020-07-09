#pragma once 

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iostream>
#include <thread>

namespace PathWorker
{
    namespace fs = boost::filesystem;
    std::list<fs::path> try_to_open_directories(int argc, char **argv);
    void process_directory(fs::path const & workingPath);
    void process_file(std::string const & fileName);
}