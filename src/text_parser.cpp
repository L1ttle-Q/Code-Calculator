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

void getNextToken(string& str, int pos, string& token)
{
    token.clear();
    bool in_token = false;
    for (int i = pos; i < str.length(); i++)
    {
        if (!in_token)
        {
            if (str[i] == ' ' || str[i] == '\t') continue;
            else {token += str[i]; in_token = true;}
        }
        else
        {
            if (str[i] == ' ' || str[i] == '\t') break;
            else token += str[i];
        }
    }
}

int getDepth(string& str)
{
    int depth = 0;
    for (char c: str)
    {
        if (c == ' ') depth++;
        else break;
    }
    return (depth + 3) / 4;
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

bool is_comment(string& str, File_type type)
{
    bool res = false;
    switch (type)
    {
    case python:
        for (char& c: str)
        {
            if (c == ' ') continue;
            else if (c == '#') {res = true; break;}
        }
        break;
    
    default:
        break;
    }
    return res;
}

bool is_func_begin(string& str, File_type type, int& depth)
{
    bool res = false;
    string token;
    switch (type)
    {
    case python:
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == ' ') depth++;
            else if (str[i] == '#') {res = false; break;}
            else
            {
                getNextToken(str, i, token);
                if (token == "def") res = true;
                else if (token == "async")
                {
                    getNextToken(str, i + 5, token);
                    if (token == "def") res = true;
                    else res = false;
                }
                else res = false;
                break;
            }
        }
        break;
    
    default:
        break;
    }
    depth = (depth + 3) / 4;
    if (!res) depth = 0;
    return res;
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
    bool in_func = false;
    int func_dep = 0;
    int func_cnt = 0;
    while (get_line(line, file))
    {
        file_attr.line++;
        if (is_empty_str(line)) {file_attr.blank++; continue;}
        if (is_comment(line, type)) {file_attr.comment++; continue;}

        if (type != python) continue; // only calc func for python

        if (!in_func && is_func_begin(line, type, func_dep))
        {
            in_func = true;
            func_cnt = 0;
        }
        else if (in_func)
        {
            if (getDepth(line) <= func_dep)
            {
                file_attr.m_function.emplace_back(func_cnt);
                if (func_cnt == 618) fprintf(stderr, "%s\n", filename);
                in_func = false; func_dep = 0; func_cnt = 0;
            }
            else func_cnt++;
        }
    }
    if (in_func) file_attr.m_function.emplace_back(func_cnt);
    if (func_cnt == 618) fprintf(stderr, "%s\n", filename);

    Fclose(file);
    now->file_attr[type] += file_attr;
}