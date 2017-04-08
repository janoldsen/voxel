
CFLAGS = -std=c11 -I$(VK_SDK_PATH)/include -D DEBUG
LDFLAGS = -L$(VK_SDK_PATH)/bin -lvulkan-1
CC = gcc

objects = main.o application.o vulkan.o


voxel: $(objects)
	gcc -o voxel.exe $(objects) $(CFLAGS) $(LDFLAGS)

main.o : application.h
application.o : application.h application_w32.c
	gcc -c application_w32.c -o application.o $(CFLAGS)
vulkan.o : vulkan.h

.PHONY: test clean

test: voxel
	./voxel.exe


clean:
	rm -f voxel.exe $(objects)

