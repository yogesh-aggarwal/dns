#include "PacketBuffer.hpp"

#include <DNS/Core/Error.hpp>

uint8_t
DNS::Utility::PacketBuffer::Get()
{
   return m_Buffer[m_ReadOffset];
}

Result<uint8_t>
DNS::Utility::PacketBuffer::GetAt(uint16_t index)
{
   if (index >= 512)
   {
      return { false,
               new Error({ DNS_PACKETBUFFER_OVERFLOW,
                           "Packet buffer overflow (512)" }) };
   }

   return m_Buffer[index];
}

Result<std::vector<uint8_t>>
DNS::Utility::PacketBuffer::GetRange(uint16_t start, uint16_t len)
{
   if (start + len >= 512)
   {
      return { {},
               new Error({ DNS_PACKETBUFFER_OVERFLOW,
                           "Packet buffer overflow (512)" }) };
   }

   std::vector<uint8_t> result(len);
   for (uint16_t i = 0; i < len; i++)
      result[i] = m_Buffer[start + i];

   return result;
}

Result<uint8_t>
DNS::Utility::PacketBuffer::ReadAsU8()
{
   if (m_ReadOffset >= 512)
   {
      return { false,
               new Error({ DNS_PACKETBUFFER_OVERFLOW,
                           "Packet buffer overflow (512)" }) };
   }

   return m_Buffer[m_ReadOffset++];
}

Result<uint16_t>
DNS::Utility::PacketBuffer::ReadAsU16()
{
   const auto [b1, b1Err] = ReadAsU8();
   if (b1Err) return { 0, b1Err };

   const auto [b2, b2Err] = ReadAsU8();
   if (b2Err) return { 0, b2Err };

   uint16_t result = (b1 << 8) | (b2 << 0);

   return result;
}

Result<uint32_t>
DNS::Utility::PacketBuffer::ReadAsU32()
{
   const auto [b1, b1Err] = ReadAsU8();
   if (b1Err) return { 0, b1Err };

   const auto [b2, b2Err] = ReadAsU8();
   if (b2Err) return { 0, b2Err };

   const auto [b3, b3Err] = ReadAsU8();
   if (b3Err) return { 0, b3Err };

   const auto [b4, b4Err] = ReadAsU8();
   if (b4Err) return { 0, b4Err };

   uint32_t result = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4 << 0);

   return result;
}