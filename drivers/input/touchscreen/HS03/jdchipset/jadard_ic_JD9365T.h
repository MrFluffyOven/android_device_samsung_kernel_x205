#ifndef JADARD_JD9365T_H
#define JADARD_JD9365T_H

static int jd9365t_EnterBackDoor(uint16_t *pRomID);
static int jd9365t_ExitBackDoor(void);

/*HS03 code for SL6215DEV-1773 by chenyihong at 20210924 start*/
#define JD9365T_ID                         (0x9383)
#define JD9365T_OUTER_ID                   "JD9365T"
#define JD9365T_MEMORY_ADDR_PRAM           (0x00000000)
#define JD9365T_MEMORY_ADDR_DRAM           (0x20000000)
#define JD9365T_MEMORY_ADDR_ARAM           (0x20010000)
#define JD9365T_MEMORY_ADDR_ERAM           (0x20011000)
#define JD9365T_MAX_DSRAM_NUM              (16)
#define JD9365T_MAX_ESRAM_NUM              (15)
#define JD9365T_SECTION_INFO_READY_VALUE   (0xA55A)
#define JD9365T_OUTPUT_DATA_HANDSHAKE_DONE (0xA55A)
#define JD9365T_SIZE                       (80 * 1024)
#define JD9365T_PRAM_MAX_SIZE              (0x10000)
/* Set BIN_FW_CID_VER & BIN_FW_VER address */
#define JD9365T_BIN_FW_HEADER_SIZE         (0x100)
/* Please reference .flash_2_fw_cfg_info of FW */
#define JD9365T_BIN_FW_CFG_INFO_ADDR       (JD9365T_BIN_FW_HEADER_SIZE + 0x10E90)
#define JD9365T_BIN_FW_CID_VER             (JD9365T_BIN_FW_CFG_INFO_ADDR + 0)
#define JD9365T_BIN_FW_VER                 (JD9365T_BIN_FW_CFG_INFO_ADDR + 7)
#define JD9365T_DATA_TIMEOUT               (5000)
/*HS03 code for SL6215DEV-1773 by chenyihong at 20210924 end*/

struct JD9365T_MOVE_INFO {
	uint8_t mov_mem_cmd;
	uint32_t fl_st_addr;
	uint16_t fl_len;
	uint32_t to_mem_st_addr;
	uint16_t to_mem_crc;
};

struct JD9365T_INFO_CONTENT {
	uint32_t info_content_addr;
	uint32_t info_content_len;
};

struct JD9365T_DSRAM_HOST_ADDR {
	uint32_t fw_cid_version;
	uint32_t fw_version;
	uint32_t game_mode_en;
	uint32_t usb_en;
	uint32_t gesture_en;
	uint32_t high_sensitivity_en;
	uint32_t border_en;
	/* HS03 code for SL6215DEV-3658 by chenyihong at 20211117 start */
	uint32_t earphone_en;
	/* HS03 code for SL6215DEV-3658 by chenyihong at 20211117 end */
	uint32_t panel_maker;
	uint32_t panel_version;
#ifdef CONFIG_TOUCHSCREEN_JADARD_SORTING
	uint32_t mpap_pw;
	uint32_t mpap_handshake;
	uint32_t mpap_keep_frame;
	uint32_t mpap_skip_frame;
	uint32_t mpap_mux_switch;
	uint32_t oscd_on;
	uint32_t mpap_diff_array;
#endif
};

struct JD9365T_DSRAM_DEBUG_ADDR {
	uint32_t output_data_addr;
	uint32_t output_data_sel;
	uint32_t output_data_handshake;
	uint32_t freq_band;
};

struct JD9365T_ESRAM_INFO_CONTENT_ADDR {
	uint32_t coordinate_report;
};

struct JD9365T_CHIP_INFO {
	bool back_door_mode;
	struct JD9365T_DSRAM_HOST_ADDR dsram_host_addr;
	struct JD9365T_DSRAM_DEBUG_ADDR dsram_debug_addr;
	struct JD9365T_ESRAM_INFO_CONTENT_ADDR esram_info_content_addr;
} g_jd9365t_chip_info;

struct JD9365T_SECTION_INFO {
	bool section_info_ready;
	uint32_t section_info_ready_addr;
	/* Dsram section */
	uint32_t dsram_num_start_addr;
	uint32_t dsram_section_info_start_addr;
	/* Esram section */
	uint32_t esram_num_start_addr;
	uint32_t esram_section_info_start_addr;
} g_jd9365t_section_info;

