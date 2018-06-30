# pasta
Proficient allocating stack-based tool assisting you

## Usage
Just pop in the header, `#include "pasta.h"`, and that's it.

## Examples
Bad:
```C
#include <stdlib.h>

int main(void)
{
	FILE *f = fopen("file.txt", "r");
	if (!f) {
		return -1;
	}

	void *obj = malloc(69);
	if (!obj) {
		fclose(f);
		return -2;
	}

	void *obj2 = malloc(420);
	if (!obj2) {
		fclose(f);
		free(obj);
		return -3;
	}

	// Do stuff

	fclose(f);
	free(obj);
	free(obj2);

	return 0;
}
```

Better:
```C
#include <stdlib.h>

int main(void)
{
	int result = 0;
	FILE *f;
	void *obj, *obj2;

	f = fopen("file.txt", "r");
	if (!f) {
		result = -1;
		goto done;
	}

	obj = malloc(69);
	if (!obj) {
		result = -2;
		goto done;
	}

	obj2 = malloc(420);
	if (!obj2) {
		result = -3;
		goto done;
	}

	// Do stuff

done:
	fclose(f);
	free(obj);
	free(obj2);

	return result;
}
```

With pasta:
```C
#include "pasta.h"

#include <stdlib.h>

int main(void)
{
	int result = 0;
	pasta_stack stack = PASTA_EMPTY_STACK;

	FILE *f = fopen("file.txt", "r");
	pasta_push(&stack, f, free);
	if (!f) {
		result = -1;
		goto done;
	}

	void *obj = pasta_malloc(69);
	if (!obj) {
		result = -2;
		goto done;
	}

	void *obj2 = pasta_malloc(420);
	if (!obj2) {
		result = -3;
		goto done;
	}

	// Do stuff

done:
	pasta_destroy(&stack);
	return result;
}
```
