#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct directory
{
    char name[15], files[5][15], fcount;
} directory;

int main()
{
    int choice, dcount = 0, flag, f, i;
    char fname[15], dname[15];
    directory dirs[10];
    for (i = 0; i < 10; i++)
        dirs[i].fcount = 0;

    do
    {
        printf("\n1.Create directory\n2.Add file\n3.Display all files\n4.Search Files\n5.Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter the dirname:");
            scanf("%s", dname);
            flag = 0;
            for (int i = 0; i < dcount; i++)
            {
                if (strcmp(dname, dirs[i].name) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                printf("Directory already exists\n");
            else
            {
                strcpy(dirs[dcount].name, dname);
                dcount++;
                printf("Directory added successfully\n");
            }
            break;
        case 2:
            printf("Enter the filename:");
            scanf("%s", fname);
            printf("Enter the dirname:");
            scanf("%s", dname);
            flag = 0;
            for (i = 0; i < dcount; i++)
            {
                if (strcmp(dname, dirs[i].name) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
            {
                f = 0;
                for (int j = 0; j < dirs[i].fcount; j++)
                {
                    if (strcmp(fname, dirs[i].files[j]) == 0)
                    {
                        f = 1;
                        break;
                    }
                }
                if (f == 1)
                    printf("The same file exists in the directory\n");
                else
                {
                    strcpy(dirs[i].files[dirs[i].fcount], fname);
                    dirs[i].fcount++;
                }
            }
            else
            {
                printf("No such directory\n");
            }
            break;
        case 3:
            for (i = 0; i < dcount; i++)
            {
                printf("\t\t%s\n", dirs[i].name);
                for (int j = 0; j < dirs[i].fcount; j++)
                {
                    printf("%s\n", dirs[i].files[j]);
                }
            }
            break;
        case 4:
            printf("Enter the filename:");
            scanf("%s", fname);
            flag = 0;
            for (i = 0; i < dcount; i++)
            {
                for (int j = 0; j < dirs[i].fcount; j++)
                {
                    if (strcmp(fname, dirs[i].files[j]) == 0)
                    {
                        printf("File present in directory %s\n", dirs[i].name);
                        flag = 1;
                        break;
                    }
                }
            }
            if (flag == 0)
                printf("The file '%s' is not present\n", fname);
            break;
        case 5:
            exit(0);
        }
    } while (choice != 5);

    return 0;
}
 