enum JD9365T_DSRAM_SECTION_INFO_ORDER {
	JD9365T_DSRAM_ADC_MAPPING_TABLE = 0,
	JD9365T_DSRAM_AFE_CONFIG,
	JD9365T_DSRAM_FW_CFG_INFO,
	JD9365T_DSRAM_FW_CFG,
	JD9365T_DSRAM_FW_CFG_SORTING,
	JD9365T_DSRAM_HOST,
	JD9365T_DSRAM_DEBUG,
	JD9365T_DSRAM_FW_PAYLOAD,
	JD9365T_DSRAM_CB_1CYC,
	JD9365T_DSRAM_CB_F0,
	JD9365T_DSRAM_CB_F1,
	JD9365T_DSRAM_CB_F2,
	JD9365T_REPAIR_MODE_SWITCH,
	JD9365T_DSRAM_MPAP_DIFF_ARRAY,
	JD9365T_PSRAM_ALGO_PARA,
	JD9365T_PSRAM_SORT_PARA,
};

enum JD9365T_ESRAM_SECTION_INFO_ORDER {
	JD9365T_ESRAM_ALL_SECTION_INFO = 0,
	JD9365T_ESRAM_OUTPUT_BUF,
	JD9365T_ESRAM_COORDINATE_REPORT,
};

enum JD9365T_HEADER_INFO {
	JD9365T_HW_HEADER_MAX_SIZE         = 256,
	JD9365T_HW_HEADER_CRC_NUMBER       = 2,
	JD9365T_HW_HEADER_INITIAL_NUMBER   = 3,
	JD9365T_HW_HEADER_MOVE_INFO_NUMBER = 12,
};

enum JD9365T_HEADER_POSITION {
	JD9365T_HW_HEADER_CRC_H = 0,
	JD9365T_HW_HEADER_CRC_L,
	JD9365T_CRC_FAIL_RETRY_TIMES,
	JD9365T_HW_HEADER_LEN,
	JD9365T_LOAD_CODE_SPI_FREQ,
};

enum JD9365T_CRC_INFO {
	JD9365T_CRC_INFO_PolynomialCRC16   = 0x8005,
	JD9365T_CRC_INFO_CRC_INITIAL_VALUE = 0xFFFF,
};

enum JD9365T_SIZE_DEF {
	JD9365T_SIZE_DEF_PAGE_SIZE   = 0x100,
	JD9365T_SIZE_DEF_SECTOR_SIZE = 0x1000,
	JD9365T_SIZE_DEF_BLOCK_32K   = 0x8000,
	JD9365T_SIZE_DEF_BLOCK_64K   = 0x10000,
};

enum JD9365T_FLASH_RUN_TIME {
	/* unit: ms */
	JD9365T_FLASH_RUN_TIME_PAGE_WRITE_WAIT_TIME = 3,
	JD9365T_FLASH_RUN_TIME_PAGE_WRITE_TIME      = 50,
	JD9365T_FLASH_RUN_TIME_ERASE_SECTOR_TIME    = 50,
	JD9365T_FLASH_RUN_TIME_ERASE_BLOCK_32K_TIME = 100,
	JD9365T_FLASH_RUN_TIME_ERASE_BLOCK_64K_TIME = 200,
	JD9365T_FLASH_RUN_TIME_ERASE_CHIP_TIME      = 1000,
};

enum JD9365T_FLASH_PACK {
	JD9365T_FLASH_PACK_READ_SIZE  = 2048,
	JD9365T_FLASH_PACK_WRITE_SIZE = 2048,
};

enum JD9365T_TIMER_REG_ADDR {
	JD9365T_TIMER_BASE_ADDR           = 0x400000,
	JD9365T_TIMER_REG_ADDR_RTC_CONFIG = (JD9365T_TIMER_BASE_ADDR << 8) + 0x42,
};

enum JD9365T_TIMER_RELATED_SETTING {
	/* 0x42[0] */
	JD9365T_TIMER_RELATED_SETTING_DISABLE_RTC_RUN = 0x00,
	JD9365T_TIMER_RELATED_SETTING_ENABLE_RTC_RUN = 0x01,
};

