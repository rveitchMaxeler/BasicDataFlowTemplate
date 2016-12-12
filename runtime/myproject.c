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

extern max_file_t *PassThrough_init();
#define MAX_FRAME_SIZE_BYTES 2000

#define MIN(x, y) ((x) < (y) ? (x) : (y))

int main(int argc, char *argv[])
{

	size_t ticks = 512;
	size_t sizeBytes = 4*ticks;

	uint32_t* input = (uint32_t*)malloc(sizeBytes);
	uint32_t* output = (uint32_t*)malloc(sizeBytes);

	for (int iii=0 ; iii<ticks ; iii++){
		input[iii] = iii;
	}

	max_file_t *maxfile = PassThrough_init();
	max_engine_t * engine = max_load(maxfile, "*");

	max_config_set_bool(MAX_CONFIG_PRINTF_TO_STDOUT, true);

	max_actions_t *action = max_actions_init(maxfile, "default");

	printf("Queuing input\n");
	max_queue_input(action, "input", input, sizeBytes);

	printf("Queuing output\n");
	max_queue_output(action, "output", output, sizeBytes);

	printf("Setting ticks\n");
	max_set_ticks(action, "PassThroughKernel", ticks);

	printf("Running\n");
	max_run(engine, action);


	printf("Done\n");
	max_actions_free(action);


	for (int iii=0 ; iii<ticks ; iii++){
		printf("%d: %d == %d\n", iii, input[iii], output[iii]);
	}

	max_unload(engine);
	max_file_free(maxfile);

	return 0;
}
