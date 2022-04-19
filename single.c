#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    int choice, count = 0, flag;
    char fname[15], files[10][15];
    choice = 1;
    do
    {
        printf("\n1.Add file\n2.Display\n3.Search Files\n4.Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter the filename:");
            scanf("%s", fname);
            flag = 0;
            for (int i = 0; i < count; i++)
            {
                if (strcmp(fname, files[i]) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                printf("File already exists\n");
            else
            {
                strcpy(files[count], fname);
                count++;
                printf("File entered successfully\n");
            }
            break;
        case 2:
            if (count == 0)
                printf("No files present\n");
            else
            {
                printf("The files are:\n");
                for (int i = 0; i < count; i++)
                {
                    printf("%s\n", files[i]);
                }
            }
            break;
        case 3: 
            printf("Enter the filename:");
            scanf("%s", fname);
            flag = 0;
            for (int i = 0; i < count; i++)
            {
                if (strcmp(fname, files[i]) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                printf("File '%s' exists\n", fname);
            else
                printf("File '%s' does not exists\n", fname);
            break;
        case 4:
            exit(0);
        }
    } while (choice != 4);
    return 0;
}
