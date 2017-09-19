#include <hidapi/hidapi.h>
#include <stdio.h>

int main() {
    int result;
    result = hid_init();
    if (result != 0) {
        printf("Error, could not open HIDAPI library!\n");
        return 0;
    }

    hid_device *handle;
    handle = hid_open(0x4242, 0x4242, NULL);
    printf("handle: %d\n", handle);
    if (handle == 0) {
      printf("Error, could not find deivce\n");
      return 0;
    }

    // Read requested state
    char buf[100];
    int i;
    int numRead = 0;
    while (1) {
      numRead = hid_read(handle, buf, sizeof(buf));
    
      // Print out the returned buffer.
      for (i = 0; i < numRead; i++) {
          printf("buf[%d]: %d\n", i, buf[i]);
      }
    }

    // Finalize the hidapi library
	  result = hid_exit();

    return(0);
}