/************************************************************
 *
 * usb.hpp
 *
 * USB peripheral hardware-access primitives:
 * PMA read/write, and the EPnR safe-write helper.
 *
 * Peripheral bring-up (usb_init, the interrupt handler).
 *
 ***********************************************************/


#pragma  once
#include "stm32f103.hpp"
#include <array>
#include <span>


// ============================================================================
// Global USB SETUP Seen
//
// This flag is set when the first valid SETUP packet is received on endpoint 0.
// It remains set until explicitly cleared by firmware, allowing software to 
// record that EP0 has received SETUP traffic.
// ============================================================================
extern volatile bool g_usb_setup_seen;


// ============================================================================
// EP0 PMA allocation constants:
//
// Defines the maximum EP0 packet size and the PMA-local offsets of the BTABLE,
// EP0 RX buffer, and EP0 TX buffer. The BTABLE occupies 8 bytes per endpoint 
// for all 8 endpoints (64 bytes total), EP0 RX begins immediately after the 
// BTABLE, and the EP0 TX begins immediately after the 64-byte EP0 RX buffer.
// ============================================================================
constexpr u32 EP0_MAX_PACKET_SIZE   = 64;
constexpr u32 BTABLE_LOCAL_SIZE     = ( 8 * 8 );
constexpr u32 EP0_RX_BUFFER_OFFSET  = BTABLE_LOCAL_SIZE;
constexpr u32 EP0_TX_BUFFER_OFFSET  = EP0_RX_BUFFER_OFFSET + EP0_MAX_PACKET_SIZE;


// ============================================================================
// EP_Num_t
//
// Defines a typed identifier for the 8 hardware endpoints (EP0..EP7) to prevent
// parameter-swapping bugs and guarantee valid endpoint range bounds.
// ============================================================================
enum class EP_Num_t : u8 {
    EP0 = 0,
    EP1 = 1,
    EP2 = 2,
    EP3 = 3,
    EP4 = 4,
    EP5 = 5,
    EP6 = 6,
    EP7 = 7
};


// ============================================================================
// BTABLE_entry_t
//
// Represents the hardware descriptor entry for a single endpoint in the USB
// Buffer Table (BTABLE), located inside Packet Memory Area (PMA). The USB
// Serial Interface Engine (SIE) automatically accesses these descriptors 
// during packet transfers to locate hardware buffers and manage byte counts.
//
//  • ADDR_TX   : Starting PMA byte offset for the transmit (IN) buffer.
//  • COUNT_TX  : Number of bytes to transmit on the next IN token.
//  • ADDR_RX   : Starting PMA byte offset for the receive (OUT/SETUP) buffer.
//  • COUNT_RX  : Allocated RX buffer capacity (write) / Bytes received (read).
// ============================================================================
struct BTABLE_entry_t {
    PMA_Word_t ADDR_TX;
    PMA_Word_t COUNT_TX;
    PMA_Word_t ADDR_RX;
    PMA_Word_t COUNT_RX;
};

static_assert(sizeof(BTABLE_entry_t) == 16, "BTABLE_entry_t size mismatch");


// ============================================================================
// PMA_BLSIZE_t
//
// Defines the allocation block size (BLSIZE, bit 15) for the endpoint reception
// buffer capacity field (COUNT_RX_n) in the USB Buffer Table (BTABLE).
//
//  • Small_2Bytes  [BLSIZE = 0] : 2-byte block step
//
//  • Large_32Bytes [BLSIZE = 1] : 32-byte block step
// ============================================================================
enum class PMA_BLSIZE_t : u8 {
  Small_2Bytes  = 0,
  Large_32Bytes = 1
};


// ============================================================================
// BTABLE_t
//
// Models the USB Buffer Descriptor Table (BTABLE) as eight endpoint descriptor
// entries, with one BTABLE_entry_t for each endpoint. Each descriptor contains 
// the PMA-local TX/RX buffer addresses and their corresponding buffer-size 
// fields.
// ============================================================================
using BTABLE_t = std::array<BTABLE_entry_t, 8>;

