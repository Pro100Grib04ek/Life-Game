#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include <inttypes.h>
#include <locale.h>
#include <wchar.h>
#pragma warning(disable : 4996)

int field_size; // Размер поля
uint8_t** field;
// Считает сумму вокруг клетки i, j в матрице
#define Count_around(i, j){\
copy_field[i-1][j-1]+copy_field[i-1][j]+copy_field[i-1][j+1]+\
copy_field[i][j-1] +copy_field[i][j+1]+\
copy_field[i+1][j-1]+copy_field[i+1][j]+copy_field[i+1][j+1] \
} 

// Функция хеширования djb2
unsigned long djb2_hash(char* str) 
{
	unsigned long hash = 5381;  // Начальное значение
	int c;

	// Итеративно вычисляем хеш для каждого символа
	while ((c = *str++)) 
	{
		hash = ((hash << 5) + hash) + c;  // hash * 33 + c
	}

	return hash;
}

void upgrade_field(uint8_t** Field) 
{
	uint8_t** copy_field = (uint8_t**)calloc(field_size, sizeof(uint8_t*));
	for (int i = 0; i < field_size; i++)
	{
		copy_field[i] = (uint8_t*)calloc(field_size, sizeof(uint8_t));
	}
	for (int i = 0; i < field_size; i++) // Создаём копию поля
	{
		for (int j = 0; j < field_size; j++)
		{
			copy_field[i][j] = Field[i][j];
		}
	}

	for (int i = 1; i < field_size - 1; i++) // Проход по внутренней части поля (Без углов)
	{
		uint16_t count; // Сумма вокруг клетки

		// Углы
		{
			// Левый верхний
			count = copy_field[0][1] + copy_field[1][0] + copy_field[1][1];
			if (Field[0][0] == 0 && count == 3)
			{
				Field[0][0] = 1;
			}
			else if (Field[0][0] == 1 && (count == 2 || count == 3))
			{
				Field[0][0] = 1;
			}
			else
			{
				Field[0][0] = 0;
			}

			// Правый верхний
			count = copy_field[0][field_size - 2] + copy_field[1][field_size - 1] + copy_field[1][field_size - 2];
			if (Field[0][field_size - 1] == 0 && count == 3)
			{
				Field[0][field_size - 1] = 1;
			}
			else if (Field[0][field_size - 1] == 1 && (count == 2 || count == 3))
			{
				Field[0][field_size - 1] = 1;
			}
			else
			{
				Field[0][field_size - 1] = 0;
			}

			// Левый нижний
			count = copy_field[field_size - 2][0] + copy_field[field_size - 1][1] + copy_field[field_size - 2][1];
			if (Field[field_size - 1][0] == 0 && count == 3)
			{
				Field[field_size - 1][0] = 1;
			}
			else if (Field[field_size - 1][0] == 1 && (count == 2 || count == 3))
			{
				Field[field_size - 1][0] = 1;
			}
			else
			{
				Field[field_size - 1][0] = 0;
			}

			// Правый нижний
			count = copy_field[field_size-1][field_size-2] + copy_field[field_size-2][field_size-1] + copy_field[field_size-2][field_size-2];
			if (Field[field_size - 1][field_size - 1] == 0 && count == 3)
			{
				Field[field_size - 1][field_size - 1] = 1;
			}
			else if (Field[field_size - 1][field_size - 1] == 1 && (count == 2 || count == 3))
			{
				Field[field_size - 1][field_size - 1] = 1;
			}
			else
			{
				Field[field_size - 1][field_size - 1] = 0;
			}
		}
		// Границы
		{
			// Верхняя 
			{
				count = copy_field[0][i - 1] + copy_field[0][i + 1] + copy_field[1][i - 1] + copy_field[1][i] + copy_field[1][i + 1];
				if (Field[0][i] == 0 && count == 3)
				{
					Field[0][i] = 1;
				}
				if (Field[0][i] == 1 && (count == 2 || count == 3))
				{
					Field[0][i] = 1;
				}
				else
				{
					Field[0][i] = 0;
				}
			}
			// Правая 
			{
				count = copy_field[i - 1][field_size - 1] + copy_field[i + 1][field_size - 1] + copy_field[i - 1][field_size - 2] + copy_field[i][field_size - 2] + copy_field[i + 1][field_size - 2];
				if (Field[i][field_size - 1] == 0 && count == 3)
				{
					Field[i][field_size - 1] = 1;
				}
				if (Field[i][field_size - 1] == 1 && (count == 2 || count == 3))
				{
					Field[i][field_size - 1] = 1;
				}
				else
				{
					Field[i][field_size - 1] = 0;
				}
			}
			// Нижняя 
			{
				count = copy_field[field_size - 1][i - 1] + copy_field[field_size - 1][i + 1] + copy_field[field_size - 2][i - 1] + copy_field[field_size - 2][i] + copy_field[field_size - 2][i + 1];
				if (Field[field_size - 1][i] == 0 && count == 3)
				{
					Field[field_size - 1][i] = 1;
				}
				if (Field[field_size - 1][i] == 1 && (count == 2 || count == 3))
				{
					Field[field_size - 1][i] = 1;
				}
				else
				{
					Field[field_size - 1][i] = 0;
				}
			}

			// Левая 
			{
				count = copy_field[i - 1][0] + copy_field[i + 1][0] + copy_field[i - 1][1] + copy_field[i][1] + copy_field[i + 1][1];
				if (Field[i][0] == 0 && count == 3)
				{
					Field[i][0] = 1;
				}
				if (Field[i][0] == 1 && (count == 2 || count == 3))
				{
					Field[i][0] = 1;
				}
				else
				{
					Field[i][0] = 0;
				}
			}
		}
		// Внутренняя часть
		for (int j = 1; j < field_size - 1; j++)
		{
			uint16_t count = Count_around(i, j);
			if (Field[i][j] == 0 && count == 3)
			{
				Field[i][j] = 1;
			}
			if (Field[i][j] == 1 && (count == 2 || count == 3))
			{
				continue;
			}
			else
			{
				Field[i][j] = 0;
			}
		}
	}
	/* Тут было зануление границ, но оно как будто бы не нужно
	for (int i = 0; i < field_size; i++)
	{
		Field[0][i] = 0;
		Field[field_size-1][i] = 0;
		Field[i][0] = 0;
		Field[i][field_size - 1] = 0;
	}
	*/
	free(copy_field);
}

