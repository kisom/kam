#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	int		i;
	unsigned char	jmp[2];
	uint16_t	effective;
	int16_t		actual;

	if (1 == argc)
		return EXIT_SUCCESS;

	for (i = 1; i < argc; i++) {
		actual = (int16_t)atoi(argv[i]);
		effective = (uint16_t)actual;
		memcpy(jmp, &effective, 2);
		printf("Actual: %d\tEffective: %d (%02x %02x)\n",
		    actual, effective, jmp[0], jmp[1]);
	}
	return EXIT_SUCCESS;
}
