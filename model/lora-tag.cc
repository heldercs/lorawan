/*
 * Copyright (c) 2017 University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 */

#include "lora-tag.h"

#include "ns3/tag.h"
#include "ns3/uinteger.h"

namespace ns3
{
namespace lorawan
{

NS_OBJECT_ENSURE_REGISTERED(LoraTag);

TypeId
LoraTag::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::LoraTag").SetParent<Tag>().SetGroupName("lorawan").AddConstructor<LoraTag>();
    return tid;
}

TypeId
LoraTag::GetInstanceTypeId() const
{
    return GetTypeId();
}

LoraTag::LoraTag (uint8_t sf, uint8_t destroyedBy) :
  m_sf (sf),
  m_destroyedBy (destroyedBy),
  m_receivePower (0),
  m_dataRate (0),
  m_frequency (0),
  m_nodeId(0),
  m_numTx(0)
{
}

LoraTag::~LoraTag()
{
}

uint32_t
LoraTag::GetSerializedSize() const
{
    // Each datum about a spreading factor is 1 byte + receivePower (the size of a double) +
    // frequency (the size of a double)
    return 6 + 2 * sizeof(double);
}

void
LoraTag::Serialize(TagBuffer i) const
{
  i.WriteU8 (m_sf);
  i.WriteU8 (m_destroyedBy);
  i.WriteDouble (m_receivePower);
  i.WriteU8 (m_dataRate);
  i.WriteDouble (m_frequency);
  i.WriteU16 (m_nodeId);
  i.WriteU8 (m_numTx);
}

void
LoraTag::Deserialize(TagBuffer i)
{
  m_sf = i.ReadU8 ();
  m_destroyedBy = i.ReadU8 ();
  m_receivePower = i.ReadDouble ();
  m_dataRate = i.ReadU8 ();
  m_frequency = i.ReadDouble ();
  m_nodeId = i.ReadU16 ();
  m_numTx = i.ReadU8 ();
}

void
LoraTag::Print(std::ostream& os) const
{
    os << m_sf << " " << m_destroyedBy << " " << m_receivePower << " " << m_dataRate;
}

uint8_t
LoraTag::GetSpreadingFactor() const
{
    return m_sf;
}

uint8_t
LoraTag::GetDestroyedBy() const
{
    return m_destroyedBy;
}

double
LoraTag::GetReceivePower() const
{
    return m_receivePower;
}

void
LoraTag::SetDestroyedBy(uint8_t sf)
{
    m_destroyedBy = sf;
}

void
LoraTag::SetSpreadingFactor(uint8_t sf)
{
    m_sf = sf;
}

void
LoraTag::SetReceivePower(double receivePower)
{
    m_receivePower = receivePower;
}

void
LoraTag::SetFrequency(double frequency)
{
    m_frequency = frequency;
}

double
LoraTag::GetFrequency() const
{
    return m_frequency;
}

uint8_t
LoraTag::GetDataRate() const
{
    return m_dataRate;
}

void
LoraTag::SetDataRate(uint8_t dataRate)
{
    m_dataRate = dataRate;
}

uint16_t
LoraTag::GetNodeId (void)
{
  return m_nodeId;
}

void
LoraTag::SetNodeId (uint16_t nodeId)
{
  m_nodeId = nodeId;
}

uint8_t
LoraTag::GetNumTx (void)
{
  return m_numTx;
}

void
LoraTag::SetNumTx (uint8_t numTx)
{
  m_numTx = numTx;
}

}
} // namespce ns3