void print_field(uint8_t** field)
{
	for (int i = 0; i < field_size; i++)
	{
		for (int j = 0; j < field_size; j++)
		{
			if (field[i][j] == 0)
			{
				printf(".");
			}
			else
			{
				printf("o");
			}
		}
		printf("\n");
	}
}
void fprint_field(FILE* out, uint8_t** field)
{
	for (int i = 0; i < field_size; i++)
	{
		for (int j = 0; j < field_size; j++)
		{
			if (field[i][j] == 0)
			{
				fprintf(out, ".");
			}
			else
			{
				fprintf(out, "o");
			}
		}
		fprintf(out, "\n");
	}
}


int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "rus");
	//setlocale(LC_ALL, "en_US.UTF-8");
	SetConsoleOutputCP(CP_UTF8);
	char command[10];
	printf("pattern | rand | find\n");
	scanf("%s", &command);
	int live_cells; // Вероятность живой клетки
	int iter = 0; // Номер итерации
	/*Заполнение поля (будет отличаться от способа запуска)*/
	if (!strcmp(command, "pattern"))
	{
		FILE* file_input = fopen("field.txt", "r");
		printf("Pattern_size: ");
		scanf("%d", &field_size);
		printf("\n");
		field = (uint8_t**)calloc(field_size, sizeof(uint8_t*));
		for (int i = 0; i < field_size; i++)
		{
			field[i] = (uint8_t*)calloc(field_size, sizeof(uint8_t));
		}
		for (int i = 0; i < field_size; i++)
		{
			for (int j = 0; j < field_size; j++)
			{
				char c;
				fscanf(file_input, "%c", &c);
				if (c == '0' || c == '.') { field[i][j] = 0; }
				if (c == '1' || c == 'o') { field[i][j] = 1; }
				if (c == '\n') { j--; }
			}
		}
		fclose(file_input);
	}
	if (!strcmp(command, "rand")) // Задаём параметры рандомной генерации  ручками
	{
		FILE* out = fopen("field.txt", "w"); // Запишем сюда случайное поле)
		printf("Enter generation patterns:\n");
		printf("Field_size: ");
		scanf("%d", &field_size);
		printf("\nLiving cells (\%): ");
		scanf("%d", &live_cells);

		field = (uint8_t**)malloc(field_size * sizeof(uint8_t*));
		for (int i = 0; i < field_size; i++)
		{
			field[i] = (uint8_t*)malloc(field_size * sizeof(uint8_t));
		}
		for (int i = 0; i < field_size; i++)
		{
			for (int j = 0; j < field_size; j++)
			{
				if (rand() % 100 < live_cells)
				{
					field[i][j] = 1;
					fprintf(out, "o");
				}
				else
				{
					field[i][j] = 0;
					fprintf(out, ".");
				}
			}
			fprintf(out, "\n");
		}
		fclose(out);
	}
	
	// Ищем начальные положения с циклом определенной длины
	if (!strcmp(command, "find"))
	{
		int try_iter = 0; // Номер попытки при поиске
		int cycle_len = 0;
		printf("Enter generation patterns:\n");
		printf("Field_size: ");
		scanf("%d", &field_size);
		printf("\nLiving cells (\\%): ");
		scanf("%d", &live_cells);
		printf("\n");
		printf("Cycle len: ");
		scanf("%d", &cycle_len);
		field = (uint8_t**)malloc(field_size * sizeof(uint8_t*));
		for (int i = 0; i < field_size; i++)
		{
			field[i] = (uint8_t*)malloc(field_size * sizeof(uint8_t));
		}
	start:
		try_iter++;

		for (int i = 0; i < field_size; i++)
		{
			for (int j = 0; j < field_size; j++)
			{
				if (rand() % 100 < live_cells)
				{
					field[i][j] = 1;
				}
				else
				{
					field[i][j] = 0;
				}
			}
		}

		/*Обнаружение цикла для find*/
		char* field_line = (char*)malloc(sizeof(char) * (field_size * field_size + 1));
		unsigned long hash;
		int ind = 0;
		unsigned long* hashes = (unsigned long*)malloc(sizeof(unsigned long));
		uint8_t** copy_field = (uint8_t**)malloc(sizeof(uint8_t*) * field_size);
		for (int i = 0; i < field_size; i++)
		{
			copy_field[i] = (uint8_t*)malloc(sizeof(uint8_t) * field_size);
		}
		for (int i = 0; i < field_size; i++)
			for (int j = 0; j < field_size; j++)
			{
				copy_field[i][j] = field[i][j];
			}

		while (1)
		{
			int pos = 0;
			for (int i = 0; i < field_size; i++)
			{
				for (int j = 0; j < field_size; j++)
				{
					field_line[pos++] = copy_field[i][j] ? '1' : '0';
				}
			}
			hash = djb2_hash(field_line);
			for (int i = 0; i < ind; i++)
			{
				if (hash == hashes[i])
				{
					if (ind-i >= cycle_len)
					{
						FILE* cycle = fopen("cycle.txt", "w");
						fprintf(cycle, "Итерация: %d\n", ind + 1);
						fprintf(cycle, "Длина цикла: %d\n", ind - i);
						//cycle_iter = ind + 1;
						fprint_field(cycle, copy_field);
						fclose(cycle);
						FILE* out = fopen("field.txt", "w"); // Запишем сюда  поле
						fprint_field(out, field);
						fclose(out);
						return;
					}
					else
					{
						if (try_iter % 500 == 0)
						{
							printf("%d patterns tried\n", try_iter);
						}
						free(field_line);
						free(copy_field);
						goto start;
					}
				}
			}
			hashes[ind] = hash;
			ind++;
			hashes = (unsigned long*)realloc(hashes, (ind + 1) * sizeof(unsigned long));
			field_line[0] = '\0';
			upgrade_field(copy_field);
		}
	}

	{
		/* Обнаружение цикла */
		int cycle_iter = 0; // Номер итерации, на которой был найден цикл
		int cycle_len = 1;
		char* field_line = (char*)malloc(sizeof(char) * (field_size * field_size + 1));
		unsigned long hash;
		int ind = 0;
		unsigned long* hashes = (unsigned long*)malloc(sizeof(unsigned long));
		uint8_t** copy_field = (uint8_t**)malloc(sizeof(uint8_t*) * field_size);
		for (int i = 0; i < field_size; i++)
		{
			copy_field[i] = (uint8_t*)malloc(sizeof(uint8_t) * field_size);
		}
		for (int i = 0; i < field_size; i++)
			for (int j = 0; j < field_size; j++)
			{
				copy_field[i][j] = field[i][j];
			}

		while (1)
		{
			int pos = 0;
			for (int i = 0; i < field_size; i++)
			{
				for (int j = 0; j < field_size; j++)
				{
					field_line[pos++] = copy_field[i][j] ? '1' : '0';
				}
			}
			hash = djb2_hash(field_line);
			for (int i = 0; i < ind; i++)
			{
				if (hash == hashes[i])
				{
					//printf("Цикл обнаружен!\n");
					FILE* cycle = fopen("cycle.txt", "w");
					fprintf(cycle, "Итерация: %d\n", ind + 1);
					fprintf(cycle, "Длина цикла: %d\n", ind - i);
					cycle_iter = ind + 1;
					cycle_len = ind - i;
					if (cycle_len <= 50)
					{
						for (int j = 0; j < cycle_len; j++)
						{
							fprintf(cycle, "iter: %d\n", j + 1);
							fprint_field(cycle, copy_field);
							fprintf(cycle, "\n");
							upgrade_field(copy_field);
						}
					}
					else
					{
						fprint_field(cycle, copy_field);
					}
					fclose(cycle);
					goto play;
				}
			}
			hashes[ind] = hash;
			ind++;
			hashes = (unsigned long*)realloc(hashes, (ind + 1) * sizeof(unsigned long));
			field_line[0] = '\0';
			upgrade_field(copy_field);
		}
		/*Сама игра*/
	play:
		while (1)
		{
			print_field(field);
			upgrade_field(field);
			iter++;
			if (cycle_iter - iter -1 >= 0)
			{
				printf("До цикла осталось: %d", cycle_iter - iter);
			}
			else
			{
				printf("Цикл: %d", (iter-1)%cycle_len + 1);
			}
			Sleep(600);
			system("cls");
		}
	} 
}