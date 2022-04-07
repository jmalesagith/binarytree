#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct vertex
{
    int klucz;
    struct vertex *lewy;
    struct vertex *prawy;
} vertex, *pvertex;

pvertex InsertBST(pvertex t, int k)
{
    if (t == NULL)
    {
        pvertex w = (pvertex) malloc(sizeof(vertex));
        w->klucz = k;
        w->lewy = NULL;
        w->prawy = NULL;
        return w;
    }

    if (k <= t->klucz)
        t->lewy = InsertBST(t->lewy, k);
    else
        t->prawy = InsertBST(t->prawy, k);

    return t;
}

int NumberOfLeaves(pvertex t)
{
    if (t == NULL)
        return 0;

    if (t->lewy == NULL && t->prawy == NULL)
        return 1;

    return NumberOfLeaves(t->lewy) + NumberOfLeaves(t->prawy);
}

pvertex DeleteMaxOfBST(pvertex t, int *deleted_value)
{
    // zakladam, ze drzewo t jest nie puste,
    // ale na wszelki przypadek ...
    if (t == NULL)
    {
        *deleted_value = -1;
        return NULL;
    }

    if (t->prawy == NULL)
    {
        *deleted_value = t->klucz;
        pvertex w = t->lewy;
        free(t);
        return w;
    }

    t->prawy = DeleteMaxOfBST(t->prawy, deleted_value);
    return t;
}

pvertex DeleteNodeOfBST(pvertex t, int k)
{
    if (t == NULL)
        return NULL;

    if (k < t->klucz)
    {
        t->lewy = DeleteNodeOfBST(t->lewy, k);
        return t;
    }
    else if (k > t->klucz)
    {
        t->prawy = DeleteNodeOfBST(t->prawy, k);
        return t;
    }
    else if (t->lewy == NULL)
    {
        pvertex w = t->prawy;
        free(t);
        return w;
    }
    else
    {
        int max_left;
        t->lewy = DeleteMaxOfBST(t->lewy, &max_left);
        t->klucz = max_left;
        return t;
    }
}

#define SCREEN_HEIGHT  100
#define SCREEN_WIDTH    80

typedef struct
{
    int numberOfLines;
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH+2]; // 100 linii, kazda linia ma 80 znakow plus \n i \0
} TreeDisplay;

#define DOWN                    '|'
#define LEFT                    '/'
#define RIGHT                   '\\'
#define RIGHT_DOWN              '.'
#define DOWN_LEFT_AND_RIGHT     '^'
#define LEFT_DOWN               '.'
#define DASH                    '-'

void WriteSymbolToTreeDisplay(TreeDisplay* td, char c, int row, int col)
{
    td->screen[row][col] = c;

    if (row+1 > td->numberOfLines)
        td->numberOfLines = row+1;
}

void WritekluczToTreeDisplay(TreeDisplay* td, int klucz, int row, int col)
{
    if (klucz >= 1000)
        td->screen[row][col-1] = td->screen[row][col] = td->screen[row][col+1] = '.'; // za duza wartosc
    else if (klucz >= 100)
    {
        td->screen[row][col-1] = (klucz / 100) + '0';
        td->screen[row][col] = ((klucz / 10) % 10) + '0';
        td->screen[row][col+1] = (klucz % 10) + '0';
    }
    else if (klucz >= 10)
    {
        td->screen[row][col] = (klucz / 10) + '0';
        td->screen[row][col+1] = (klucz % 10) + '0';
    }
    else if (klucz >= 0)
        td->screen[row][col] = klucz + '0';
    else
    {
        td->screen[row][col] = '-';
        td->screen[row][col+1] = 'X';
    }

    if (row+1 > td->numberOfLines)
        td->numberOfLines = row+1;
}

int UpdateTreeDisplay(TreeDisplay* td, pvertex t, int depth, int left, int right)
// wypisz drzewo t do ekranu td w obszarze: pionowo od depth do SCREEN_HEIGHT, a poziomo od left do right;
// funkcja zwraca pozioma pozycje korzenia drzewa
{
    if (t == NULL)
        return -1;

    int left_width = NumberOfLeaves(t->lewy);
    int right_width = NumberOfLeaves(t->prawy);

    if (left_width + right_width == 0)
    {
        int middle = (left+right)/2;
        WritekluczToTreeDisplay(td, t->klucz, depth, middle);
        return middle;
    }

    if (left_width == 0)
    {
        int middle = UpdateTreeDisplay(td, t->prawy, depth+2, left, right);
        WritekluczToTreeDisplay(td, t->klucz, depth, middle);
        WriteSymbolToTreeDisplay(td, RIGHT, depth+1, middle);
        return middle;
    }

    if (right_width == 0)
    {
        int middle = UpdateTreeDisplay(td, t->lewy, depth+2, left, right);
        WritekluczToTreeDisplay(td, t->klucz, depth, middle);
        WriteSymbolToTreeDisplay(td, LEFT, depth+1, middle);
        return middle;
    }

    int k = (right - left - 2) / (left_width + right_width);
    int l = UpdateTreeDisplay(td, t->lewy, depth+2, left, left + left_width*k - 1);
    int r = UpdateTreeDisplay(td, t->prawy, depth+2, left + left_width*k + 1, right-1);

    int middle = (l+r)/2;
    WritekluczToTreeDisplay(td, t->klucz, depth, middle);

    WriteSymbolToTreeDisplay(td, DOWN_LEFT_AND_RIGHT, depth+1, middle);
    WriteSymbolToTreeDisplay(td, LEFT_DOWN, depth+1, l);
    WriteSymbolToTreeDisplay(td, RIGHT_DOWN, depth+1, r);

    int i;
    for (i = l+1; i < middle; i++)
        WriteSymbolToTreeDisplay(td, DASH, depth+1, i);
    for (i = middle+1; i < r; i++)
        WriteSymbolToTreeDisplay(td, DASH, depth+1, i);

    return middle;
}




int LiczLiscie(pvertex t)
{
    if (!t)
        return 0;

    int x=LiczLiscie(t->lewy);
    int y=LiczLiscie(t->prawy);
    if(x==0&&y==0)
    {
        return 1;
    }
    if(x-y>=-1&&x-y<=1&&x>=0)
        return x+y;

    return -2;
}




void DisplayTree(pvertex t)
{
    TreeDisplay TD;
    TD.numberOfLines = 0;
    int i, j;
    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
            TD.screen[i][j] = ' '; // spacja
        TD.screen[i][SCREEN_WIDTH] = '\n';
        TD.screen[i][SCREEN_WIDTH+1] = '\0';
    }

    UpdateTreeDisplay(&TD, t, 0, 1, SCREEN_WIDTH - 1);

    for (i = 0; i < TD.numberOfLines; i++)
        printf("%s", TD.screen[i]);
    printf("\n");
}

int main()
{
    int i,a,k,l;
    srand (time(NULL));

    pvertex t = NULL;
    for (i = 0; i < 10; i++)
        t = InsertBST(t, rand() % 1000);

    DisplayTree(t);
    do
    {
        k=LiczLiscie(t);
        printf("%d \n",k);
        if(k>=0)
        {
            printf("Drzewo zbalansowane \n");
        }
        else
        {
            printf("Drzewo niezbalansowane\n");
        }
        printf("Podaj klucz elementu do usuniecia: ");
        scanf("%d", &a);

        if (a == -1)
            break;

        t = DeleteNodeOfBST(t, a);

        printf("Po usunieciu %d:\n", a);
        DisplayTree(t);
    }
    while (1);

    return 0;
}
