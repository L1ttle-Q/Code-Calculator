#include "pch.h"
#include "calc_code.h"

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        cerr << "argument number more than 1." << endl;
        return -1;
    }
    if (argc == 2)
    {
        int ret = CD(argv[1]);
        if (ret != 0)
        {
            cerr << "Failed to change directory to " << argv[1] << endl;
            return -1;
        }
    }

    Folder_Node root(".");
    recursive_calc(root);
    print_attr(&root);
    return 0;
}