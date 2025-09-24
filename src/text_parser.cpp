#include "pch.h"
#include "text_parser.h"

using namespace std;

static const int BUF_SIZE = 10005;
static char buffer[BUF_SIZE], *p1, *p2;

FILE* Fopen(const char* filename, const char* mode)
{
    p1 = p2 = buffer;
    return fopen(filename, mode);
}
void Fclose(FILE* file)
{
    p1 = p2 = buffer;
    fclose(file);
}

char getNextChar(FILE* in)
{
    if (p1 == p2)
    {
        p2 = (p1 = buffer) + fread(buffer, 1, BUF_SIZE, in);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}

bool get_line(string& res, FILE* in)
{
    if (!in) return false;
    res.clear();
    char c;
    while (~(c = getNextChar(in)))
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
    FILE* file = Fopen(filename, "rb");
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
    
    Fclose(file);
    now->file_attr[type] += file_attr;
}