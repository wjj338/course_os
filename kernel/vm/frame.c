#include "vm.h"
//#include "frame.h"
#include "klibc.h"

struct vm_free_frame {
	void *next; // Next free frame
};

struct vm_free_frame *vm_free_list;

int vm_build_free_frame_list(void *start, void *end) {
	vm_free_list = start;
	struct vm_free_frame *p = start;
	for (p=start; (void*)p<end; p=(void*)p+BLOCK_SIZE) {
		p->next = (void*)p+BLOCK_SIZE;
		if ((unsigned int)((void*)p+BLOCK_SIZE) >= (unsigned int)end) {
			p->next = 0x0;
		}
	}
	return 0;
}

void *vm_get_free_frame() {
	// Check if there are no frames
	if (vm_free_list == 0x0) {
		return 0x0;
	}
	void *p = vm_free_list;
	vm_free_list = vm_free_list->next;
	return p;
}

void vm_release_frame(void *p) {
	// TODO: Check if p is actually a valid frame
	struct vm_free_frame *flist = p;
	flist->next = vm_free_list;
	vm_free_list = p;
}

int vm_count_free_frames() {
	int cnt = 0;
	struct vm_free_frame *p = vm_free_list;
	while ((p = p->next)) {
		cnt++;
	}
	return cnt;
}
