#include "Level.hpp"

#include "Plist.hpp"

#include <base64/base64.hpp>
#include <libdeflate.h>

#include <fstream>
#include <sstream>

#include <span>

Level* Level::fromGMD(std::string path) {
    std::map<std::string, boost::any> dict;
    Plist::readPlist(path.c_str(), dict);

    if (dict.find("kCEK") == dict.end()) {
        std::cout << "Level::fromGMD: Invalid level file" << std::endl;
        return nullptr;
    }

    Level* level = new Level();

    level->m_levelName = boost::any_cast<std::string>(dict["k2"]);
    level->m_levelString = boost::any_cast<std::string>(dict["k4"]);
    
    level->m_audioTrack = dict.find("k8") == dict.end() ? dict.find("k45") == dict.end() ? 1 : boost::any_cast<int64_t>(dict["k45"]) : boost::any_cast<int64_t>(dict["k8"]);
    
    printf("Decoding level string\n");
    level->decodeLevelString();
    printf("Level string decoded\n");

    return level;
}

Level* Level::fromServers(int id) {
    return nullptr;
}

void Level::decodeLevelString() {
    std::replace(m_levelString.begin(), m_levelString.end(), '_', '/');
	std::replace(m_levelString.begin(), m_levelString.end(), '-', '+');

    bool gzip = m_levelString.rfind("H4sIAAAAAAAAC", 0) == 0;
    auto decompressFunction = gzip ? libdeflate_gzip_decompress : libdeflate_zlib_decompress;
    
	std::string decoded;
    macaron::Base64::Decode(m_levelString, decoded);
    
    size_t bufferSize = 128 * 1024;

    libdeflate_result result; 
    size_t size;

    do {
    	libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();

        bufferSize *= 2;

        m_levelString.resize(bufferSize);
        result = decompressFunction(decompressor, (const void*)decoded.data(), decoded.size(), (void*)m_levelString.data(), m_levelString.size(), &size);
     
        libdeflate_free_decompressor(decompressor);
    } while (result == LIBDEFLATE_INSUFFICIENT_SPACE);
    
    m_levelString.resize(size);
}