/* definitions to keep the things sane, don't modify */
#define PORT_(port)	PORT ## port
#define DDR_(port)	DDR  ## port
#define PIN_(port)	PIN  ## port

#define PORT(port)	PORT_(port)
#define DDR(port)	DDR_(port)
#define PIN(port)	PIN_(port)

#define FW_VERSION "0.1"
#define HW_VERSION "0.1"

// serial baudrate
#define BAUD	9600

/*
 * IO definitions
 */
// uncomment to use serial port 1, 0 used by default
// #define UART_USE_1
