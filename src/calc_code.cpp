#include "pch.h"
#include "calc_code.h"
#include "text_parser.h"
#include "cmd_parser.h"

using namespace std;

File_type check_file(string filename)
{
    int len = filename.length();
    for (int i = 0; i < FILE_TYPE_NUM; i++)
    {
        int postfix_len = File_extensions[i].length();
        if (len < postfix_len)
            continue;
        if (filename.substr(len - postfix_len) == File_extensions[i])
            return static_cast<File_type>(i);
    }
    return other;
}

void recursive_calc(Folder_Node& now)
{
    get_filename(now.son_file);
    get_foldername(now.son_folder);

    for (string file: now.son_file)
    {
        File_type t_type = check_file(file);
        if (t_type == other) continue;
        now.file_sum[t_type]++;
        count_attr(&now, file.c_str(), t_type);
    }
    for (Folder_Node* folder: now.son_folder)
    {
        if (CD(folder->name.c_str()) != 0) 
        {
            cerr << "Warning: Cannot access directory \'" << folder->name << "\'" << endl;
            continue;
        }
        recursive_calc(*folder);
        now.pushup();
        CD("..");
    }
}

void print_attr(Folder_Node* now)
{
    printf("File type\t|line\t|blank\t|comment\t|file\n");
    for (int i = 0; i < FILE_TYPE_NUM; i++)
    {
        int t_line = now->file_attr[i].line,
            t_blank = now->file_attr[i].blank,
            t_comment = now->file_attr[i].comment,
            t_file_sum = now->file_sum[i];
        printf("%s\t\t|%d\t|%d\t|%d\t\t|%d\n", File_extensions[i].c_str(),
                                             t_line - t_blank,
                                             t_blank,
                                             t_comment,
                                             t_file_sum);
    }
}