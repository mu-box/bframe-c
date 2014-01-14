// -*- mode: c; tab-width: 4; indent-tabs-mode: 1; st-rulers: [70] -*-
// vim: ts=8 sw=8 ft=c no

#ifndef BFRAME_H
#define BFRAME_H

typedef union bframe_len_s {
	uint32_t	int_len;
	char		char_len[4];
} bframe_len_t;

typedef struct bframe_s {
	bframe_len_t	len;
	char			*data;
} bframe_t;

bframe_t *parse_bframes(char *data, int data_len, int *number_of_frames);
bframe_t *pack_bframe(char *data, int len);
char *bframe_to_char(bframe_t *bframe, int *len);

void clean_bframe(bframe_t *bframe);

#endif // BFRAME_H