#ifndef MAX22200_REGS_H
#define MAX22200_REGS_H

#define MAX22200_FRAME_SIZE        4
#define MAX22200_CHANNELS          8
#define MAX22200_CHANNELS_CONFIG   4
#define MAX22200_MAX_CHN_IDX       7


//ADDERSSES


#define MAX22200_ADDR_STATUS   0x00
#define MAX22200_ADDR_CH0      0x01
#define MAX22200_ADDR_CH1      0x02
#define MAX22200_ADDR_CH2      0x03
#define MAX22200_ADDR_CH3      0x04
#define MAX22200_ADDR_CH4      0x05
#define MAX22200_ADDR_CH5      0x06
#define MAX22200_ADDR_CH6      0x07
#define MAX22200_ADDR_CH7      0x08
#define MAX22200_ADDR_FAULT    0x09
#define MAX22200_ADDR_CFG_DPM  0x0A

//COMMAND REGISTRY


#define MAX22200_CMD_RW_POS        7
#define MAX22200_CMD_ADDR_POS      1
#define MAX22200_CMD_8BIT_POS      0

#define MAX22200_CMD_READ          (0x00 << MAX22200_CMD_RW_POS)
#define MAX22200_CMD_WRITE         (0x01 << MAX22200_CMD_RW_POS)

#define MAX22200_CMD_ADDR_MASK     (0x0F << MAX22200_CMD_ADDR_POS)

#define MAX22200_CMD_32BIT_MODE    (0x00 << MAX22200_CMD_8BIT_POS)
#define MAX22200_CMD_8BIT_MODE     (0x01 << MAX22200_CMD_8BIT_POS)

//STATUS


#define MAX22200_STATUS_ONCH_POS   24

#define MAX22200_ONCH_CH0          (0x01 << 0)
#define MAX22200_ONCH_CH1          (0x01 << 1)
#define MAX22200_ONCH_CH2          (0x01 << 2)
#define MAX22200_ONCH_CH3          (0x01 << 3)
#define MAX22200_ONCH_CH4          (0x01 << 4)
#define MAX22200_ONCH_CH5          (0x01 << 5)
#define MAX22200_ONCH_CH6          (0x01 << 6)
#define MAX22200_ONCH_CH7          (0x01 << 7)

#define MAX22200_STATUS_M_POS      16

#define MAX22200_MASK_OVT          (0x01 << 7)
#define MAX22200_MASK_OCP          (0x01 << 6)
#define MAX22200_MASK_OLF          (0x01 << 5)
#define MAX22200_MASK_HHF          (0x01 << 4)
#define MAX22200_MASK_DPM          (0x01 << 3)
#define MAX22200_MASK_COMF         (0x01 << 2)
#define MAX22200_MASK_UVM          (0x01 << 1)
#define MAX22200_FREQM             (0x01 << 0)

// --- MASKI CZYSZCZĄCE (potrzebne do resetowania konfiguracji pary) ---
#define MAX22200_STATUS_CM_10_MASK       (0x03 << 8)
#define MAX22200_STATUS_CM_32_MASK       (0x03 << 10)
#define MAX22200_STATUS_CM_54_MASK       (0x03 << 12)
#define MAX22200_STATUS_CM_76_MASK       (0x03 << 14)

// --- KONFIGURACJA DLA PARY CH1 i CH0 (Bity 9:8) ---
#define MAX22200_STATUS_CM_10_SINGLE     (0x00 << 8)
#define MAX22200_STATUS_CM_10_PARALLEL   (0x01 << 8)
#define MAX22200_STATUS_CM_10_FULL       (0x02 << 8)

// --- KONFIGURACJA DLA PARY CH3 i CH2 (Bity 11:10) ---
#define MAX22200_STATUS_CM_32_SINGLE     (0x00 << 10)
#define MAX22200_STATUS_CM_32_PARALLEL   (0x01 << 10)
#define MAX22200_STATUS_CM_32_FULL       (0x02 << 10)

// --- KONFIGURACJA DLA PARY CH5 i CH4 (Bity 13:12) ---
#define MAX22200_STATUS_CM_54_SINGLE     (0x00 << 12)
#define MAX22200_STATUS_CM_54_PARALLEL   (0x01 << 12)
#define MAX22200_STATUS_CM_54_FULL       (0x02 << 12)

// --- KONFIGURACJA DLA PARY CH7 i CH6 (Bity 15:14) ---
#define MAX22200_STATUS_CM_76_SINGLE     (0x00 << 14)
#define MAX22200_STATUS_CM_76_PARALLEL   (0x01 << 14)
#define MAX22200_STATUS_CM_76_FULL       (0x02 << 14)

#define MAX22200_STATUS_F_POS      0

#define MAX22200_STATUS_OVT        (0x01 << 7)
#define MAX22200_STATUS_OCP        (0x01 << 6) 
#define MAX22200_STATUS_OLF        (0x01 << 5)
#define MAX22200_STATUS_HHF        (0x01 << 4)
#define MAX22200_STATUS_DPM        (0x01 << 3)
#define MAX22200_STATUS_COM        (0x01 << 2)
#define MAX22200_STATUS_UVM        (0x01 << 1)
#define MAX22200_STATUS_ACTIVE     (0x01 << 0)

//CHANNELS


#define MAX22200_HFS_POS            31
#define MAX22200_HFS_MASK           0x01   

#define MAX22200_HOLD_POS           24
#define MAX22200_HOLD_MASK          0x7F

#define MAX22200_TRGNSP_IO_POS      23
#define MAX22200_TRGNSP_IO_MASK     0x01
 
#define MAX22200_HIT_POS            16
#define MAX22200_HIT_MASK           0x7F

#define MAX22200_HIT_T_POS          8
#define MAX22200_HIT_T_MASK         0xFF

#define MAX22200_VDRNCDR_POS        7
#define MAX22200_VDRNCDR_MASK       0x01

#define MAX22200_HSNLS_POS          6
#define MAX22200_HSNLS_MASK         0x01

#define MAX22200_FREQ_CFG_POS       4
#define MAX22200_FREQ_CFG_MASK      0x03

#define MAX22200_SRC_POS            3
#define MAX22200_SRC_MASK           0x01

#define MAX22200_OL_EN_POS          2
#define MAX22200_OL_EN_MASK         0x01

#define MAX22200_DPM_EN_POS         1
#define MAX22200_DPM_EN_MASK        0x01

#define MAX22200_HHF_EN_POS         0
#define MAX22200_HHF_EN_MASK        0x01

//FAULT


#define MAX22200_OCP_POS            24
#define MAX22200_OCP_MASK           0xFF

#define MAX22200_HHF_POS            16
#define MAX22200_HHF_MASK           0xFF

#define MAX22200_OLF_POS            8
#define MAX22200_OLF_MASK           0xFF

#define MAX22200_DPM_POS            0
#define MAX22200_DPM_MASK           0xFF

//DPM


//bity 15-31 reserved (ustawione na 0)

#define MAX22200_DPM_ISTART_POS     8
#define MAX22200_DPM_ISTART_MASK    0x7F

#define MAX22200_DPM_TDEB_POS       4
#define MAX22200_DPM_TDEB_MASK      0x0F

#define MAX22200_DPM_IPTH_POS       0
#define MAX22200_DPM_IPTH_MASK      0x0F

#endif //MAX22200_REGS_H