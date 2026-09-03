#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void printSubnetMask(uint8_t bits_in_mask) {
    // Calculate subnet mask
    uint32_t mask = 0xFFFFFFFF << (32 - bits_in_mask);
    uint8_t mask_bytes[4];
    mask_bytes[0] = (mask >> 24) & 0xFF; //shifting to get the first bytes
    mask_bytes[1] = (mask >> 16) & 0xFF;
    mask_bytes[2] = (mask >> 8) & 0xFF;
    mask_bytes[3] = mask & 0xFF;

    printf("Subnet mask:       %d.%d.%d.%d\n", mask_bytes[0], mask_bytes[1], mask_bytes[2], mask_bytes[3]);
}

uint32_t getNetworkAddress(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint8_t bits_in_mask) {
    // Calculate network address
    uint32_t ip = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4; //using bit or to get each part of the binary ip address
    uint32_t mask = 0xFFFFFFFF << (32 - bits_in_mask);
    uint32_t network_address = ip & mask; // masking ip with the subnet mask to get network address
    return network_address;
}

void printNetworkAddress(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint8_t bits_in_mask) {
    // Print network address
    uint32_t network_address = getNetworkAddress(ip1, ip2, ip3, ip4, bits_in_mask);
    uint8_t address_bytes[4];
    address_bytes[0] = (network_address >> 24) & 0xFF;
    address_bytes[1] = (network_address >> 16) & 0xFF;
    address_bytes[2] = (network_address >> 8) & 0xFF;
    address_bytes[3] = network_address & 0xFF;

    printf("Network address:   %d.%d.%d.%d\n", address_bytes[0], address_bytes[1], address_bytes[2], address_bytes[3]);
}

uint32_t getBroadcastAddress(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint8_t bits_in_mask) {
    // Calculate broadcast address
    uint32_t network_address = getNetworkAddress(ip1, ip2, ip3, ip4, bits_in_mask);
    uint32_t mask = 0xFFFFFFFF << (32 - bits_in_mask);
    uint32_t inverted_mask = ~mask;
    uint32_t broadcast_address = network_address | inverted_mask;
    return broadcast_address;
}


void printBroadcastAddress(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint8_t bits_in_mask) {
    // Print broadcast address
    uint32_t broadcast_address = getBroadcastAddress(ip1, ip2, ip3, ip4, bits_in_mask);
    uint8_t address_bytes[4];
    address_bytes[0] = (broadcast_address >> 24) & 0xFF;
    address_bytes[1] = (broadcast_address >> 16) & 0xFF;
    address_bytes[2] = (broadcast_address >> 8) & 0xFF;
    address_bytes[3] = broadcast_address & 0xFF;

    printf("Broadcast address: %d.%d.%d.%d\n", address_bytes[0], address_bytes[1], address_bytes[2], address_bytes[3]);
}

void printIPRange(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint8_t bits_in_mask) {
    // Calculate and print usable IP range
    uint32_t network_address = getNetworkAddress(ip1, ip2, ip3, ip4, bits_in_mask) + 1;
    uint32_t broadcast_address = getBroadcastAddress(ip1, ip2, ip3, ip4, bits_in_mask) - 1;

    uint8_t start_ip1 = (network_address >> 24) & 0xFF;
    uint8_t start_ip2 = (network_address >> 16) & 0xFF;
    uint8_t start_ip3 = (network_address >> 8) & 0xFF;
    uint8_t start_ip4 = network_address & 0xFF;

    uint8_t end_ip1 = (broadcast_address >> 24) & 0xFF;
    uint8_t end_ip2 = (broadcast_address >> 16) & 0xFF;
    uint8_t end_ip3 = (broadcast_address >> 8) & 0xFF;
    uint8_t end_ip4 = broadcast_address & 0xFF;

    printf("Usable IP range:   %d.%d.%d.%d - %d.%d.%d.%d\n", start_ip1, start_ip2, start_ip3, start_ip4, end_ip1, end_ip2, end_ip3, end_ip4);
}

int main(int argc, char** argv){
    FILE* file;
    char* buffer;
    long fsize;
    size_t result;

    file = fopen(argv[1], "rb");
    if(file == NULL){
        printf("Error: File not found\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);

    buffer = (char*) malloc(fsize * sizeof(char));
    if(buffer == NULL){
        printf("Error: Invalid argument\n");
        fclose(file);
        return 1;
    }

    result = fread(buffer, 1, fsize, file);
    if(result != fsize){
        printf("Error: Invalid argument\n");
        fclose(file);
        free(buffer);
        return 1;
    }

    for (int i = 0; i < fsize; i += 5) {
        uint8_t ip1 = buffer[i];
        uint8_t ip2 = buffer[i + 1];
        uint8_t ip3 = buffer[i + 2];
        uint8_t ip4 = buffer[i + 3];
        uint8_t bits_in_mask = buffer[i + 4];

        printf("IP address:        %d.%d.%d.%d\n", ip1, ip2, ip3, ip4);
        printSubnetMask(bits_in_mask);

        printIPRange(ip1, ip2, ip3, ip4, bits_in_mask);

        printNetworkAddress(ip1, ip2, ip3, ip4, bits_in_mask);
        printBroadcastAddress(ip1, ip2, ip3, ip4, bits_in_mask);
        printf("\n");
    }

    fclose(file);
    free(buffer);
    return 0;
    //Github copilot assisted with the creation of the functions for this code.
}
