// -*- mode: c; tab-width: 4; indent-tabs-mode: 1; st-rulers: [70] -*-
// vim: ts=8 sw=8 ft=c noet

#include "bframe.h"

static char *buffer = NULL;
static int buffer_len = 0;

static int
count_bframes(char *data, int data_len)
{
	int count = -1;
	int offset = 0;
	bframe_len_t *bframe_len;
	do {
		count++;
		bframe_len = (bframe_len_t *)(data + offset);
		offset += bframe_len.int_len + 4;
	} while (offset < data_len - 4);

	return count;
}

bframe_t 
*parse_char_to_bframes(char *data, int data_len, int *number_of_frames)
{
	char *local_buffer = NULL;
	int local_buffer_len = 0;
	bframe_len_t bframe_len;
	bframe_t *bframes = NULL;

	if (buffer == NULL) {
		local_buffer_len = data_len;
		local_buffer = (char *)malloc(local_buffer_len + 1);
		local_buffer[local_buffer_len] = '\0';
		memcpy(local_buffer, data, data_len);

	} else {
		local_buffer_len = data_len + buffer_len;
		local_buffer = (char *)malloc(local_buffer_len + 1);
		local_buffer[local_buffer_len] = '\0';
		memcpy(local_buffer, buffer, buffer_len);
		memcpy(local_buffer + buffer_len, data, data_len);

		free(buffer);
		buffer_len = 0;
	}
	*number_of_frames = count_bframes(local_buffer, local_buffer_len);
	if (*number_of_frames) {
		bframes = malloc((sizeof *bframes) * (*number_of_frames));
	} else
	{

	}
	return bframes;
}

bframe_t 
*pack_bframe(char *data, int length)
{
	bframe_t *bframe = (bframe_t *)malloc(sizeof *bframe);
	bframe.len.int_len = (uint32_t)length;
	bframe->data = (char *)malloc(length + 1);
	bframe->data[length] = '\0';
	memcpy(bframe->data, data, length);
	return bframe;
}

char 
*bframe_to_char(bframe_t *bframe, int *length)
{
	char *packed_frame;

	*length = bframe.len.int_len + 4;
	packed_frame = (char *)malloc((sizeof char ) * (*length) + 1);
	packed_frame[*length] = '\0'
	memcpy(packed_frame, &(bframe.len.char_len), 4);
	memcpy(packed_frame + 4, bframe->data, bframe.len.int_len);
	return packed_frame;
}

void clean_bframe(bframe_t *bframe)
{
	free(bframe->data);
	bframe->data = NULL;
}
