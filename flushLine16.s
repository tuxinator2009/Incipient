# SystemInit.S by FManga
# This code is in the PUBLIC DOMAIN

.code 16
.syntax unified

# LPC
.equ LPC_SCT_CONFIG, 0x5000C000
.equ LPC_SCT_CTRL, 0x5000C004

.equ LPC_SYSPLLCTRL,    0x40048008  
.equ LPC_SYSOSCCTRL,    0x40048020
.equ LPC_SYSPLLCLKSTAT, 0x4004800C
.equ LPC_SYSPLLCLKSEL,  0x40048040
.equ LPC_SYSPLLCLKUEN,  0x40048044
.equ LPC_SYSAHBCLKCTRL, 0x40048080

.equ LPC_MAINCLKSEL,   0x40048070
.equ LPC_MAINCLKUEN,   0x40048074
.equ LPC_SYSAHBCLKDIV, 0x40048078
.equ LPC_USBPLLCTRL,   0x40048010
.equ LPC_USBPLLSTAT,   0x40048014
.equ LPC_USBPLLCLKSEL, 0x40048048
.equ LPC_USBPLLCLKUEN, 0x4004804C
.equ LPC_USBCLKSEL,    0x400480C0
.equ LPC_USBCLKDIV,    0x400480C8
.equ LPC_PDRUNCFG,   0x40048238
.equ LPC_SYSPLLSTAT, 0x4004800C
.equ LPC_SSP0CLKDIV, 0x40048094

.equ LPC_RTCCTRL, 0x40024000
     
.equ LPC_PRESETCTRL, 0x40048004

.equ LPC_SPI0_CR0, 0x40040000
.equ LPC_SPI0_CR1, 0x40040004
.equ LPC_SPI0_CPSR, 0x40040010

.equ LPC_GPIO_PORT_DIR0, 0xA0002000
.equ LPC_GPIO_PORT_DIR1, 0xA0002004
.equ LPC_GPIO_PORT_DIR2, 0xA0002008

.equ LPC_GPIO_PORT_MASK0, 0xA0002080
.equ LPC_GPIO_PORT_MASK1, 0xA0002084
.equ LPC_GPIO_PORT_MASK2, 0xA0002088

.equ LPC_GPIO_PORT_PIN0, 0xA0002100
.equ LPC_GPIO_PORT_PIN1, 0xA0002104
.equ LPC_GPIO_PORT_PIN2, 0xA0002108

.equ LPC_GPIO_PORT_MPIN0, 0xA0002180
.equ LPC_GPIO_PORT_MPIN1, 0xA0002184
.equ LPC_GPIO_PORT_MPIN2, 0xA0002188
	
.equ LPC_GPIO_PORT_CLR0, 0xA0002280
.equ LPC_GPIO_PORT_CLR1, 0xA0002284
.equ LPC_GPIO_PORT_CLR2, 0xA0002288

.equ LPC_GPIO_PORT_SET0, 0xA0002200
.equ LPC_GPIO_PORT_SET1, 0xA0002204
.equ LPC_GPIO_PORT_SET2, 0xA0002208

.equ LPC_GPIO_PORT_NOT0, 0xA0002300
.equ LPC_GPIO_PORT_NOT1, 0xA0002304
.equ LPC_GPIO_PORT_NOT2, 0xA0002308

.equ LCD_CD_PORT, 0
.equ LCD_CD_PIN, 2
.equ LCD_WR_PORT, 1
.equ LCD_WR_PIN, 12
.equ LCD_RD_PORT, 1
.equ LCD_RD_PIN, 24
.equ LCD_RES_PORT, 1
.equ LCD_RES_PIN, 0

.equ LCD_CD_SET, LPC_GPIO_PORT_SET0
.equ LCD_CD_CLR, LPC_GPIO_PORT_CLR0
.equ LCD_WR_SET, LPC_GPIO_PORT_SET1
.equ LCD_WR_CLR, LPC_GPIO_PORT_CLR1
.equ LCD_RD_SET, LPC_GPIO_PORT_SET1
.equ LCD_RD_CLR, LPC_GPIO_PORT_CLR1
.equ LCD_RES_SET, LPC_GPIO_PORT_SET1
.equ LCD_RES_CLR, LPC_GPIO_PORT_CLR1
.equ LCD_MPIN, LPC_GPIO_PORT_MPIN2

.equ PIO0_6, 0x40044018
.equ PIO0_8, 0x40044020     
.equ PIO0_9, 0x40044024     
.equ PIO1_31, 0x400440DC
.equ PIO2_0,  0x400440F0
.equ PIO2_1,  0x400440F4
.equ PIO2_2,  0x400440FC
     
.equ ARM_NVIC_ISER, 0xe000e100
     
.macro lcd_clr_cd scratch_bit:req, scratch_ptr:req
	movs \scratch_bit, (1<<LCD_CD_PIN)
	ldr \scratch_ptr, =LPC_GPIO_PORT_CLR0
	str \scratch_bit, [\scratch_ptr]
.endm

.macro lcd_write data:req, scratch:req
	ldr \scratch, =LPC_GPIO_PORT_MPIN0
	lsls \data, \data, #3
	str \data, [\scratch, #8]
.endm	

# PVCOPY

.macro SET REG:req, VAL:req
.word \REG
.word \VAL
.endm

.macro CALL REG:req, VAL:req
.word (\REG)+1
.word \VAL
.endm

.macro OR REG:req, VAL:req
.word \REG+2
.word \VAL
.endm

.macro AND REG:req, VAL:req
.word \REG+3
.word \VAL
.endm

.macro END
.word 0
.endm


.global flushLine16

        // LCD output
.func flushLine16
flushLine16:
LINE .req r0
X .req r1
LCD .req r2
TMP .req r3
WRBIT .req r4
CLR .req r5
OUT .req r6
        push {r4-r6, lr}
        ldr X, =-220*2
        subs LINE, X
        ldr WRBIT, =1<<12
        ldr CLR,   =252
        ldr LCD,   =0xA0002188

        ldrh OUT, [LINE, X]

1:
        lsls OUT, 3
        str OUT, [LCD]
        str WRBIT, [LCD, CLR]
        adds X, 2
        ldrh OUT, [LINE, X]
        str WRBIT, [LCD, 124]

        lsls OUT, 3
        str OUT, [LCD]
        str WRBIT, [LCD, CLR]
        adds X, 2
        ldrh OUT, [LINE, X]
        str WRBIT, [LCD, 124]

        lsls OUT, 3
        str OUT, [LCD]
        str WRBIT, [LCD, CLR]
        adds X, 2
        ldrh OUT, [LINE, X]
        str WRBIT, [LCD, 124]

        lsls OUT, 3
        str OUT, [LCD]
        str WRBIT, [LCD, CLR]
        adds X, 2
        ldrh OUT, [LINE, X]
        str WRBIT, [LCD, 124]

        blt 1b
        pop {r4-r6, pc}
.pool
.endFunc
.unreq LINE
.unreq X
.unreq LCD
.unreq TMP
.unreq WRBIT
.unreq CLR
.unreq OUT
