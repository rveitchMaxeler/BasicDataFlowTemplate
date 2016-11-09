#define _GNU_SOURCE

#include <errno.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "MaxSLiCInterface.h"
#include "MaxSLiCNetInterface.h"

extern max_file_t *MyProject_init();
#define MAX_FRAME_SIZE_BYTES 2000

#define MIN(x, y) ((x) < (y) ? (x) : (y))

static max_framed_stream_t *init_framed_stream(max_engine_t *engine, char *name)
{
	void *buffer = NULL;
	size_t buffer_size = 4096 * 512;

	/*
	 * Allocate a buffer where the frames will be DMA'd to/from.
	 */
	if (posix_memalign(&buffer, 4096, buffer_size)) {
		perror("init_framed_stream: posix_memalign");
	}

	/*
	 * MAX_FRAME_SIZE_BYTES is only used for stream from CPU. It is ignored on streams to the CPU.
	 */
	return max_framed_stream_setup(engine, name, buffer, buffer_size, MAX_FRAME_SIZE_BYTES);
}

int main(int argc, char *argv[])
{

	max_file_t *maxfile = MyProject_init();
	max_engine_t * engine = max_load(maxfile, "*");


	max_config_set_bool(MAX_CONFIG_PRINTF_TO_STDOUT, true);

	max_actions_t *action = max_actions_init_explicit(maxfile);
	max_run(engine, action);
	max_actions_free(action);


	/*
	 * Initialize the framed streams 'src' and 'dst'
	 */
	max_framed_stream_t *src = init_framed_stream(engine, "src");
	max_framed_stream_t *dst = init_framed_stream(engine, "dst");

	/*
	 * Loopback
	 */
	
	while (true) {
		void *incoming_frame = NULL;
		size_t incoming_frame_size = 0;

		if (max_framed_stream_read(dst, 1, &incoming_frame, &incoming_frame_size)) {
			printf("Got a frame %zd bytes\n", incoming_frame_size);

			/*
			 * Send the same frame we just got right back
			 *
			 * First, acquire an outgoing slot
			 */

			void *outgoing_frame = NULL;
			while (max_framed_stream_write_acquire(src, 1, &outgoing_frame) != 1) {
				/*
				 * No slots are available, lets try again.
				 * Sleep to avoid a busy wait... (won't be low latency)
				 */
				usleep(1);
			}

			/*
			 * We got a slot, src_frame is pointing to our buffer.
			 * We can send any buffer up to MAX_FRAME_SIZE_BYTES
			 */
			size_t outgoing_frame_size = MIN(MAX_FRAME_SIZE_BYTES, incoming_frame_size);
			memcpy(outgoing_frame, incoming_frame, outgoing_frame_size);

			printf("Sending frame back out (%zd bytes)...\n", outgoing_frame_size);
			max_framed_stream_write(src, 1, &outgoing_frame_size);
		}
	}


	max_unload(engine);
	max_file_free(maxfile);

	return 0;
}
