#pragma once
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

// Thank you leetcode
inline std::vector<std::pair<float, float>> merge(std::vector<std::pair<float, float>>& intervals) {
    std::sort(intervals.begin(), intervals.end(), [](std::pair<float, float>& a, std::pair<float, float>& b) {
        return a.first < b.first;
    });

    std::vector<std::pair<float, float>> output;
    for(auto interval : intervals){
        if(output.empty() || output.back().second < interval.first){
            output.push_back(interval);
        }
        else{
            output.back().second = std::max(output.back().second, interval.second);
        }
    }
    return output;
}

template<typename ... Args>
inline std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

inline std::string fileContents(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();

        return contents;
    }
    printf("Could not open file %s\n", filename);
    throw(errno);
}

inline std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}