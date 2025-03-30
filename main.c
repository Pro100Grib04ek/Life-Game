#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include <inttypes.h>
#pragma warning(disable : 4996)

#define SIZE 50
#define Count_around(i, j){\
copy_field[i-1][j-1]+copy_field[i-1][j]+copy_field[i-1][j+1]+\
copy_field[i][j-1] +copy_field[i][j+1]+\
copy_field[i+1][j-1]+copy_field[i+1][j]+copy_field[i+1][j+1] \
}
void upgrade_field(uint8_t(*field)[SIZE])
{
	uint8_t copy_field[SIZE][SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			copy_field[i][j] = field[i][j];
		}
	}

	for (int i = 1; i < SIZE - 1; i++)
	{
		for (int j = 1; j < SIZE - 1; j++)
		{
			uint16_t count = Count_around(i, j);
			if (field[i][j] == 0 && count == 3)
			{
				field[i][j] = 1;
			}
			if (field[i][j] == 1 && (count == 2 || count == 3))
			{
				continue;
			}
			else
			{
				field[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < SIZE; i++)
	{
		field[0][i] = 0;
		field[SIZE-1][i] = 0;
		field[i][0];
		field[i][SIZE - 1] = 0;
	}
}

void print_field(uint8_t(*field)[SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (field[i][j] == 0)
			{
				printf(".");
			}
			else
			{
				printf("%"SCNu8, field[i][j]);
			}
		}
		printf("\n");
	}
}

int main()
{
	FILE* file_input = fopen("field.txt", "r");

	uint8_t field[SIZE][SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			field[i][j] = 0;
		}
	}
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			char c;
			fscanf(file_input, "%c", &c);
			if (c == '0' || c == '.') { field[i][j] = 0; }
			if (c == '1') { field[i][j] = 1; }
			if (c == '\n') { j--; }
		}
	}

	while (1)
	{
		print_field(field);
		Sleep(1000);
		upgrade_field(*field);
		system("cls");
	}
}