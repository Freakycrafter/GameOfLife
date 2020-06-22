#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

/**
*   Game of Life
*   Author: Daniel Netzlaff, Marc Bosau
*   Date: 19.05.20
**/

int _width;         //width of the field, global var
int _length;        //length of the field, global var
int _ruleDeadMax;   //max amount of neighbors for a dead cell to become alive, global var
int _ruleDeadMin;   //min amount of neighbors for a dead cell to become alive, global var
int _ruleLifeMax;   //max amount of neighbors for a live cell to stay alive, global var
int _ruleLifeMin;   //min amount of neighbors for a live cell to stay alive, global var
char _iconDead;     //char for dead cells, global var
char _iconLive;     //char for live cells, global var
int _fps;           //iterations per second, global var
int _cycle = 0;     //current iteration, global var
FILE *_file;        //the export/import file pointer, global var

/**
*   This method exports the field values and width/length of the field to the "export.txt" file.
**/

void exportValues(int field[_length][_width])
{
    _file = fopen("export.txt", "w");
    fprintf(_file, "%i\n", _length);
    fprintf(_file, "%i\n", _width);
    for(int i=0; i<_length; i++)
    {
        for(int j=0; j<_width; j++)
        {
            fprintf(_file, "%i", field[i][j]);
        }
        fprintf(_file, "\n");
    }
    fclose(_file);
}
/**
*   This method imports only the dimensions, as the field cant be imported without being declared yet.
**/
void importDimensions()
{
    char buffer[30], c;
    _file = fopen("export.txt", "r");
    _length = atoi(fgets(buffer, 30, _file));
    _width = atoi(fgets(buffer, 30, _file));
    printf("%i", _length);
    printf("%i", _width);
}
/**
*   This method imports the field values into the field.
**/

int importField(int *field[_length][_width])
{
    for(int i=0; i<_length; i++)
    {
        for(int j=0; j<_width; j++)
        {
            field[i][j] = getc(_file);
        }
    }
}

/**
*   This method checks the previous methods for the exact same configuration.
**/

void checkPreviousIterations(int field[_length][_width], int *lastIterations[1000][_length][_width])
{
    int same = 0;
    for(int i=0; i<_cycle; i++)
    {
        same = 1;
        for(int j=0; j<_length; j++)
        {
            for(int k=0; k<_width; k++)
            {
                if(field[j][k] != lastIterations[i][j][k])
                {
                    same = 0;
                }
            }
        }
    }
    for(int i=0; i<_length; i++)
    {
        for(int j=0; j<_width; j++)
        {
            lastIterations[_cycle % 1000][i][j] = field[i][j];
        }
    }
    if(same)
    {
        printf("Loop in Iterationen gefunden!");
    }
    _cycle++;
}

/**
*   This method checks whether an individual cell should live in the next cycle or not.
**/

