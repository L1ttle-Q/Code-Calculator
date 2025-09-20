#ifndef TEXT_PARSER_H_
#define TEXT_PARSER_H_

#include <string>
#include "globals.h"

bool get_line(string&, FILE*);
bool is_empty_str(string&);
void count_attr(Folder_Node*, const char*, File_type);

#endif /* TEXT_PARSER_H_ */