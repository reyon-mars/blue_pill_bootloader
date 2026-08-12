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

#pragma once
#include "stm32f103.hpp"
#include <span>

// ============================================================================
// Read/Write one 16-bit slot in PMA, given its LOCAL offset (0 - 511).
// ============================================================================
void pma_write_word( u32 local_offset, u16 value );
u16  pma_read_word( u32 local_offset );


// ============================================================================
// Copy an arbitrary byte buffer into PMA starting at a given local offset.
// ============================================================================
void pma_write_bytes( u32 local_offset, std::span<const std::byte> src );


// ============================================================================
// Encode a byte size into count_rx's | BL_SIZE | NUM_BLOCK | representation.
// Correct for sizes needing BL_SIZE = 1.
// ============================================================================
u16 pma_count_rx_encode( u16 byte_size );


// ============================================================================
// Change STAT_TX/STAT_RX on one endpoint's EPnR safely, at any time.
// Avoid a blind read-modify-write.
// ============================================================================
void ep_set_status( u8 ep_num, u16 new_stat_tx, u16 new_stat_rx );


  /*------------------------------------------------------------*/
 /*                  This is the END                           */
/*------------------------------------------------------------*/