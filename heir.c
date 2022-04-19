#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tabs = 0;
typedef struct element
{
    char name[20];
    int type, nelements;
    struct element *link[5];
} element;

void addelement(element *dir)
{
    int ne, ch;
    element *temp;
    printf("Enter the no.of elements in %s:", dir->name);
    scanf("%d", &ne);
    for (int j = 0; j < ne; j++)
    {
        printf("1.Add file\n2.Add directory\n");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            temp = (element *)malloc(sizeof(element));
            temp->nelements = 0;
            temp->type = 0;
            printf("Enter name of file:");
            scanf("%s", temp->name);
            dir->link[dir->nelements] = temp;
            dir->nelements++;
            break;
        case 2:
            temp = (element *)malloc(sizeof(element));
            temp->nelements = 0;
            temp->type = 1;
            printf("Enter name of directory:");
            scanf("%s", temp->name);
            dir->link[dir->nelements] = temp;
            dir->nelements++;
            addelement(temp);
            break;
        }
    }
}

void display(element *e, int t)
{
    for (int i = 0; i < t; i++)
        printf("\t");
    printf("%s\n", e->name);

    if (e->type == 1)
    {
        //printf("attii");
        for (int i = 0; i < e->nelements; i++)
        {
            display(e->link[i], t + 1);
        }
    }
}

int search(element *e, char name[20], int r)
{
    if (r == 0)
        if (strcmp(e->name, name) == 0)
        {
            if (e->type == 0)
                printf("File %s found.\n", e->name);
            else
                printf("Directory %s found.\n", e->name);
            r = 1;
            return r;
        }
        else
        {
            for (int i = 0; i < e->nelements; i++)
            {
                r = search(e->link[i], name, 0);
            }
        }
    return r;
}

int main()
{
    int ne;
    int ch;
    element *root = (element *)malloc(sizeof(element));
    strcpy(root->name, "root");
    root->type = 1;
    element *temp;
    int nuser;
    char samplename[20];
    printf("Enter no.of users:");
    scanf("%d", &nuser);
    root->nelements = nuser;
    for (int i = 0; i < nuser; i++)
    {
        printf("Enter name of %d user:", i + 1);
        scanf("%s", samplename);
        temp = (element *)malloc(sizeof(element));
        temp->nelements = 0;
        temp->type = 1;
        strcpy(temp->name, samplename);
        root->link[i] = temp;
        addelement(temp);
    }
    do
    {
        printf("1.Display all\n2.Search\n3.End\n");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            display(root, 0);
            break;
        case 2:
            printf("Enter the filename:");
            scanf("%s", samplename);
            search(root, samplename, 0);
            break;
        }
    } while (ch != 3);

    return 0;
}
 