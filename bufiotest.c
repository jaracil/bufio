/*
 * bufiotest.c
 *
 *  Created on: 26 de oct. de 2015
 *      Author: pepe
 */



#include "bufio.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
	bufio_t buf, buf2;
	int ch;
	int pipefd[2];
	uint8_t tmpbuf[2048];

	bufio_init(&buf, 2048);
	bufio_init(&buf2, 2048);
	assert(bufio_is_empty(&buf));
	assert(!bufio_is_full(&buf));
	assert(bufio_cap(&buf) == 2048);
	assert(bufio_avail(&buf) == 2048);
	assert(bufio_maxblk(&buf) == 2048);
	assert(bufio_used(&buf) == 0);

	bufio_push_byte(&buf, 65);
	assert(!bufio_is_empty(&buf));
	assert(!bufio_is_full(&buf));
	assert(bufio_avail(&buf) == 2047);
	assert(bufio_maxblk(&buf) == 2047);
	assert(bufio_used(&buf) == 1);

	ch = bufio_pull_byte(&buf);
	assert(ch == 65);
	assert(bufio_is_empty(&buf));
	assert(bufio_avail(&buf) == 2048);
	assert(bufio_maxblk(&buf) == 2048);
	assert(bufio_used(&buf) == 0);

	bufio_printf(&buf,"%s","HOLA");
	assert(bufio_avail(&buf) == 2044);
	assert(bufio_maxblk(&buf) == 2044);
	assert(bufio_used(&buf) == 4);
	assert(strcmp((char *)bufio_tail(&buf), "HOLA") == 0);
	assert(bufio_push_buffer(&buf2, &buf) == 4);
	assert(bufio_used(&buf2) == 4);
	assert(bufio_is_empty(&buf));
	assert(bufio_push_buffer(&buf, &buf2) == 4);

	ch = bufio_pull_byte(&buf);
	assert(ch == 'H');
	assert(!bufio_is_empty(&buf));
	assert(bufio_avail(&buf) == 2045);
	assert(bufio_maxblk(&buf) == 2044);
	assert(bufio_used(&buf) == 3);

	bufio_shift(&buf);
	assert(bufio_maxblk(&buf) == 2045);

	bufio_discard(&buf, bufio_used(&buf));
	assert(bufio_avail(&buf) == 2048);
	assert(bufio_used(&buf) == 0);

	assert(pipe(pipefd) == 0);
	bufio_push_bytes(&buf,"Hola mundo C", 12);
	assert(bufio_used(&buf) == 12);
	bufio_write(&buf, pipefd[1]);
	assert(bufio_is_empty(&buf));
	bufio_read(&buf, pipefd[0]);
	assert(bufio_used(&buf) == 12);
	assert(strcmp((char *)bufio_tail(&buf), "Hola mundo C") == 0);
	bufio_discard(&buf, -1);
	assert(strcmp((char *)bufio_tail(&buf), "Hola mundo ") == 0);
	assert(bufio_pull_bytes(&buf, tmpbuf, sizeof(tmpbuf)) == 11);
	tmpbuf[11] = 0;
	assert(strcmp((char *)tmpbuf, "Hola mundo ") == 0);
	close(pipefd[0]);
	close(pipefd[1]);
	assert(bufio_is_empty(&buf));

	puts("test passed");
	return 0;
}
