// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "dataflow_api.h"

void kernel_main() {
    uint32_t dst_addr  = get_arg_val<uint32_t>(0);
    uint32_t dst_dram_bank_id = get_arg_val<uint32_t>(1);
    uint32_t num_tiles = get_arg_val<uint32_t>(2);
    uint32_t cb_id_out0 = get_arg_val<uint32_t>(3);
    uint32_t ublock_size_tiles = get_arg_val<uint32_t>(4);
    bool writer_only = get_arg_val<uint32_t>(5);

    uint32_t ublock_size_bytes = get_tile_size(cb_id_out0) * ublock_size_tiles;

    for (uint32_t i = 0; i < num_tiles; i += ublock_size_tiles) {
        uint64_t dst_noc_addr = get_noc_addr_from_bank_id<true>(dst_dram_bank_id, dst_addr);
        if (writer_only == false) {
            cb_wait_front(cb_id_out0, ublock_size_tiles);
        }
        uint32_t l1_read_addr = get_read_ptr(cb_id_out0);

        noc_async_write(l1_read_addr, dst_noc_addr, ublock_size_bytes);

        noc_async_write_barrier();
        if (writer_only == false) {
            cb_pop_front(cb_id_out0, ublock_size_tiles);
        }
        dst_addr += ublock_size_bytes;
    }
}
