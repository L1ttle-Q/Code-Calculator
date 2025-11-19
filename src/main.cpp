#include "pch.h"
#include "calc_code.h"

int main(int argc, char* argv[])
{
    bool Structured_print = false;
    if (argc > 3)
    {
        cerr << "argument number more than 2." << endl;
        return -1;
    }
    if (argc >= 2)
    {
        int ret = CD(argv[1]);
        if (ret != 0)
        {
            cerr << "Failed to change directory to " << argv[1] << endl;
            return -1;
        }
        if (argc == 3)
        {
            if (!strcmp(argv[2], "json"))
                Structured_print = true;
        }
    }

    Folder_Node root(".");
    recursive_calc(root);
    print_attr(&root, Structured_print);
    return 0;
}