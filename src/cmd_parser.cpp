#include "pch.h"
#include "cmd_parser.h"
#include "text_parser.h"

FILE* get_cmd_output(const char* cmd)
{
#ifdef _WIN32
    FILE* fp = Popen((string(cmd) + " 2>nul").c_str(), "r");
#else
    FILE* fp = Popen(cmd, "r");
#endif
    if (fp == NULL)
        return NULL;
    return fp;
}

bool get_filename(vector<string>& filename_list)
{
    filename_list.clear();
    string t;
    FILE* cmd_out = get_cmd_output(List_file);
    if (cmd_out == NULL)
    {
        cerr << "Warning: fail to read files in folder ";
        return false;
    }

    while (get_line(t, cmd_out))
    {
        // regard all files beginning with '.' as ignored files
        if (is_empty_str(t) || t[0] == '.') continue;
        for (auto &c: t) c = tolower(c); // insensitive to letter lower/upper
        filename_list.emplace_back(t);
    }
    Pclose(cmd_out);
    return true;
}

bool get_foldername(vector<Folder_Node*>& folder_list)
{
    string t;
    FILE* cmd_out = get_cmd_output(List_dir);
    if (cmd_out == NULL)
    {
        cerr << "Warning: fail to read folders in folder ";
        return false;
    }

    while (get_line(t, cmd_out))
    {
        // regard all folders beginning with '.' as ignored folders
        if (is_empty_str(t) || t[0] == '.') continue;
        for (auto &c: t) c = tolower(c); // insensitive to letter lower/upper
        Folder_Node* Newnode = new Folder_Node(t);
        folder_list.emplace_back(Newnode);
    }
    Pclose(cmd_out);
    return true;
}
