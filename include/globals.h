#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <iostream>
#include <algorithm>

using namespace std;

const int MAX_LINE = 100000;
const int FILE_TYPE_NUM = 5;
typedef enum
{
    cpp, c, h, java, python, other = -1
} File_type;
const string File_extensions[FILE_TYPE_NUM] = {
    ".cpp", ".c", ".h", ".java", ".py"
};

struct File_attr
{
    int line;
    int comment;
    int blank;
    vector<int> m_function;

    File_attr() {line = comment = blank = 0;}
    File_attr operator += (File_attr& b)
    {
        line += b.line;
        comment += b.comment;
        blank += b.blank;
        for (int& v: b.m_function)
            m_function.emplace_back(v);
        return *this;
    }

    int function_cnt()
    {
        if (m_function.empty()) return 0;
        return m_function.size();
    }
    int function_line()
    {
        if (m_function.empty()) return 0;
        int res = 0;
        for (int i: m_function)
            res += i;
        return res;
    }
    int function_max()
    {
        if (m_function.empty()) return 0;
        int res = 0;
        for (int i: m_function)
            res = max(res, i);
        return res;
    }
    int function_min()
    {
        if (m_function.empty()) return 0;
        int res = MAX_LINE;
        for (int i: m_function)
            res = min(res, i);
        return res;
    }
    int function_middle()
    {
        if (m_function.empty()) return 0;
        sort(m_function.begin(), m_function.end());
        return m_function[function_cnt() / 2];
    }
    int function_avg()
    {
        if (m_function.empty()) return 0;
        return function_line() / function_cnt();
    }

};

struct Folder_Node
{
    string name;
    int file_sum[FILE_TYPE_NUM];
    vector<string> son_file;
    vector<Folder_Node*> son_folder;
    File_attr file_attr[FILE_TYPE_NUM];

    Folder_Node(string _name) : name(_name), son_file(), son_folder()
    {
        for (int i = 0; i < FILE_TYPE_NUM; i++)
        {
            file_sum[i] = 0;
            file_attr[i] = File_attr();
        }
    }

    void pushup()
    {
        for (Folder_Node* son: son_folder)
        {
            for (int i = 0; i < FILE_TYPE_NUM; i++)
            {
                file_attr[i] += son->file_attr[i];
                file_sum[i] += son->file_sum[i];
            }
        }
        
    }

    ~Folder_Node()
    {
        for (Folder_Node* son: son_folder)
            delete son;
    }
};

#ifdef _WIN32
    #include <direct.h>
    #define CD(path) _chdir(path)
    #define List_dir "dir /ad /b"
    #define List_file "dir /a-d /b"
#else
    #include <unistd.h>
    #define CD(path) chdir(path)
    #define List_dir "ls -l | grep '^d' | awk \'{print $9}\'"
    #define List_file "ls -l | grep '^-' | awk \'{print $9}\'"
#endif

#endif /* GLOBALS_H_ */