enum JD9365T_EX_FLASH_ADDR {
	JD9365T_EX_FLASH_ADDR_READ             = 0x03,
	JD9365T_EX_FLASH_ADDR_STATUS_REGISTER1 = 0x05,
	JD9365T_EX_FLASH_ADDR_WRITE_ENABLE     = 0x06,
	JD9365T_EX_FLASH_ADDR_WRITE_DISABLE    = 0x04,
	JD9365T_EX_FLASH_ADDR_ERASE_SECTOR     = 0x20,
	JD9365T_EX_FLASH_ADDR_STATUS_REGISTER2 = 0x35,
	JD9365T_EX_FLASH_ADDR_ERASE_BLOCK_32K  = 0x52,
	JD9365T_EX_FLASH_ADDR_ERASE_BLOCK_64K  = 0xD8,
	JD9365T_EX_FLASH_ADDR_ERASE_CHIP       = 0xC7,
	JD9365T_EX_FLASH_ADDR_READ_ID          = 0x9F,
};

enum JD9365T_DMA_RELATED_SETTING {
	/* 0x0C[4:0] */
	JD9365T_DMA_RELATED_SETTING_READ_FROM_FLASH           = 0x01,
	JD9365T_DMA_RELATED_SETTING_WRITE_TO_PRAM             = 0x03,
	JD9365T_DMA_RELATED_SETTING_WRITE_TO_FLASH            = 0x07,
	JD9365T_DMA_RELATED_SETTING_READ_FLASH_STATUS         = 0x08,
	JD9365T_DMA_RELATED_SETTING_PAGE_PROGRAM_FLASH        = 0x10,
	/* 0x0C[4] */
	JD9365T_DMA_RELATED_SETTING_PAGE_PROGRAM_DONE         = 0x00,
	/* 0x0E[1:0] */
	JD9365T_DMA_RELATED_SETTING_TRANSFER_DATA_1_BYTE_MODE = 0x00,
	JD9365T_DMA_RELATED_SETTING_TRANSFER_DATA_2_BYTE_MODE = 0x01,
	JD9365T_DMA_RELATED_SETTING_TRANSFER_DATA_4_BYTE_MODE = 0x02,
	/* 0x1D[0] */
	JD9365T_DMA_RELATED_SETTING_DMA_DONE                  = 0x00,
	JD9365T_DMA_RELATED_SETTING_DMA_BUSY                  = 0x01,
};

enum JD9365T_MASTER_SPI_RELATED_SETTING {
	/* 0x11[2:0] */
	JD9365T_MASTER_SPI_RELATED_SETTING_SPEED_24MHz   = 0x00,
	JD9365T_MASTER_SPI_RELATED_SETTING_SPEED_12MHz   = 0x01,
	JD9365T_MASTER_SPI_RELATED_SETTING_SPEED_6MHz    = 0x02,
	JD9365T_MASTER_SPI_RELATED_SETTING_SPEED_3MHz    = 0x03,
	JD9365T_MASTER_SPI_RELATED_SETTING_SPEED_1500KHz = 0x04,
	JD9365T_MASTER_SPI_RELATED_SETTING_SPEED_750KHz  = 0x05,
	JD9365T_MASTER_SPI_RELATED_SETTING_SPEED_375KHz  = 0x06,
	/* 0x12[0] */
	JD9365T_MASTER_SPI_RELATED_SETTING_CSN_H         = 0x00,
	JD9365T_MASTER_SPI_RELATED_SETTING_CSN_L         = 0x01,
	/* 0x15[7] */
	JD9365T_MASTER_SPI_RELATED_SETTING_SPIF_BUSY     = 0x00,
	JD9365T_MASTER_SPI_RELATED_SETTING_SPIF_DONE     = 0x80,
	/* 0x1D[4] */
	JD9365T_MASTER_SPI_RELATED_SETTING_FLASH_DONE    = 0x00,
	JD9365T_MASTER_SPI_RELATED_SETTING_FLASH_BUSY    = 0x10,
};

enum JD9365T_CRC_CODE_POSITION {
	JD9365T_CRC_CODE_POSITION_LOW_BYTE = 0,
	JD9365T_CRC_CODE_POSITION_HIGH_BYTE,
};

