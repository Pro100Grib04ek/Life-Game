#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include <inttypes.h>
#pragma warning(disable : 4996)

#define SIZE 50 // Размер поля

// Считает сумму вокруг клетки i, j в матрице
#define Count_around(i, j){\
copy_field[i-1][j-1]+copy_field[i-1][j]+copy_field[i-1][j+1]+\
copy_field[i][j-1] +copy_field[i][j+1]+\
copy_field[i+1][j-1]+copy_field[i+1][j]+copy_field[i+1][j+1] \
} 

void upgrade_field(uint8_t(*field)[SIZE]) 
{
	uint8_t copy_field[SIZE][SIZE];
	for (int i = 0; i < SIZE; i++) // Создаём копию поля
	{
		for (int j = 0; j < SIZE; j++)
		{
			copy_field[i][j] = field[i][j];
		}
	}

	for (int i = 1; i < SIZE - 1; i++) 
	{
		uint16_t count; // Сумма вокруг клетки

		// Углы
		{
			// Левый верхний
			count = copy_field[0][1] + copy_field[1][0] + copy_field[1][1];
			if (field[0][0] == 0 && count == 3)
			{
				field[0][0] = 1;
			}
			else if (field[0][0] == 1 && (count == 2 || count == 3))
			{
				field[0][0] = 1;
			}
			else
			{
				field[0][0] = 0;
			}

			// Правый верхний
			count = copy_field[0][SIZE - 2] + copy_field[1][SIZE - 1] + copy_field[1][SIZE - 2];
			if (field[0][SIZE - 1] == 0 && count == 3)
			{
				field[0][SIZE - 1] = 1;
			}
			else if (field[0][SIZE - 1] == 1 && (count == 2 || count == 3))
			{
				field[0][SIZE - 1] = 1;
			}
			else
			{
				field[0][SIZE - 1] = 0;
			}

			// Левый нижний
			count = copy_field[SIZE - 2][0] + copy_field[SIZE - 1][1] + copy_field[SIZE - 2][1];
			if (field[SIZE - 1][0] == 0 && count == 3)
			{
				field[SIZE - 1][0] = 1;
			}
			else if (field[SIZE - 1][0] == 1 && (count == 2 || count == 3))
			{
				field[SIZE - 1][0] = 1;
			}
			else
			{
				field[SIZE - 1][0] = 0;
			}

			// Правый нижний
			count = copy_field[SIZE-1][SIZE-2] + copy_field[SIZE-2][SIZE-1] + copy_field[SIZE-2][SIZE-2];
			if (field[SIZE - 1][SIZE - 1] == 0 && count == 3)
			{
				field[SIZE - 1][SIZE - 1] = 1;
			}
			else if (field[SIZE - 1][SIZE - 1] == 1 && (count == 2 || count == 3))
			{
				field[SIZE - 1][SIZE - 1] = 1;
			}
			else
			{
				field[SIZE - 1][SIZE - 1] = 0;
			}
		}
		// Границы
		{
			// Верхняя 
			{
				count = copy_field[0][i - 1] + copy_field[0][i + 1] + copy_field[1][i - 1] + copy_field[1][i] + copy_field[1][i + 1];
				if (field[0][i] == 0 && count == 3)
				{
					field[0][i] = 1;
				}
				if (field[0][i] == 1 && (count == 2 || count == 3))
				{
					field[0][i] = 1;
				}
				else
				{
					field[0][i] = 0;
				}
			}
			// Правая 
			{
				count = copy_field[i - 1][SIZE - 1] + copy_field[i + 1][SIZE - 1] + copy_field[i - 1][SIZE - 2] + copy_field[i][SIZE - 2] + copy_field[i + 1][SIZE - 2];
				if (field[i][SIZE - 1] == 0 && count == 3)
				{
					field[i][SIZE - 1] = 1;
				}
				if (field[i][SIZE - 1] == 1 && (count == 2 || count == 3))
				{
					field[i][SIZE - 1] = 1;
				}
				else
				{
					field[i][SIZE - 1] = 0;
				}
			}
			// Нижняя 
			{
				count = copy_field[SIZE - 1][i - 1] + copy_field[SIZE - 1][i + 1] + copy_field[SIZE - 2][i - 1] + copy_field[SIZE - 2][i] + copy_field[SIZE - 2][i + 1];
				if (field[SIZE - 1][i] == 0 && count == 3)
				{
					field[SIZE - 1][i] = 1;
				}
				if (field[SIZE - 1][i] == 1 && (count == 2 || count == 3))
				{
					field[SIZE - 1][i] = 1;
				}
				else
				{
					field[SIZE - 1][i] = 0;
				}
			}

			// Левая 
			{
				count = copy_field[i - 1][0] + copy_field[i + 1][0] + copy_field[i - 1][1] + copy_field[i][1] + copy_field[i + 1][1];
				if (field[i][0] == 0 && count == 3)
				{
					field[i][0] = 1;
				}
				if (field[i][0] == 1 && (count == 2 || count == 3))
				{
					field[i][0] = 1;
				}
				else
				{
					field[i][0] = 0;
				}
			}
		}
		// Внутренняя часть
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
	/* Тут было зануление границ, но оно как будто бы не нужно
	for (int i = 0; i < SIZE; i++)
	{
		field[0][i] = 0;
		field[SIZE-1][i] = 0;
		field[i][0] = 0;
		field[i][SIZE - 1] = 0;
	}
	*/
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
				printf("o");
				//printf("%"SCNu8, field[i][j]);
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
			if (c == '1' || c == 'o') { field[i][j] = 1; }
			if (c == '\n') { j--; }
		}
	}

	while (1)
	{
		print_field(field);
		Sleep(650);
		upgrade_field(*field);
		system("cls");
	}
}