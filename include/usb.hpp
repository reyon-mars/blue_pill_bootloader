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
#include <span>


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
void ep_set_status( u8 ep_num, u16 new_stat_tx, u16 new_stat_rx );


  /*--------------------------------------------------------------------------*/
 /*                            This is the END                               */
/*--------------------------------------------------------------------------*/