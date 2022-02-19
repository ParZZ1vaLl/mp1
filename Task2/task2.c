#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PAGE_SIZE 45
#define WORD_SIZE 12
#define WORDS_TO_IGNORE_COUNT 3

inline void wrapper()
{
	char c;
	char filename[FILENAME_MAX];
	FILE* file;

	struct PageList
	{
		int page;
		struct PageList* next;
	};

	struct Word
	{
		char* word;
		long long int heuristic;
		int count;
		struct PageList* pages;
	} w;

	//struct WordList;
	struct WordList {
		struct Word data;
		struct WordList* next;
	};

	struct WordList* words = 0;
	struct WordList* current_word = 0;
	struct WordList** current_word_ptr = 0;

	int current_page = 0;
	int current_row = 0;

task_1:
	//открыть файл
	printf("Enter filename\n ");
	gets(filename);
	file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Invalid filename\n ");
		goto task_1;
	}

get_word:
	w.count = 0;
	w.heuristic = 0;
	w.word = malloc(WORD_SIZE * sizeof(char));

	int char_position = 0;

get_char:
	//игнорируем часть слова, которая не влазит
	if (char_position == WORD_SIZE - 1)
	{
		w.word[char_position] = '\0';
		w.heuristic *= pow('z' - 'a', WORD_SIZE - char_position);
	getc: if ((c = fgetc(file)) != ' ' && c != '\n' && c != EOF) goto getc;
		goto process_word;
	}
	//выходим если файл закончился
	if ((c = fgetc(file)) == EOF)
	{
		w.word[char_position] = '\0';
		w.heuristic *= pow('z' - 'a', WORD_SIZE - char_position);
		goto process_word;
	}
	//переход на следующее слово
	if (c == ' ')
	{
		if (char_position == 0)
			goto get_char;
		w.word[char_position] = '\0';
		w.heuristic *= pow('z' - 'a', WORD_SIZE - char_position);
		goto process_word;
	}
	if (c == '\n')
	{
		w.word[char_position] = '\0';
		w.heuristic *= pow('z' - 'a', WORD_SIZE - char_position);
		current_row++;
		if (current_row == PAGE_SIZE)
		{
			current_row = 0;
			current_page++;
		}
		goto process_word;
	}
	//игнорируем символы, которые не буквы
	else if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z'))
	{
		goto get_char;
	}
	//обрабатываем буквы
	else
	{
		//to lower
		if (c >= 'A' && c <= 'Z')
		{
			c -= 'A' - 'a';
		}
		//add char to word
		w.word[char_position] = c;
		w.heuristic *= 'z' - 'a';
		w.heuristic += c - 'a' + 1;
		++char_position;
		goto get_char;
	}


process_word:
	current_word_ptr = &words;
	char index = 0;

	if (w.word[0] == 0)
		goto end_processing_word;

compare_with_word:
	if (*current_word_ptr == 0)
	{
		*current_word_ptr = malloc(sizeof(struct WordList));
		(**current_word_ptr).data.word = w.word;
		(**current_word_ptr).data.count = 1;
		(**current_word_ptr).data.heuristic = w.heuristic;
		(**current_word_ptr).data.pages = malloc(sizeof(struct PageList));
		(*(**current_word_ptr).data.pages).page = current_page;
		(*(**current_word_ptr).data.pages).next = 0;
		(**current_word_ptr).next = 0;
	}
	else
	{
		index = 0;

	compare_chars:
		if (w.word[index] != (**current_word_ptr).data.word[index])
		{
			current_word_ptr = &((**current_word_ptr).next);
			goto compare_with_word;
		}
		else if (w.word[index] == '\0')
		{
			++(**current_word_ptr).data.count;
			struct PageList** it = &((**current_word_ptr).data.pages);
		add_page:
			if (*it == 0)
			{
				(*it) = malloc(sizeof(struct PageList));
				(**it).page = current_page;
				(**it).next = 0;
			}
			else
			{
				if ((**it).page == current_page)
					goto get_word;
				it = &((**it).next);
				goto add_page;
			}
			goto get_word;
		}
		else
		{
			index++;
			goto compare_chars;
		}
	}

end_processing_word:
	if (!feof(file))
		goto get_word;

sort_words:

	//selection sort)))))))))))
	//O = n^2 in any case
	//so cool

	if (words == 0 || (*words).next == 0)
		goto out;

	current_word = words;
	struct WordList* another_word = (*words).next;
	struct Word buff;

sort_iteration:
	if (current_word == 0)
		goto out;
	struct WordList* best_word = current_word;
	another_word = current_word;

select_max:
	another_word = (*another_word).next;
	if (another_word == 0)
		goto swap;
	if ((*another_word).data.heuristic < (*best_word).data.heuristic)
		best_word = another_word;
	goto select_max;
swap:
	buff = (*best_word).data;
	(*best_word).data = (*current_word).data;
	(*current_word).data = buff;

	current_word = (*current_word).next;
	goto sort_iteration;

out:
	current_word = words;

out_next:
	if (current_word != 0)
	{
		if ((*current_word).data.count < 100)
		{
			printf("%s - ", (*current_word).data.word);
			struct PageList* it = (*current_word).data.pages;
		print_page:
			if (it == 0)
				printf("\n");
			else
			{
				printf("%i ", (*it).page);
				it = (*it).next;
				goto print_page;
			}
		}
		current_word = (*current_word).next;
		goto out_next;
	}

end:
	current_word = words;
	if (current_word != 0)
	{
		current_word = (*current_word).next;
		free(words);
		words = current_word;
		goto end;
	}
	return;
}
