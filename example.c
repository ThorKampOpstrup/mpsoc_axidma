#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "axidma/pinner.h"
#include "axidma/axidma.h"
#include "axidma/pinner_fns.h"

#define DATA_BUF_SIZE 10000
//Ensure DATA_BUF_SIZE >= NUM_BUFFERS*BUFFER_SZ
#define NUM_BUFFERS 10
#define BUFFER_SZ 1600

int main(int argc, char **argv) {
    if (argc != 2) {
        puts("Usage: example /dev/uioN, where /dev/uioN is the AXI DMA driver");
        return -1;
    }
    
    axidma_ctx *ctx = axidma_open(argv[1]);
    if (!ctx) {
        return -1;
    }
    
    int pinner_fd = pinner_open();
    
    char *tx_sg_buf = malloc(5000);
    char *rx_sg_buf = malloc(5000);

    //For the sake of testing, poison the initial SG list to see what's going on
    for (int i = 0; i < 5000; i++) {
        tx_sg_buf[i] = 0x00;
        rx_sg_buf[i] = 0x00;
    }
    
    char *tx_buf = malloc(sizeof(int)*DATA_BUF_SIZE);
    char *rx_buf = malloc(sizeof(int)*DATA_BUF_SIZE);
    for (int i = 0; i < DATA_BUF_SIZE; i++) {
        tx_buf[i] = i;
        rx_buf[i] = 0x00;
    }
    
    // Pin tx 
    struct pinner_physlist tx_sg_plist;
    struct pinner_handle tx_sg_handle;
    int rc = pin_buf(pinner_fd, tx_sg_buf, 5000, &tx_sg_handle, &tx_sg_plist);
    if (rc < 0) {
        return -1;
    }

    struct pinner_physlist tx_data_plist;
    struct pinner_handle tx_data_handle;
    rc = pin_buf(pinner_fd, tx_buf, DATA_BUF_SIZE, &tx_data_handle, &tx_data_plist);
    if (rc < 0) {
        return -1;
    }


    // Pin rx
    struct pinner_physlist rx_sg_plist;
    struct pinner_handle rx_sg_handle;
    rc = pin_buf(pinner_fd, rx_sg_buf, 5000, &rx_sg_handle, &rx_sg_plist);
    if (rc < 0) {
        return -1;
    }

    struct pinner_physlist rx_data_plist;
    struct pinner_handle rx_data_handle;
    rc = pin_buf(pinner_fd, rx_buf, DATA_BUF_SIZE, &rx_data_handle, &rx_data_plist);
    if (rc < 0) {
        return -1;
    }

    //set rx list
    sg_list *rx_lst = axidma_list_new(rx_sg_buf, &rx_sg_plist, rx_buf, &rx_data_plist);
    for (int i = 0; i < NUM_BUFFERS; i++) {
        rc = axidma_add_entry(rx_lst, BUFFER_SZ);
        if (rc == ADD_ENTRY_SG_OOM) {
            puts("Ran out of memory for SG descriptors");
            break;
        } else if (rc == ADD_ENTRY_BUF_OOM) {
            puts("Ran out of memory for data");
            break;
        } else if (rc == ADD_ENTRY_ERROR) {
            puts("Unrecoverable error");
            exit(-1);
        }
    }
    
    axidma_write_sg_list(ctx, rx_lst, pinner_fd, &rx_sg_handle);
    //recieve information is now set.

    //set tx list
    sg_list *tx_lst = axidma_list_new(tx_sg_buf, &tx_sg_plist, tx_buf, &tx_data_plist);
    for (int i = 0; i < NUM_BUFFERS; i++) {
        rc = axidma_add_entry(tx_lst, BUFFER_SZ);
        if (rc == ADD_ENTRY_SG_OOM) {
            puts("Ran out of memory for SG descriptors");
            break;
        } else if (rc == ADD_ENTRY_BUF_OOM) {
            puts("Ran out of memory for data");
            break;
        } else if (rc == ADD_ENTRY_ERROR) {
            puts("Unrecoverable error");
            exit(-1);
        }
    }
    
    axidma_write_sg_list(ctx, tx_lst, pinner_fd, &tx_sg_handle);

    //Start tx transfer
    axidma_mm2s_transfer(ctx, 0, 0);
    
    axidma_s2mm_transfer(ctx, 1, 0);
    
    s2mm_buf buf = axidma_dequeue_s2mm_buf(rx_lst);
    int cnt = 0;
    while (buf.code != END_OF_LIST) {
        
        if(buf.code == TRANSFER_SUCCESS) cnt++;
        printf("Received %s buffer of length %u:\n", (buf.code == TRANSFER_SUCCESS ? "good" : "bad"), buf.len);
        for (int i = 0; i < (buf.len) / 64; i++) {
            uint32_t *word_ptr = ((uint32_t*) buf.base) + 16*i;
            for (int j = 15; j >= 0; j--) {
                printf("%08x ", word_ptr[j]);
            }
        }
        puts("");
        
        buf = axidma_dequeue_s2mm_buf(rx_lst);
    }
    
    printf("Got %d good packets out of %d\n", cnt, NUM_BUFFERS);
    
    axidma_list_del(rx_lst);
    axidma_list_del(tx_lst);
    
    axidma_close(ctx);
    
    unpin_buf(pinner_fd, &tx_sg_plist);
    unpin_buf(pinner_fd, &tx_data_plist);
    unpin_buf(pinner_fd, &rx_sg_plist);
    unpin_buf(pinner_fd, &rx_data_plist);

    pinner_close(pinner_fd);
    return 0;
}
