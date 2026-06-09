#include "sys/alt_stdio.h"
#include "system.h"             // Contains base addresses (e.g., SWITCHES_BASE, HEX0_BASE)
#include "altera_avalon_pio_regs.h" // Contains IORD and IOWR macros
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

    *operand_a = switch_data & 0x0F;

    *operand_b = (switch_data >> 8) & 0x0F;
}

// ---------------------------------------------------------
// 2. PROCESSING MODULE (ALU)
// ---------------------------------------------------------
uint8_t calculate_sum(uint8_t a, uint8_t b) {
    return a + b;
}

// ---------------------------------------------------------
// 3. OUTPUT MODULE (DISPLAY)
// ---------------------------------------------------------
void update_display(uint8_t sum_result) {
    // Hardware limitation check: 
    // A single display can't show numbers greater than 15.
    if (sum_result > 512) {
        // If overflow occurs, force the display to show an error state (e.g., all dashes or the letter 'E')
        IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, 0x06); // 0x06 is the Active-Low code for 'E'
    } else {
        IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, hex7_map[sum_result]);
    }
}

// ---------------------------------------------------------
// 4. MAIN LOOP
// ---------------------------------------------------------
int main() { 
    uint8_t val_a = 0;
    uint8_t val_b = 0;
    uint8_t result = 0;

    alt_putstr("Adder System Started!\n");

    while (1) {
        read_inputs(&val_a, &val_b);
        result = calculate_sum(val_a, val_b);
        update_display(result);
    }

    return 0;
}
