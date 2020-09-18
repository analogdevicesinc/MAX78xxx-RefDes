#ifndef MAX20303_DRIVER
#define MAX20303_DRIVER

#define MAX20303_ADDR 0x50
#define HARDWARE_ID 0x02

#define LDO_OUTPUT_ON			1
#define LDO_OUTPUT_OFF			0

#define BUCK_OUTPUT_ON			1
#define BUCK_OUTPUT_OFF			0

#define BOOST_OUTPUT_ON			1
#define BOOST_OUTPUT_OFF		0

#define LED_OUTPUT_ON			1
#define LED_OUTPUT_OFF			0

// Registers
#define REG_HARDWARE_ID         0x00     // HardwareID Register
#define REG_FIRMWARE_REV        0x01     // FirmwareID Register
#define REG_INT0                0x03     // Int0 Register
#define REG_INT1                0x04     // Int1 Register
#define REG_INT2                0x05     // Int2 Register
#define REG_STATUS0             0x06     // Status Register 0
#define REG_STATUS1             0x07     // Status Register 1
#define REG_STATUS2             0x08     // Status Register 2
#define REG_STATUS3             0x09     // Status Register 2
#define REG_SYSTEM_ERROR        0x0B     // SystemError Register
#define REG_INT_MASK0           0x0C     // IntMask0 Register
#define REG_INT_MASK1           0x0D     // IntMask1 Register
#define REG_INT_MASK2           0x0E     // IntMask1 Register
#define REG_AP_DATOUT0          0x0F     // APDataOut0 Register
#define REG_AP_DATOUT1          0x10     // APDataOut1 Register
#define REG_AP_DATOUT2          0x11     // APDataOut2 Register
#define REG_AP_DATOUT3          0x12     // APDataOut3 Register
#define REG_AP_DATOUT4          0x13     // APDataOut4 Register
#define REG_AP_DATOUT5          0x14     // APDataOut5 Register
#define REG_AP_DATOUT6          0x15     // APDataOut6 Register
#define REG_AP_CMDOUT           0x17     // APCmdOut Register
#define REG_AP_RESPONSE         0x18     // APResponse Register
#define REG_AP_DATAIN0          0x19
#define REG_AP_DATAIN1          0x1A
#define REG_AP_DATAIN2          0x1B
#define REG_AP_DATAIN3          0x1C
#define REG_AP_DATAIN4          0x1D
#define REG_AP_DATAIN5          0x1E
#define REG_LDO_DIRECT          0x20
#define REG_MPC_DIRECTWRITE     0x21
#define REG_MPC_DIRECTRED       0x22
#define REG_LED_STEP_DIRECT     0x2C
#define REG_LED0_DIRECT         0x2D
#define REG_LED1_DIRECT         0x2E
#define REG_LED2_DIRECT         0x2F
#define REG_LDO1_CONFIG_WRITE   0x40
#define REG_LDO1_CONFIG_READ    0x41
#define REG_LDO2_CONFIG_WRITE   0x42
#define REG_LDO2_CONFIG_READ    0x43

#define	APREG_GPIO_CONFIG_WRITE					0x01
#define	APREG_GPIO_CONFIG_READ					0x02
#define	APREG_GPIO_CONTROL_WRITE				0x03
#define	APREG_GPIO_CONTROL_READ					0x04

#define	APREG_MPC_CONFIG_WRITE					0x06
#define	APREG_MPC_CONFIG_READ					0x07
#define	APREG_INPUTCURRENT_CONFIG_WRITE			0x10
#define	APREG_INPUTCURRENT_CONFIG_READ			0x11
#define	APREG_THERMALSHUTDOWN_CONFIG_READ		0x12
#define	APREG_CHARHER_CONFIG_WRITE				0x14
#define	APREG_CHARHER_CONFIG_READ				0x15
#define APREG_CHARGERTHERMALLIMITS_CONFIG_WRITE	0x16
#define APREG_CHARGERTHERMALLIMITS_CONFIG_READ	0x17

#define	APREG_BST_CONFIG_WRITE					0x30
#define	APREG_BST_CONFIG_READ					0x31
#define APREG_BUCK1_CONFIG_WRITE				0x35
#define	APREG_BUCK1_CONFIG_READ					0x36
#define APREG_BUCK1_DVSCONFIG_WRITE				0x37
#define APREG_BUCK2_CONFIG_WRITE				0x3A
#define APREG_BUCK2_CONFIG_READ					0x3B
#define APREG_BUCK2_DVSCONFIG_WRITE				0x3C
#define APREG_LDO1_CONFIG_WRITE					0x40
#define APREG_LDO1_CONFIG_READ					0x41
#define APREG_LDO2_CONFIG_WRITE					0x42
#define APREG_LDO2_CONFIG_READ					0x43
#define APREG_CHARGEPUMP_CONFIG_WRITE			0x46
#define APREG_CHARGEPUMP_CONFIG_READ			0x47
#define APREG_SFOUT_CONFIG_WRITE				0x48
#define APREG_SFOUT_CONFIG_READ					0x49

#define APREG_BBST_CONFIG_WRITE					0x70
#define APREG_BBST_CONFIG_READ					0x71

#define APREG_POWEROFF							0x80
#define APREG_SOFTRESET							0x81
#define APREG_HARDRESET							0x82
#define APREG_STAYON							0x83
#define APREG_POWEROFF_COMMAND_DELAY			0x84


void MAX20303_initialize( uint8_t initialize_i2c );
void MAX20303_setldo1( uint8_t ldo1_onoff );
void MAX20303_worker();
void MAX20303_setbuck1( uint8_t buck1_onoff);
void MAX20303_setbuck2( uint8_t buck2_onoff);
void MAX20303_setboost( uint8_t boost_onoff, uint8_t boost_output_level );

void MAX20303_led_red( uint8_t ledStatus );
void MAX20303_led_green( uint8_t ledStatus );
void MAX20303_led_blue( uint8_t ledStatus );
#endif /* MAX20303_DRIVER_H */
