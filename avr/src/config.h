/* definitions to keep the things sane, don't modify */
#define PORT_(port)	PORT ## port
#define DDR_(port)	DDR  ## port
#define PIN_(port)	PIN  ## port

#define PORT(port)	PORT_(port)
#define DDR(port)	DDR_(port)
#define PIN(port)	PIN_(port)

#define FW_VERSION "0.1"
#define HW_VERSION "0.1"

/*
 *  Default values
 */
// serial baudrate
#define BAUD	9600

//led current to set when using additional pwm dimming, 0-255, should be close
//within reasonable led driver efficiency region
#define DEFAULT_DIM_CURRENT	30


/*
 * IO definitions
 */
// uncomment to use serial port 1, 0 used by default
// #define UART_USE_1

/*
 * TOCCx outputs for pwm + corresponding port/pin
 *
 * Due to internal limitations, one LEDx_PWM_NUM must be even, second must be
 * odd
 */
#define LED1_PWM_NUM	2
#define LED2_PWM_NUM	3

#define LED1_PWM_PORT	A
#define LED1_PWM_PIN	3
#define LED2_PWM_PORT	A
#define LED2_PWM_PIN	4

#define LED1_DIM_PORT	A
#define LED1_DIM_PIN	5
#define LED2_DIM_PORT	A
#define LED2_DIM_PIN	6

/* LED driver power transistor */
#define DRIVER_ENABLE_PORT	A
#define DRIVER_ENABLE_PIN	6

/* Small low-power leds */
#define LED_SM1_PORT	A
#define LED_SM1_PIN	0
#define LED_SM2_PORT	B
#define LED_SM2_PIN	2

/* Light measurement */
#define PHOTOTRANS_ADC	9
