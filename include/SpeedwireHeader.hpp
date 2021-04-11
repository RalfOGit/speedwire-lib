#ifndef __SPEEDWIREPROTOCOL_H__
#define __SPEEDWIREPROTOCOL_H__

#include <cstdint>


/**
 * Class for parsing and assembling of speedwire protocol headers.
 * 
 * The header is in the first 24 bytes of a speedwire udp packet. The header format is
 * described in a public technical SMA document: "SMA Energy Meter Z�hlerprotokoll". The
 * english version is called "SMA Energy Meter Protocol" and can be found here:
 * https://www.sma.de/fileadmin/content/global/Partner/Documents/SMA_Labs/EMETER-Protokoll-TI-en-10.pdf
 */

class SpeedwireHeader {

protected:
    static constexpr uint8_t  sma_signature[4] = { 0x53, 0x4d, 0x41, 0x00 };     // "SMA\0"
    static constexpr uint8_t  sma_tag0[4]      = { 0x00, 0x04, 0x02, 0xa0 };     // length: 0x0004  tag: 0x02a0;
    static constexpr uint8_t  sma_net_v2[2]    = { 0x00, 0x10 };

    static constexpr unsigned long sma_signature_offset  = 0;
    static constexpr unsigned long sma_tag0_offset       = sizeof(sma_signature);
    static constexpr unsigned long sma_group_offset      = sma_tag0_offset + sizeof(sma_tag0);
    static constexpr unsigned long sma_length_offset     = sma_group_offset + 4;
    static constexpr unsigned long sma_netversion_offset = sma_length_offset + 2;
    static constexpr unsigned long sma_protocol_offset   = sma_netversion_offset + sizeof(sma_net_v2);
    static constexpr unsigned long sma_protocol_size     = 2;
    static constexpr unsigned long sma_long_words_offset = sma_protocol_offset + sma_protocol_size;
    static constexpr unsigned long sma_control_offset    = sma_long_words_offset + 1;
    static constexpr unsigned long sma_control_size      = 1;

    uint8_t *udp;
    unsigned long size;

public:

    static constexpr uint16_t sma_emeter_protocol_id    = 0x6069;
    static constexpr uint16_t sma_inverter_protocol_id  = 0x6065;
    static constexpr uint16_t sma_discovery_protocol_id = 0xffff;


    SpeedwireHeader(const void *const udp_packet, const unsigned long udp_packet_size);
    ~SpeedwireHeader(void);

    bool checkHeader(void) const;

    // getter methods to retrieve header fields
    uint32_t getSignature(void) const;
    uint32_t getTag0(void) const;
    uint32_t getGroup(void) const;
    uint16_t getLength(void) const;
    uint16_t getNetworkVersion(void) const;
    uint16_t getProtocolID(void) const;
    uint8_t  getLongWords(void) const;
    uint8_t  getControl(void) const;
    bool isEmeterProtocolID(void) const;
    bool isInverterProtocolID(void) const;

    // setter methods to fill header fields
    void setDefaultHeader(void);
    void setDefaultHeader(uint32_t group, uint16_t length, uint16_t protocolID);
    void setSignature(uint32_t value);
    void setTag0(uint32_t value);
    void setGroup(uint32_t value);
    void setLength(uint16_t value);
    void setNetworkVersion(uint16_t value);
    void setProtocolID(uint16_t value);
    void setLongWords(uint8_t value);
    void setControl(uint8_t value);

    unsigned long getPayloadOffset(void) const;     
    uint8_t* getPacketPointer(void) const;
    unsigned long getPacketSize(void) const;
};

#endif
