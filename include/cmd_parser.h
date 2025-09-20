#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include <vector>
#include <string>
#include "globals.h"

#ifdef _WIN32
    #define Popen(ins, type) _popen(ins, type)
    #define Pclose(pfile) _pclose(pfile)
#else
    #define Popen(ins, type) popen(ins, type)
    #define Pclose(pfile) pclose(pfile)
#endif

bool get_filename(vector<string>&);
bool get_foldername(vector<Folder_Node*>&);

#endif /* CMD_PARSER_H_ */