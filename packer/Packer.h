#pragma once

#include <vector>
#include <iostream>

#include "math.h"
template<typename T = float>
class Packer
{
    std::vector<T> content;
    int operation = 0;
    T result = T();

public:
    Packer() = default;

    void setContent(const T val)
    {
        content.push_back(val);
    }
    void setOperation(int operation_)
    {
        operation = operation_;
    }

    void count()
    {
        switch(operation)
        {
            case 1:
            {
                for(auto i : content)
                    result += i;

                break;
            }
            case 2:
            {
                result = 1;
                for(auto i : content)
                    result *= i;

                break;
            }
            case 3:
            {
                result = 1;
                for(auto it = content.begin() ; it != content.end(); it++)
                    result = pow(*(it),2) + pow(*(it-1),2);

                break;
            }
            default:
                break;
        }
    }
    int getOperation() const
    {
        return operation;
    }

    T getResult() const
    {
        return result;
    }
    
    friend std::ostream & operator << (std::ostream & stream_, const Packer &packer_)
    {
        stream_ << "variables: ";
        for(auto i : packer_.content)
        {
            stream_  << i << ' ';
        }

        stream_ << "operation - ";

        switch(packer_.operation)
        {
            case 1:
            {
                stream_ << '+' << ' ';

                break;
            }
            case 2:
            {
                stream_ << '*' << ' ';
                break;
            }
            case 3:
            {
               stream_ << "pow" << ' ';
                break;
            }
            default:
                break;
        }

        stream_ << "result = " << packer_.result;

        return stream_;
    } 
};
