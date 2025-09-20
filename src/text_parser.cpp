#include "pch.h"
#include "text_parser.h"

using namespace std;

bool get_line(string& res, FILE* in)
{
    if (!in) return false;
    res.clear();
    unsigned char c;
    while (fread(&c, sizeof(unsigned char), 1, in))
    {
        if (c == '\n')
            return true;
        if (c != 0x0D)
            res += c;
    }
    return !res.empty();
}

bool is_empty_str(string& str)
{
    for (char c: str)
    {
        if (c == '\t' || c == ' ') continue;
        return false;
    }
    return true;
}

void count_attr(Folder_Node* now, const char* filename, File_type type)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        cerr << "Fail to open file \'" << filename << "\'" << endl;
        return ;
    }
    
    File_attr file_attr;
    string line;
    while (get_line(line, file))
    {
        file_attr.line++;
        file_attr.blank += is_empty_str(line);
        // comment check
        /*
        recommend:
        check one-line comment(accurately just '//' at the beginning of a line),
        regardless of multi-line comment.
        
        hard to classify such a situation:
        /*
        *
        * a line is a comment, and a blank line as well.
        */
    }
    
    fclose(file);
    now->file_attr[type] += file_attr;
}