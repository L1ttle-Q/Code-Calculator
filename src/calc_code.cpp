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
    if (!get_filename(now.son_file))
        cerr << "\'" << now.name << "\'" << endl;
    else
    {
        for (string file: now.son_file)
        {
            File_type t_type = check_file(file);
            if (t_type == other) continue;
            now.file_sum[t_type]++;
            count_attr(&now, file.c_str(), t_type);
        }
    }
    
    if (!get_foldername(now.son_folder))
        cerr << "\'" << now.name << "\'" << endl;
    else
    {
        for (Folder_Node* folder: now.son_folder)
        {
            if (CD(folder->name.c_str()) != 0) 
            {
                cerr << "Warning: Cannot access directory \'" << folder->name << "\'" << endl;
                continue;
            }
            recursive_calc(*folder);
            CD("..");
        }
    }
    now.pushup();
}

void print_attr(Folder_Node* now)
{
    printf("File type\t|      line|     blank|   comment|      file| func_line|  func_cnt\n");
    for (int i = 0; i < FILE_TYPE_NUM; i++)
    {
        int t_line = now->file_attr[i].line,
            t_blank = now->file_attr[i].blank,
            t_comment = now->file_attr[i].comment,
            t_file_sum = now->file_sum[i],
            t_func_line = now->file_attr[i].function_line(),
            t_func_cnt = now->file_attr[i].function_cnt(),
            t_func_avg = now->file_attr[i].function_avg(),
            t_func_max = now->file_attr[i].function_max(),
            t_func_min = now->file_attr[i].function_min(),
            t_func_middle = now->file_attr[i].function_middle();
        printf("%s\t\t|%10d|%10d|%10d|%10d|%10d|%10d|(%d, %d, %d, %d)\n",
                                            File_extensions[i].c_str(),
                                            t_line - t_blank,
                                            t_blank,
                                            t_comment,
                                            t_file_sum,
                                            t_func_line,
                                            t_func_cnt,
                                            t_func_avg,
                                            t_func_max,
                                            t_func_min,
                                            t_func_middle);
    }
}