// -*- mode: c; tab-width: 4; indent-tabs-mode: 1; st-rulers: [70] -*-
// vim: ts=8 sw=8 ft=c noet

#include "bframe.h"

bframe_buffer_t 
*new_bframe_buffer()
{
	bframe_buffer_t *bframe_buffer;
	bframe_buffer = malloc(sizeof(bframe_buffer_t));
	bframe_buffer->data = NULL;
	bframe_buffer->len = 0;
	return bframe_buffer;
}

bframe_t 
*new_bframe(char *data, int length)
{
	bframe_t *bframe = (bframe_t *)malloc(sizeof(bframe_t));
	bframe->len.int_len = (uint32_t)length;
	bframe->data = (char *)malloc(length + 1);
	bframe->data[length] = '\0';
	memcpy(bframe->data, data, length);
	return bframe;
}

static int
count_bframes(char *data, int data_len)
{
	int count = 0;
	int offset = 0;
	bframe_len_t *bframe_len;
	do {
		count++;
		bframe_len = (bframe_len_t *)(data + offset);
		offset += bframe_len->int_len + 4;
	} while (offset < data_len - 4);

	if (offset > data_len)
		count--;

	return count;
}

bframe_t 
**parse_char_to_bframes(char *data, int data_len, 
	bframe_buffer_t *bframe_buffer, int *number_of_frames)
{
	char *local_buffer = NULL;
	int local_buffer_len = 0;
	bframe_len_t bframe_len;
	bframe_t **bframes = NULL;
	int count = 0;
	int offset = 0;

	if (bframe_buffer->data == NULL) {
		local_buffer_len = data_len;
		local_buffer = (char *)malloc(local_buffer_len + 1);
		local_buffer[local_buffer_len] = '\0';
		memcpy(local_buffer, data, data_len);

	} else {
		local_buffer_len = data_len + bframe_buffer->len;
		local_buffer = (char *)malloc(local_buffer_len + 1);
		local_buffer[local_buffer_len] = '\0';
		memcpy(local_buffer, bframe_buffer->data, bframe_buffer->len);
		memcpy(local_buffer + bframe_buffer->len, data, data_len);

		free(bframe_buffer->data);
		bframe_buffer->data = NULL;
		bframe_buffer->len = 0;
	}
	*number_of_frames = count_bframes(local_buffer, local_buffer_len);
	bframes = (bframe_t **)malloc(sizeof(bframe_t) * (*number_of_frames));
	while (count < *number_of_frames) {
		bframes[count]= new_bframe(local_buffer + offset + 4, *((int *) (local_buffer + offset)));
		// memcpy(bframes[count].len.char_len, local_buffer + offset, 4);
		// bframes[count].data = (char *)malloc(bframes[count].len.int_len + 1);
		// bframes[count].data[bframes[count].len.int_len] = '\0';
		// memcpy(bframes[count].data, local_buffer + offset + 4, bframes[count].len.int_len);
		offset += bframes[count]->len.int_len + 4;
		count++;
	}
	if (offset < local_buffer_len) {
		bframe_buffer->len = local_buffer_len - offset;
		bframe_buffer->data = (char *)malloc(bframe_buffer->len + 1);
		bframe_buffer->data[bframe_buffer->len] = '\0';
		memcpy(bframe_buffer->data, local_buffer + offset, bframe_buffer->len);
	}
	free(local_buffer);
	local_buffer = NULL;
	return bframes;
}

char 
*bframe_to_char(bframe_t *bframe, int *length)
{
	char *packed_frame;

	*length = bframe->len.int_len + 4;
	packed_frame = (char *)malloc(sizeof(char) * (*length) + 1);
	packed_frame[*length] = '\0';
	memcpy(packed_frame, &(bframe->len.char_len), 4);
	memcpy(packed_frame + 4, bframe->data, bframe->len.int_len);
	return packed_frame;
}

void clean_bframe(bframe_t *bframe)
{
	free(bframe->data);
	bframe->data = NULL;
}

void clean_bframe_buffer(bframe_buffer_t *bframe_buffer)
{
	free(bframe_buffer->data);
	bframe_buffer->data = NULL;
	bframe_buffer->len = 0;
}
