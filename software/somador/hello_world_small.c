/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include "sys/alt_stdio.h"
#include "system.h"             // Contains base addresses (e.g., SWITCHES_BASE, HEX0_BASE)
#include "altera_avalon_pio_regs.h" // Contains IORD and IOWR macros
#include <stdint.h>
#include <unistd.h>

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
    // capturing the state of all 18 switches at once.
    uint32_t switch_data = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);

    // Masking with 0x0F (0000 1111 in binary) isolates the first 4 bits.
    // This maps SW[3:0] to operand_a.
    *operand_a = switch_data & 0x0F;

    // Shifting right by 4 brings SW[7:4] to the start.
    // Masking with 0x0F isolates them.
    *operand_b = (switch_data >> 4) & 0x0F;
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
    if (sum_result > 15) {
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
