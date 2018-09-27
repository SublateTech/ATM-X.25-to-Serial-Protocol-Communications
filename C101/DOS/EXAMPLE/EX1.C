#include	"../driver/c101.h"

main()
{
	int	ret;
	char	str[] = "This is a test frame";
	char	buf[1024];

	printf("C101 Example program\n");
	ret = sio_init();
	printf("Init C101 by call sio_init return: %d\n", ret);
	if (ret < 0) {
		printf("Can't be continue, Pls check error code.\n");
		exit();
	}
	ret = sio_write(str, strlen(str));
	printf("Test write out len=%d ret=%d\n", strlen(str), ret);
	printf("Received data frame = %d\n", sio_iframe());
	if (sio_iframe()) {
		ret = sio_read(buf, 1024);
		if (ret)
			printf("Read one frame len=%d\n", ret);
	}
	printf("Transmit buffer data frame = %d\n", sio_oframe());
	sio_release();
	printf("End example program\n");
}
