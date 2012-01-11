#include "poFileLoader.h"


#ifdef _WIN32
#include <windows.h>
#endif
#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

size_t write_data( void *ptr, size_t size, size_t nmeb, FILE *stream) {
    return std::fwrite(ptr,size,nmeb,stream);
}

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
    ((std::string*)stream)->append((char*)ptr, 0, size*count);
    return size*count;
}

poFileLoader::poFileLoader() {
}

poFileLoader::~poFileLoader() {}



void poFileLoader::getFile(std::string url, std::string filename) {
    if(filename == "") {
        boost::char_separator<char> sep("/");
        boost::tokenizer< boost::char_separator<char> > tokens(url, sep);
        BOOST_FOREACH(const std::string &t, tokens) {
            filename = t;
        }
    }
    
    FILE * file = (FILE *)fopen(filename.c_str(),"w+");
    if(!file){
        perror("File Open:");
        exit(0);
    }
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle,CURLOPT_URL,url.c_str()); /*Using the http protocol*/
    curl_easy_setopt(handle,CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle,CURLOPT_WRITEDATA, file);
    curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    
    fclose(file);
}

std::string poFileLoader::getFileAsString(std::string url) {
    std::string response;
    
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle,CURLOPT_URL, url.c_str()); /*Using the http protocol*/
    curl_easy_setopt(handle,CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(handle,CURLOPT_WRITEDATA, &response);
    curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    
    return response;
}



//void poFileLoader::update() {
//}
//
//void poFileLoader::eventHandler(poEvent *event) {
//}
//
//void poFileLoader::messageHandler(const std::string &msg, const poDictionary& dict) {
//}