int checkForLife(int width, int length, int alive, int field[_length][_width])
{
    int aliveNeighbors = 0;
    for(int i=-1; i<2; i++)
    {
        for(int j=-1; j<2; j++)
        {
            if(width + i >= 0 && width + i < _width && length + j >= 0 && length + i < _length)
            {
                aliveNeighbors += field[length+i][width+j];
            }
        }
    }
    aliveNeighbors -= alive;
    if(alive)
    {
        if(aliveNeighbors <= _ruleLifeMax && aliveNeighbors >= _ruleLifeMin)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(aliveNeighbors <= _ruleDeadMax && aliveNeighbors >= _ruleDeadMin)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/**
*   This method generates a completely random field.
**/

void randomField(int *field[_length][_width])
{
    for(int i=0; i<_length; i++)
    {
        for(int j=0; j<_width; j++)
        {
            field[i][j] = rand() % 2;
        }
    }
}

/**
*   This method updates the whole field.
**/

void updateTick(int *field[_length][_width])
{
    int preField[_length][_width];
    for(int i=0; i<_length; i++)
    {
        for(int j=0; j<_width; j++)
        {
            preField[i][j] = field[i][j];
        }
    }
    for(int i=0; i<_length; i++)
    {
        for(int j=0; j<_width; j++)
        {
            field[i][j] = checkForLife(i, j, preField[i][j], preField);
        }
    }
}

/**
*   This method prints out the field.
**/

void printField(int field[_length][_width])
{
    printf("\n");
    printf("%ite Iteration", _cycle);
    printf("\n");
    for(int i=0; i<_length; i++)
    {
        for(int j=0; j<_width; j++)
        {
            if(field[i][j] == 1)
            {
                printf("%c", _iconLive);
            }
            else if(field[i][j] == 0)
            {
                printf("%c", _iconDead);
            }
        }
        printf("\n");
    }
}

/**
*   This method checks whether a char array is a number or not.
**/

int isNumber(char input[100])
{
    int inputLength = strlen(input);
    for (int i=0; i<inputLength; i++)
    {
        if (!isdigit(input[i]))
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    char input[100];
    while(input[0] != 'Y' && input[0] != 'y' && input[0] != 'N' && input[0] != 'n')
    {
        printf("\nImportieren? Y für ja, n für nein.\n");
        scanf("%s", &input[0]);
        fflush(stdin);
    }
    if(input[0] == 'Y' || input[0] == 'y')
    {
        importDimensions();
    }
    else
    {
        do
        {
            printf("Wie hoch soll das Feld sein?\n");
            scanf("%s", &input);
            fflush(stdin);
        }
        while(!isNumber(input));
        _length = atoi(input);
        do
        {
            printf("\nWie breit soll das Feld sein?\n");
            scanf("%s", &input);
            fflush(stdin);
        }
        while(!isNumber(input));
        _width = atoi(input);
    }
    int field[_length][_width];
    int lastIterations[1000][_length][_width];
    if(input[0] == 'Y' || input[0] == 'y')
    {
        importField(&field);
    }
    printf("\nWelches Zeichen soll für tote Zellen benutzt werden?\n");
    scanf("%c", &_iconDead);
    fflush(stdin);
    printf("\nWelches Zeichen soll für lebende Zellen benutzt werden?\n");
    scanf("%c", &_iconLive);
    fflush(stdin);
    do
    {
        printf("\nWie viele Zellen neben einer toten Zelle müssen mindestens leben, damit die Zelle lebt?\n");
        scanf("%s", &input);
        fflush(stdin);
    }
    while(!isNumber(input));
    _ruleDeadMin = atoi(input);
    do
    {
        printf("\nWie viele Zellen neben einer toten Zelle dürfen maximal leben, damit die Zelle lebt?\n");
        scanf("%c", &input[0]);
        fflush(stdin);
    }
    while(!isNumber(input));
    _ruleDeadMax = atoi(input);
    do
    {
        printf("\nWie viele Zellen neben einer lebendigen Zelle müssen mindestens leben, damit die Zelle am Leben bleibt?\n");
        scanf("%c", &input[0]);
        fflush(stdin);
    }
    while(!isNumber(input));
    _ruleLifeMin = atoi(input);
    do
    {
        printf("\nWie viele Zellen neben einer lebendigen Zelle müssen mindestens leben, damit die Zelle am Leben bleibt?\n");
        scanf("%c", &input[0]);
        fflush(stdin);
    }
    while(!isNumber(input));
    _ruleLifeMax = atoi(input);
    while(input[0] != 'M' && input[0] != 'm' && input[0] != 'A' && input[0] != 'a')
    {
        printf("\nManuelle Updates oder automatische Updates? M für manuell, A für automatisch.\n");
        scanf("%c", &input[0]);
        fflush(stdin);
    }
    printf("\nJederzeit kann x zum exportieren oder q zum schließen gedrückt werden.\n");
    if(input[0] == 'A' || input[0] == 'a')
    {
        printf("\nWie viele Updates pro Sekunde?\n");
        scanf("%i", &_fps);
        fflush(stdin);
        randomField(&field);
        while(1)
        {
            while(!kbhit())
            {
                printField(field);
                updateTick(&field);
                checkPreviousIterations(field, &lastIterations);
                usleep(round(1000000/_fps));
            }
            char inChar = getch();
            if(inChar == 'q' || inChar == 'Q')
            {
                return 1;
            }
            if(inChar == 'x' || inChar == 'X')
            {
                exportValues(field);
            }
        }
    }
    else if(input[0] == 'M' || input[0] == 'm')
    {
        printf("\nBeliebige Taste für die nächste Iteration drücken.\n");
        randomField(&field);
        while(1)
        {
            while(!kbhit())
            {
            }
            printField(field);
            updateTick(&field);
            checkPreviousIterations(field, &lastIterations);
            char inChar = getch();
            if(inChar == 'q' || inChar == 'Q')
            {
                return 1;
            }
            if(inChar == 'x' || inChar == 'X')
            {
                exportValues(field);
            }
        }
    }
    return 1;
}