inline BTABLE_t& USB_BTABLE = *reinterpret_cast<BTABLE_t*>(0x40006000U);


// ============================================================================
// pma_count_rx_encode()
//
// Encodes the maximum RX buffer capacity into the 16-bit format 
// required by the USB endpoint Buffer Table (BTABLE) 'COUNT_RX_n'
// entry.
//              
//              BTABLE 'COUNT_RX_n' Bitfield Layout
//      ┌──────────┬──────────────────┬──────────────────────┐
//      │  BIT 15  |    BITS 14:10    |        BITS 9:0      |
//      ├──────────┼──────────────────┼──────────────────────┤
//      |  BLSIZE  |    NUM_BLOCK     |      COUNT_RX        |
//      | (write)  |    (write)       | (Hardware written    |
//      |          |                  | on Packet Reception) |
//      └──────────┴──────────────────┴──────────────────────┘ 
//
// Buffer Capacity Encoding Rules:
//
// 1. Small Buffer Mode [BLSIZE = 0] (Capacity: 2 to 62 bytes)
//    • Index Starts from 1
//    • Capacity = NUM_BLOCK x 2
//    • NUM_BLOCK = ( max_bytes + 1 ) / 2
//
// 2. Large Buffer Mode [BLSIZE = 1] (Capacity: 32 to 512 bytes)
//    • Index Starts from 0
//    • Capacity = ( NUM_BLOCK + 1 ) x 32
//    • NUM_BLOCK = ((max_bytes + 31) / 32) - 1 
// 
// max_bytes : Desired RX buffer capacity in bytes.
// ============================================================================
constexpr u16 pma_count_rx_encode( u16 byte_size, PMA_BLSIZE_t block_size ) {
    if ( byte_size == 0 ) {
        return 0;
    }

    // Large Block: Step = 32 bytes, BLSIZE = 1
    // NUM_BLOCK = ceil(byte_size / 32) - 1
    if ( block_size == PMA_BLSIZE_t::Large_32Bytes ) {
        const u16 num_blocks_minus_one = static_cast<u16>((byte_size - 1U) >> 5);
        return static_cast<u16>((1U << 15) | ((num_blocks_minus_one & 0x1FU) << 10));
    }

    // Small Block: Step = 2 bytes, BLSIZE = 0
    // NUM_BLOCK = ceil(byte_size / 2)
    const u16 num_blocks = static_cast<u16>((byte_size + 1U) >> 1);
    return static_cast<u16>((num_blocks & 0x1FU) << 10);   
}


// ============================================================================
// Read/Write one 16-bit slot in PMA, given its LOCAL OFFSET (0 - 511).
// ============================================================================
void pma_write_word( u32 local_offset, u16 value );
u16  pma_read_word ( u32 local_offset );


// ============================================================================
// Copy a contiguous buffer of 16-bit words into PMA starting at a given LOCAL 
// OFFSET.
// ============================================================================
void pma_write_words( u32 local_offset, std::span<const u16> src );


// ============================================================================
// Change STAT_TX/STAT_RX on one endpoint's EPnR safely, at any time.
// Avoid a blind read-modify-write.
// ============================================================================
void ep_set_status( EP_Num_t ep_num, u16 new_stat_tx, u16 new_stat_rx );


// ============================================================================
// Initializes the USB peripheral for device operation by powering up the USB
// Analog Transceiver and the Digital SIE, configuring the PMA BTABLE and control
// endpoint (EP0), and enabling the RESET and Correct Transfer (CTR) interrupt
// sources.
// NOTE: Call after clock_init(), because the USB peripheral depends on the 
// APB1 register interface and the 48 MHz USB clock required by the SIE and 
// analog transceiver.
// ============================================================================
void usb_init();


  /*--------------------------------------------------------------------------*/
 /*                            This is the END                               */
/*--------------------------------------------------------------------------*/