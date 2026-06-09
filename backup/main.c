#include "sys/alt_stdio.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include <stdint.h>

// ---------------------------------------------------------
// GLOBAL VARIABLES & LOOKUP TABLES
// ---------------------------------------------------------

// Active-Low 7-segment lookup table (0 to F)
const uint8_t hex7_map[16] = {
    0x40, // 0
    0x79, // 1
    0x24, // 2
    0x30, // 3
    0x19, // 4
    0x12, // 5
    0x02, // 6
    0x78, // 7
    0x00, // 8
    0x10, // 9
    0x08, // A
    0x03, // B
    0x46, // C
    0x21, // D
    0x06, // E
    0x0E  // F
};

// ---------------------------------------------------------
// 1. INPUT MODULE (READ)
// ---------------------------------------------------------
void read_inputs(uint8_t *operand_a, uint8_t *operand_b) {
    // IORD always reads the full register width (up to 32 bits), 
    // capturing the state of all 16 switches at once.
    uint32_t switch_data = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);

    *operand_a = switch_data & 0xFF;

    *operand_b = (switch_data >> 8) & 0xFF;
}

// ---------------------------------------------------------
// 2. PROCESSING MODULE (ALU)
// ---------------------------------------------------------
uint16_t calculate_sum(uint8_t a, uint8_t b) {
    return a + b;
}

// ---------------------------------------------------------
// 3. OUTPUT MODULE (DISPLAY)
// ---------------------------------------------------------
void update_display(uint8_t val_a, uint8_t val_b, uint16_t sum_result) {
    // A
    uint8_t a_low = val_a & 0x0F;
    uint8_t a_high = (val_a >> 4) & 0x0F;
    
    IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, hex7_map[a_low]);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, hex7_map[a_high]);

    // B
    uint8_t b_low = val_b & 0x0F;
    uint8_t b_high = (val_b >> 4) & 0x0F;
    
    IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, hex7_map[b_low]);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, hex7_map[b_high]);

    uint8_t res_low = sum_result & 0x0F;
    uint8_t res_mid = (sum_result >> 4) & 0x0F;
    uint8_t res_high = (sum_result >> 8) & 0x0F;

    IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, hex7_map[res_low]);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, hex7_map[res_mid]);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, hex7_map[res_high]);
}

// ---------------------------------------------------------
// 4. MAIN LOOP
// ---------------------------------------------------------
int main() {
    uint8_t val_a = 0;
    uint8_t val_b = 0;
    uint16_t result = 0;

    alt_putstr("Adder System Started!\n");

    while (1) {
        read_inputs(&val_a, &val_b);
        result = calculate_sum(val_a, val_b);
        update_display(val_a, val_b, result);
    }

    return 0;
}
