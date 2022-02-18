#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>

#define WORD_SIZE 25
#define WORDS_TO_IGNORE_COUNT 3

inline void wrapper()
{
	char c;
	char filename[FILENAME_MAX];
	FILE* file;

	struct Word
	{
		char* word;
		int count;
	} w;

	struct WordList;
	struct WordList {
		struct Word data;
		struct WordList* next;
	};

	struct WordList* words = 0;
	struct WordList* current_word = 0;
	struct WordList** current_word_ptr = 0;

	char* words_to_ignore[WORDS_TO_IGNORE_COUNT] = {
		"the",
		"a",
		"an"
	};

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
	w.word = malloc(WORD_SIZE * sizeof(char));

	int char_position = 0;

get_char:
	//вылетаем, если слова какие-то длинные
	if (char_position == WORD_SIZE)
	{
		printf("Words in text is longer than I expected 0_0\n");
		goto end;
	}
	//выходим если файл закончился
	if ((c = fgetc(file)) == EOF)
	{
		w.word[char_position] = '\0';
		goto process_word;
	}
	//переход на следующее слово
	if (c == ' ' )
	{
		if (char_position == 0)
			goto get_char;
		w.word[char_position] = '\0';
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
		++char_position;
		goto get_char;
	}


process_word: 
	current_word_ptr = &words;
	char index = 0;
	char index2 = 0;

compare_with_stop_words:
	if (index == WORDS_TO_IGNORE_COUNT)
		goto compare_with_word;
	else if (w.word[index2] != words_to_ignore[index][index2])
	{
		index++;
		index2 = 0;
		goto compare_with_stop_words;
	}
	else if (w.word[index2] == '\0')
		goto end_processing_word;
	else
	{
		index2++;
		goto compare_with_stop_words;
	}

compare_with_word:
	if (*current_word_ptr == 0)
	{
		*current_word_ptr = malloc(sizeof(struct WordList));
		(**current_word_ptr).data.word = w.word; 
		(**current_word_ptr).data.count = 1;
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
		else if(w.word[index] == '\0')
		{
			++(**current_word_ptr).data.count;
			goto get_word;
		}
		else
		{
			index++;
			goto compare_chars;
		}
	}

end_processing_word:
	if(!feof(file))
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
	if ((*another_word).data.count > (*best_word).data.count)
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
		printf("%s - %i\n", (*current_word).data.word, (*current_word).data.count);
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