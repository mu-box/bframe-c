// -*- mode: c; tab-width: 4; indent-tabs-mode: 1; st-rulers: [70] -*-
// vim: ts=8 sw=8 ft=c no

#ifndef BFRAME_H
#define BFRAME_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef union bframe_len_s {
	uint32_t	int_len;
	char		char_len[4];
} bframe_len_t;

typedef struct bframe_s {
	bframe_len_t	len;
	char			data[];
} bframe_t;

typedef struct bframe_buffer_s {
	char	*data;
	int		len;
} bframe_buffer_t;

bframe_buffer_t *new_bframe_buffer();

bframe_t *new_bframe(char *data, int len);
bframe_t *parse_bframes(char *data, int data_len, bframe_buffer_t *bframe_buffer, int *number_of_frames);
char *bframe_to_char(bframe_t *bframe, int *len);

void clean_bframe_buffer(bframe_buffer_t *bframe_buffer);
void clean_bframe(bframe_t *bframe);

#endif // BFRAME_H
