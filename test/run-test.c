// -*- mode: c; tab-width: 4; indent-tabs-mode: 1; st-rulers: [70] -*-
// vim: ts=8 sw=8 ft=c noet

#include <stdio.h>
#include "bframe.h"

void test_new_bframe_buffer()
{
	printf("\tTesting new_bframe_buffer()\n");
	bframe_buffer_t *bframe_buffer = new_bframe_buffer();
	clean_bframe(bframe_buffer);
	bframe_buffer = NULL;
}

void test_new_bframe()
{
	printf("\tTesting new_bframe()\n");
	char data[] = "this is a string!";
	char *bframe_char = NULL;
	bframe_t *bframe = new_bframe(data, strlen(data));
	clean_bframe(bframe);
	bframe = NULL;
}

void test_parse_char_to_bframes()
{
	printf("\tTesting parse_char_to_bframes()\n");
	int i;
	int size_a;
	int size_b;
	int size_c;
	int size_d;
	char data_a[] = "Here is a little message:";
	char data_b[] = "There once was a man who liked to eat cheese.";
	char data_c[] = "He also liked to ride on the train.";
	char data_d[] = "Sometimes he did both at the same time.";
	bframe_t *bframe_a = new_bframe(data_a, strlen(data_a));
	bframe_t *bframe_b = new_bframe(data_b, strlen(data_b));
	bframe_t *bframe_c = new_bframe(data_c, strlen(data_c));
	bframe_t *bframe_d = new_bframe(data_d, strlen(data_d));
	char *bframe_char_a = bframe_to_char(bframe_a, &size_a);
	char *bframe_char_b = bframe_to_char(bframe_b, &size_b);
	char *bframe_char_c = bframe_to_char(bframe_c, &size_c);
	char *bframe_char_d = bframe_to_char(bframe_d, &size_d);
	int total_size = size_a + size_b + size_c + size_d;
	char *whole_message = (char *)malloc(total_size + 1);
	whole_message[total_size] = '\0';
	memcpy(whole_message, bframe_char_a, size_a);
	memcpy(whole_message + size_a, bframe_char_b, size_b);
	memcpy(whole_message + size_a + size_b, bframe_char_c, size_c);
	memcpy(whole_message + size_a + size_b + size_c, bframe_char_d, size_d);

	int number_of_frames;
	bframe_buffer_t *bframe_buffer = new_bframe_buffer();

	bframe_t **parsed_bframes = parse_char_to_bframes(whole_message, total_size, bframe_buffer, &number_of_frames);
	printf("\t\tnumber of frames: %d\n", number_of_frames);
	printf("\t\tbframe_buffer size: %d\n", bframe_buffer->len);
	for (i = 0; i < number_of_frames; i++) {
		printf("\t\tsize(%d) data(%s)\n", parsed_bframes[i]->len.int_len, parsed_bframes[i]->data);
	}
	for (i = 0; i < number_of_frames; i++) {
		clean_bframe(parsed_bframes[i]);
	}
	free(parsed_bframes);
	parsed_bframes = NULL;
	clean_bframe_buffer(bframe_buffer);
	free(bframe_buffer);
	printf("\tTesting parse_char_to_bframes() with smaller chunks of data\n");
	int half_size = 33;
	bframe_buffer = new_bframe_buffer();

	parsed_bframes = parse_char_to_bframes(whole_message, half_size, bframe_buffer, &number_of_frames);
	printf("\t\tnumber of frames: %d\n", number_of_frames);
	printf("\t\tbframe_buffer size: %d\n", bframe_buffer->len);
	for (i = 0; i < number_of_frames; i++) {
		printf("\t\tsize(%d) data(%s)\n", parsed_bframes[i]->len.int_len, parsed_bframes[i]->data);
	}
	for (i = 0; i < number_of_frames; i++) {
		clean_bframe(parsed_bframes[i]);
	}
	free(parsed_bframes);
	parsed_bframes = NULL;
	printf("\tTesting parse_char_to_bframes() with smaller chunks of data - second half\n");
	parsed_bframes = parse_char_to_bframes(&whole_message[half_size], total_size - half_size, bframe_buffer, &number_of_frames);
	printf("\t\tnumber of frames: %d\n", number_of_frames);
	printf("\t\tbframe_buffer size: %d\n", bframe_buffer->len);
	for (i = 0; i < number_of_frames; i++) {
		printf("\t\tsize(%d) data(%s)\n", parsed_bframes[i]->len.int_len, parsed_bframes[i]->data);
	}
	for (i = 0; i < number_of_frames; i++) {
		clean_bframe(parsed_bframes[i]);
	}
	free(parsed_bframes);
	parsed_bframes = NULL;
	clean_bframe_buffer(bframe_buffer);
	free(bframe_buffer);

}

void test_bframe_to_char()
{
	printf("\tTesting bframe_to_char()\n");
	char data[] = "this is a string!";
	char *bframe_char = NULL;
	int size;
	int i;
	bframe_t *bframe = new_bframe(data, strlen(data));
	bframe_char = bframe_to_char(bframe, &size);
	printf("\t\tsize(0x");
	for (i = 0; i < 4; i++) {
		printf("%02x", (uint8_t)bframe_char[i]);
	}
	printf(") data(%s)", bframe_char + 4);
	printf("\n");
}

void test_clean_bframe_buffer()
{
	printf("\tTesting clean_bframe_buffer()\n");
	bframe_buffer_t *bframe_buffer = new_bframe_buffer();
	clean_bframe_buffer(bframe_buffer);
	free(bframe_buffer);
}

void test_clean_bframe()
{
	printf("\tTesting clean_bframe()\n");

	char data[] = "this is a string!";
	char *bframe_char = NULL;
	bframe_t *bframe = new_bframe(data, strlen(data));
	clean_bframe(bframe);
	bframe = NULL;
}

int
main(int argc, char **argv)
{
	printf("Testing libbframe\n");

	test_new_bframe_buffer();
	test_new_bframe();
	test_parse_char_to_bframes();
	test_bframe_to_char();
	test_clean_bframe_buffer();
	test_clean_bframe();

	return 0;
}