enum JD9365T_FLASH_REG_ADDR {
	JD9365T_FLASH_BASE_ADDR                            = 0x400002,
	JD9365T_FLASH_REG_ADDR_DMA_FLASH_ADDR0             = (JD9365T_FLASH_BASE_ADDR << 8) + 0x00,
	JD9365T_FLASH_REG_ADDR_DMA_FLASH_ADDR1             = (JD9365T_FLASH_BASE_ADDR << 8) + 0x01,
	JD9365T_FLASH_REG_ADDR_DMA_FLASH_ADDR2             = (JD9365T_FLASH_BASE_ADDR << 8) + 0x02,
	JD9365T_FLASH_REG_ADDR_DMA_FLASH_ADDR3             = (JD9365T_FLASH_BASE_ADDR << 8) + 0x03,
	JD9365T_FLASH_REG_ADDR_DMA_SIZE0                   = (JD9365T_FLASH_BASE_ADDR << 8) + 0x04,
	JD9365T_FLASH_REG_ADDR_DMA_SIZE1                   = (JD9365T_FLASH_BASE_ADDR << 8) + 0x05,
	JD9365T_FLASH_REG_ADDR_DMA_SIZE2                   = (JD9365T_FLASH_BASE_ADDR << 8) + 0x06,
	JD9365T_FLASH_REG_ADDR_DMA_SIZE3                   = (JD9365T_FLASH_BASE_ADDR << 8) + 0x07,
	JD9365T_FLASH_REG_ADDR_DMA_TARGET_ADDR0            = (JD9365T_FLASH_BASE_ADDR << 8) + 0x08,
	JD9365T_FLASH_REG_ADDR_DMA_TARGET_ADDR1            = (JD9365T_FLASH_BASE_ADDR << 8) + 0x09,
	JD9365T_FLASH_REG_ADDR_DMA_TARGET_ADDR2            = (JD9365T_FLASH_BASE_ADDR << 8) + 0x0A,
	JD9365T_FLASH_REG_ADDR_DMA_TARGET_ADDR3            = (JD9365T_FLASH_BASE_ADDR << 8) + 0x0B,
	JD9365T_FLASH_REG_ADDR_DMA_2WFLASHEN_1WRITE_0START = (JD9365T_FLASH_BASE_ADDR << 8) + 0x0C,
	JD9365T_FLASH_REG_ADDR_DMA_BYTE_MODE               = (JD9365T_FLASH_BASE_ADDR << 8) + 0x0E,
	JD9365T_FLASH_REG_ADDR_SPI_BAUD_RATE               = (JD9365T_FLASH_BASE_ADDR << 8) + 0x11,
	JD9365T_FLASH_REG_ADDR_SPI_CSN_OUT                 = (JD9365T_FLASH_BASE_ADDR << 8) + 0x12,
	JD9365T_FLASH_REG_ADDR_SPI_2CPHA_1CPOL_0SPEN       = (JD9365T_FLASH_BASE_ADDR << 8) + 0x13,
	JD9365T_FLASH_REG_ADDR_SPI_WDATA                   = (JD9365T_FLASH_BASE_ADDR << 8) + 0x14,
	JD9365T_FLASH_REG_ADDR_SPI_STATUS                  = (JD9365T_FLASH_BASE_ADDR << 8) + 0x15,
	JD9365T_FLASH_REG_ADDR_SPI_RDATA                   = (JD9365T_FLASH_BASE_ADDR << 8) + 0x16,
	JD9365T_FLASH_REG_ADDR_CRC_INIT                    = (JD9365T_FLASH_BASE_ADDR << 8) + 0x17,
	JD9365T_FLASH_REG_ADDR_CRC_INIT_CODE1              = (JD9365T_FLASH_BASE_ADDR << 8) + 0x18,
	JD9365T_FLASH_REG_ADDR_CRC_INIT_CODE0              = (JD9365T_FLASH_BASE_ADDR << 8) + 0x19,
	JD9365T_FLASH_REG_ADDR_CRC_CHK1                    = (JD9365T_FLASH_BASE_ADDR << 8) + 0x1A,
	JD9365T_FLASH_REG_ADDR_CRC_CHK0                    = (JD9365T_FLASH_BASE_ADDR << 8) + 0X1B,
	JD9365T_FLASH_REG_ADDR_CRC_ENABLE                  = (JD9365T_FLASH_BASE_ADDR << 8) + 0x1C,
	JD9365T_FLASH_REG_ADDR_DMA_BUSY_OR_START           = (JD9365T_FLASH_BASE_ADDR << 8) + 0x1D,
};

