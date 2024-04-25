#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

typedef char ALIGN[16];

union header
{
	struct
	{
		size_t size;
		unsigned isFree;
		struct header_t *next;
	} s;

	ALIGN stub;
};

typedef union header header_t;
header_t *head, *tail;
pthread_mutex_t global_malloc_lock;

void *mmalloc(size_t size);
static header_t *get_free_block(size_t size);

int main(void)
{
	int *arr = (int *)mmalloc(sizeof(int) * 10);
	for (int i = 0; i < 10; ++i)
	{
		arr[i] = i;
	}
	printf("%d\n", arr);
	printf("%d\n", arr[5]);
	printf("%d\n", &arr[5]);
	return 0;
}

void *mmalloc(size_t size)
{
	size_t total_size;
	header_t *header;
	void *block;
	if (!size)
	{
		return NULL;
	}
	pthread_mutex_lock(&global_malloc_lock);
	header = get_free_block(size);
	if (header)
	{
		header->s.isFree = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		return (void *)header + 1;
	}
	total_size = sizeof(header_t) + size;
	block = sbrk(total_size);
	if (block == (void *)-1)
	{
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}
	header = block;
	header->s.size = size;
	header->s.isFree = 0;
	header->s.next = NULL;
	if (!head)
	{
		head = header;
	}
	if (tail)
	{
		tail->s.next = header;
	}
	tail = header;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void *)(header + 1);
}

static header_t *get_free_block(size_t size)
{
	header_t *current = head;
	while (current)
	{
		if (current->s.isFree && current->s.size >= size)
		{
			return current;
		}
		current = current->s.next;
	}
	return NULL;
}