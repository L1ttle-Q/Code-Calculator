#include "pch.h"
#include "cmd_parser.h"
#include "text_parser.h"

FILE* get_cmd_output(const char* cmd)
{
    return Popen(cmd, "r");
}

void get_filename(vector<string>& filename_list)
{
    filename_list.clear();
    string t;
    FILE* cmd_out = get_cmd_output(List_file);
    while (get_line(t, cmd_out))
    {
        // regard all files beginning with '.' as ignored files
        if (t.empty() || t[0] == '.') continue;
        filename_list.emplace_back(t);
    }
    Pclose(cmd_out);
}

void get_foldername(vector<Folder_Node*>& folder_list)
{
    string t;
    FILE* cmd_out = get_cmd_output(List_dir);
    while (get_line(t, cmd_out))
    {
        // regard all folders beginning with '.' as ignored folders
        if (t.empty() || t[0] == '.') continue;
        Folder_Node* Newnode = new Folder_Node(t);
        folder_list.emplace_back(Newnode);
    }
    Pclose(cmd_out);
}