enum JD9365T_SOC_REG_ADDR {
	JD9365T_SOC_BASE_ADDR               = 0x400080,
	JD9365T_SOC_REG_ADDR_RGU_0          = (JD9365T_SOC_BASE_ADDR << 8) + 0x04,
	JD9365T_SOC_REG_ADDR_CPU_SOFT_RESET = (JD9365T_SOC_BASE_ADDR << 8) + 0x12,
	JD9365T_SOC_REG_ADDR_CPU_STATUS     = (JD9365T_SOC_BASE_ADDR << 8) + 0x13,
	JD9365T_SOC_REG_ADDR_PRAM_PROG      = (JD9365T_SOC_BASE_ADDR << 8) + 0x14,
	JD9365T_SOC_REG_ADDR_CPU_CLK_STOP   = (JD9365T_SOC_BASE_ADDR << 8) + 0x16,
	JD9365T_SOC_REG_ADDR_SLPOUT         = (JD9365T_SOC_BASE_ADDR << 8) + 0x45,
	JD9365T_SOC_REG_ADDR_SLPOUT_INT_EN  = (JD9365T_SOC_BASE_ADDR << 8) + 0x44,
	JD9365T_SOC_REG_ADDR_CHIP_ID0       = (JD9365T_SOC_BASE_ADDR << 8) + 0x74,
	JD9365T_SOC_REG_ADDR_CHIP_ID1       = (JD9365T_SOC_BASE_ADDR << 8) + 0x75,
	JD9365T_SOC_REG_ADDR_CHIP_ID2       = (JD9365T_SOC_BASE_ADDR << 8) + 0x76,
	JD9365T_SOC_REG_ADDR_CHIP_ID3       = (JD9365T_SOC_BASE_ADDR << 8) + 0x77,
};

enum JD9365T_SOC_PASSWORD {
	JD9365T_SOC_PASSWORD_SOC_RESET       = 0xA5,
	JD9365T_SOC_PASSWORD_MCU_RESET       = 0xA5,
	JD9365T_SOC_PASSWORD_STOP_MCU        = 0x00,
	JD9365T_SOC_PASSWORD_START_MCU       = 0xF1,
	JD9365T_SOC_PASSWORD_STOP_MCU_CLOCK  = 0x5A,
	JD9365T_SOC_PASSWORD_START_MCU_CLOCK = 0xA5,
};

enum JD9365T_STC1_REG_ADDR {
	JD9365T_STC1_BASE_ADDR              = 0x400081,
	JD9365T_STC1_REG_ADDR_XSI_IF_EN     = (JD9365T_STC1_BASE_ADDR << 8) + 0x00,
	JD9365T_STC1_REG_ADDR_XSI_ADDR      = (JD9365T_STC1_BASE_ADDR << 8) + 0x01,
	JD9365T_STC1_REG_ADDR_XSI_WDATA     = (JD9365T_STC1_BASE_ADDR << 8) + 0x02,
	JD9365T_STC1_REG_ADDR_XSI_CMD       = (JD9365T_STC1_BASE_ADDR << 8) + 0x03,
	JD9365T_STC1_REG_ADDR_XSI_RDATA_L   = (JD9365T_STC1_BASE_ADDR << 8) + 0x04,
	JD9365T_STC1_REG_ADDR_XSI_RDATA_R   = (JD9365T_STC1_BASE_ADDR << 8) + 0x05,
	JD9365T_STC1_REG_ADDR_XSI_INT_CLR   = (JD9365T_STC1_BASE_ADDR << 8) + 0x08,
	JD9365T_STC1_REG_ADDR_XSI_INT_FLAG  = (JD9365T_STC1_BASE_ADDR << 8) + 0x09,
	JD9365T_STC1_REG_ADDR_AFE_SCAN_CTRL = (JD9365T_STC1_BASE_ADDR << 8) + 0x0D,
	JD9365T_STC1_REG_ADDR_SCAN_STATUS   = (JD9365T_STC1_BASE_ADDR << 8) + 0x52,
};

enum JD9365T_STC1_RELATED_SETTING {
	/* 0x00[0] */
	JD9365T_STC1_RELATED_SETTING_STC_CLOCK_DISABLE = 0x00,
	JD9365T_STC1_RELATED_SETTING_STC_CLOCK_ENABLE  = 0x01,
	/* 0x0D[0] */
	JD9365T_STC1_RELATED_SETTING_STC_SCAN_DISABLE  = 0x00,
	JD9365T_STC1_RELATED_SETTING_STC_SCAN_ENABLE   = 0x01,
	/* 0xCF[0] */
	JD9365T_STC1_RELATED_SETTING_SCAN_DONE         = 0x00,
	JD9365T_STC1_RELATED_SETTING_SCAN_BUSY         = 0x01,
};

#endif
