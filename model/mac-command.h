/*
 * Copyright (c) 2017 University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 */

#ifndef MAC_COMMAND_H
#define MAC_COMMAND_H

#include "ns3/buffer.h"
#include "ns3/nstime.h"
#include "ns3/object.h"

namespace ns3
{
namespace lorawan
{

/**
 * Enum for every possible command type
 */
enum MacCommandType
{
    INVALID,
    LINK_CHECK_REQ,
    LINK_CHECK_ANS,
    LINK_ADR_REQ,
    LINK_ADR_ANS,
    DUTY_CYCLE_REQ,
    DUTY_CYCLE_ANS,
    RX_PARAM_SETUP_REQ,
    RX_PARAM_SETUP_ANS,
    DEV_STATUS_REQ,
    DEV_STATUS_ANS,
    NEW_CHANNEL_REQ,
    NEW_CHANNEL_ANS,
    RX_TIMING_SETUP_REQ,
    RX_TIMING_SETUP_ANS,
    TX_PARAM_SETUP_REQ,
    TX_PARAM_SETUP_ANS,
    DL_CHANNEL_REQ,
    DL_CHANNEL_ANS
};

/**
 * \ingroup lorawan
 *
 * This base class is used to represent a general MAC command.
 *
 * Pure virtual methods that handle serialization, deserialization and other
 * common features are supposed to be defined in detail by child classes, based
 * on that MAC command's attributes and structure.
 */
class MacCommand : public Object
{
  public:
    /**
     *  Register this type.
     *  \return The object TypeId.
     */
    static TypeId GetTypeId();

    MacCommand();           //!< Default constructor
    ~MacCommand() override; //!< Destructor

    /**
     * Serialize the contents of this MAC command into a buffer, according to the
     * LoRaWAN standard.
     *
     * \param start A pointer to the buffer into which to serialize the command.
     */
    virtual void Serialize(Buffer::Iterator& start) const = 0;

    /**
     * Deserialize the buffer into a MAC command.
     *
     * \param start A pointer to the buffer that contains the serialized command.
     * \return The number of bytes that were consumed.
     */
    virtual uint8_t Deserialize(Buffer::Iterator& start) = 0;

    /**
     * Print the contents of this MAC command in human-readable format.
     *
     * \param os The std::ostream instance on which to print the MAC command.
     */
    virtual void Print(std::ostream& os) const = 0;

    /**
     * Get serialized length of this MAC command.
     *
     * \return The number of bytes the MAC command takes up.
     */
    virtual uint8_t GetSerializedSize() const;

    /**
     * Get the commandType of this MAC command.
     *
     * \return The type of MAC command this object represents.
     */
    virtual enum MacCommandType GetCommandType() const;

    /**
     * Get the CID that corresponds to a type of MAC command.
     *
     * \param commandType The type of MAC command.
     * \return The CID as a uint8_t type.
     */
    static uint8_t GetCIDFromMacCommand(enum MacCommandType commandType);

  protected:
    enum MacCommandType m_commandType; //!< The type of this command.
    uint8_t m_serializedSize;          //!< This MAC command's serialized size.
};

/**
 * \ingroup lorawan
 *
 * Implementation of the LinkCheckReq LoRaWAN MAC command.
 *
 * This command holds no variables, and just consists in the CID.
 */
class LinkCheckReq : public MacCommand
{
  public:
    LinkCheckReq();
    ~LinkCheckReq() override; //!< Destructor
    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;
};

/**
 * \ingroup lorawan
 *
 * Implementation of the LinkCheckAns LoRaWAN MAC command.
 *
 * This command contains the demodulation margin and the number of receiving
 * gateways of the packet containing the LinkCheckReq command.
 */
class LinkCheckAns : public MacCommand
{
  public:
    LinkCheckAns(); //!< Default constructor

    /**
     * Constructor with given fields.
     *
     * \param margin The demodulation margin to set.
     * \param gwCnt The gateway count to set.
     */
    LinkCheckAns(uint8_t margin, uint8_t gwCnt);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

    /**
     * Set the demodulation margin value.
     *
     * \param margin The demodulation margin to set.
     */
    void SetMargin(uint8_t margin);

    /**
     * Get the demodulation margin value.
     *
     * \return The demodulation margin value.
     */
    uint8_t GetMargin() const;

    /**
     * Set the gateway count value.
     *
     * \param gwCnt The count value to set.
     */
    void SetGwCnt(uint8_t gwCnt);

    /**
     * Get the gateway count value.
     *
     * \return The gateway count value.
     */
    uint8_t GetGwCnt() const;

    /**
     * Increment this MacCommand's gwCnt value.
     */
    void IncrementGwCnt();

  private:
    uint8_t m_margin; //!< This MAC command's demodulation margin value.
    uint8_t m_gwCnt;  //!< This MAC command's gateway count value.
};

/**
 * \ingroup lorawan
 *
 * Implementation of the LinkAdrReq LoRaWAN MAC command.
 *
 * With this command, the network server can request a device to change its
 * data rate, transmission power and the channel it uses for uplink
 * transmissions.
 */
class LinkAdrReq : public MacCommand
{
  public:
    LinkAdrReq();
    /**
     * Constructor with given fields.
     *
     * The parameters of this constructor are serializable fields of the MAC command specified by
     * the LoRaWAN protocol. They represent MAC and PHY layer configurations to be applied by the
     * receiving end device. See, [LoRaWAN® L2 1.0.4 Specification (TS001-1.0.4)] and [LoRaWAN®
     * Regional Parameters RP002-1.0.4] for more details on how the fields are used and which
     * physical values they stand for.
     *
     * \param dataRate The DataRate field to set.
     * \param txPower The TXPower field to set.
     * \param channelMask The ChMask field to set.
     * \param chMaskCntl The ChMaskCntl field to set.
     * \param nbRep The NbTrans field to set.
     */
    LinkAdrReq(uint8_t dataRate,
               uint8_t txPower,
               uint16_t channelMask,
               uint8_t chMaskCntl,
               uint8_t nbRep);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

    /**
     * Return the data rate prescribed by this MAC command.
     *
     * \return An unsigned 8-bit integer containing the data rate.
     */
    uint8_t GetDataRate();

    /**
     * Get the transmission power prescribed by this MAC command.
     *
     * The MAC layer is expected to translate this value to a certain power in
     * dBm when communicating it to the PHY, and the translation will vary based
     * on the region of the device.
     *
     * \return The TX power, encoded as an unsigned 8-bit integer.
     */
    uint8_t GetTxPower();

    /**
     * Get the list of enabled channels. This method takes the 16-bit channel mask
     * and translates it to a list of integers that can be more easily parsed.
     *
     * \return The list of enabled channels.
     */
    std::list<int> GetEnabledChannelsList();

    /**
     * Get the number of repetitions prescribed by this MAC command.
     *
     * \return The number of repetitions.
     */
    int GetRepetitions();

  private:
    uint8_t m_dataRate;     //!< The DataRate field, a serializable parameter for setting the
                            //!< spreading factor and bandwidth of end devices
    uint8_t m_txPower;      //!< The TXPower field, a serializable parameter for setting the
                            //!< transmission power of end devices
    uint16_t m_channelMask; //!< The ChMask field
    uint8_t m_chMaskCntl;   //!< The ChMaskCntl field
    uint8_t m_nbRep;        //!< The NbTrans field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the LinkAdrAns LoRaWAN MAC command.
 *
 * With this command, the end device acknowledges a LinkAdrReq.
 */
class LinkAdrAns : public MacCommand
{
  public:
    LinkAdrAns(); //!< Default constructor

    /**
     * Constructor with given fields.
     *
     * \param powerAck The PowerACK field to set.
     * \param dataRateAck The DataRateACK field to set.
     * \param channelMaskAck The ChannelMaskACK field to set.
     */
    LinkAdrAns(bool powerAck, bool dataRateAck, bool channelMaskAck);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

  private:
    bool m_powerAck;       //!< The PowerACK field
    bool m_dataRateAck;    //!< The DataRateACK field
    bool m_channelMaskAck; //!< The ChannelMaskACK field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the DutyCycleReq LoRaWAN MAC command.
 *
 * With this command, the network server can limit the maximum aggregated
 * transmit duty cycle of an end device. The aggregate duty cycle is computed
 * as the duty cycle among all sub bands.
 */
class DutyCycleReq : public MacCommand
{
  public:
    DutyCycleReq();
    /**
     * Constructor providing initialization of all parameters.
     *
     * \param dutyCycle The duty cycle as a 8-bit unsigned integer.
     */
    DutyCycleReq(uint8_t dutyCycle);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

    /**
     * Get the maximum duty cycle prescribed by this Mac command, in fraction form.
     *
     * \return The maximum duty cycle.
     */
    double GetMaximumAllowedDutyCycle() const;

  private:
    uint8_t m_maxDCycle; //!< The MaxDutyCycle field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the DutyCycleAns LoRaWAN MAC command.
 *
 * This command holds no variables, and just consists in the CID.
 */
class DutyCycleAns : public MacCommand
{
  public:
    DutyCycleAns(); //!< Default constructor

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;
};

/**
 * \ingroup lorawan
 *
 * Implementation of the RxParamSetupReq LoRaWAN MAC command.
 *
 * \todo The use of frequencies in Hz here will cause problems for users, as frequencies are in MHz
 * in the rest of the module code. IMO it would be better to have freqs in Hz as uint32_t
 */
class RxParamSetupReq : public MacCommand
{
  public:
    RxParamSetupReq();

    /**
     * Constructor providing initialization of all fields.
     *
     * \param rx1DrOffset The data rate offset to use for the first receive window.
     * \param rx2DataRate The data rate to use for the second receive window.
     * \param frequency The frequency in Hz to use for the second receive window.
     */
    RxParamSetupReq(uint8_t rx1DrOffset, uint8_t rx2DataRate, double frequency);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

    /**
     * Get this command's Rx1DrOffset parameter.
     *
     * \return The Rx1DrOffset parameter.
     */
    uint8_t GetRx1DrOffset();

    /**
     * Get this command's Rx2DataRate parameter.
     *
     * \return The Rx2DataRate parameter.
     */
    uint8_t GetRx2DataRate();

    /**
     * Get this command's frequency.
     *
     * \return The frequency parameter, in Hz.
     */
    double GetFrequency();

  private:
    uint8_t m_rx1DrOffset; //!< The RX1DROffset field
    uint8_t m_rx2DataRate; //!< The RX2DataRate field
    double m_frequency;    //!< The Frequency field, _in Hz_
};

/**
 * \ingroup lorawan
 *
 * Implementation of the RxParamSetupAns LoRaWAN MAC command.
 */
class RxParamSetupAns : public MacCommand
{
  public:
    RxParamSetupAns();
    /**
     * Constructor with initialization of all parameters.
     *
     * \param rx1DrOffsetAck Whether or not the offset was correctly set.
     * \param rx2DataRateAck Whether or not the second slot data rate was correctly set.
     * \param channelAck Whether or not the second slot frequency was correctly set.
     */
    RxParamSetupAns(bool rx1DrOffsetAck, bool rx2DataRateAck, bool channelAck);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

  private:
    bool m_rx1DrOffsetAck; //!< The RX1DROffsetACK field
    bool m_rx2DataRateAck; //!< The RX2DataRateACK field
    bool m_channelAck;     //!< The ChannelACK field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the DevStatusReq LoRaWAN MAC command.
 */
class DevStatusReq : public MacCommand
{
  public:
    DevStatusReq();

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;
};

/**
 * \ingroup lorawan
 *
 * Implementation of the DevStatusAns LoRaWAN MAC command.
 */
class DevStatusAns : public MacCommand
{
  public:
    DevStatusAns(); //!< Default constructor

    /**
     * Constructor with initialization of all parameters.
     *
     * \param battery The battery level in [0, 255].
     * \param margin The demodulation margin of the last received DevStatusReq packet.
     */
    DevStatusAns(uint8_t battery, uint8_t margin);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

    /**
     * Get the battery information contained in this MAC command.
     *
     * \return The battery level.
     */
    uint8_t GetBattery() const;

    /**
     * Get the demodulation margin contained in this MAC command.
     *
     * \return The margin.
     */
    uint8_t GetMargin() const;

  private:
    uint8_t m_battery; //!< The Battery field
    uint8_t m_margin;  //!< The RadioStatus field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the NewChannelReq LoRaWAN MAC command.
 *
 * \todo The use of frequencies in Hz here will cause problems for users, as frequencies are in
 * MHz in the rest of the module code. IMO it would be better to have freqs in Hz as uint32_t
 */
class NewChannelReq : public MacCommand
{
  public:
    NewChannelReq();

    /**
     * Constructor providing initialization of all parameters.
     *
     * \param chIndex The index of the channel this command wants to operate on.
     * \param frequency The new frequency for this channel in Hz.
     * \param minDataRate The minimum data rate allowed on this channel.
     * \param maxDataRate The maximum data rate allowed on this channel.
     */
    NewChannelReq(uint8_t chIndex, double frequency, uint8_t minDataRate, uint8_t maxDataRate);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

    /**
     * Get the the ChIndex field contained in this MAC command.
     *
     * \return The ChIndex field.
     */
    uint8_t GetChannelIndex() const;
    /**
     * Get the the Frequency field contained in this MAC command.
     *
     * \return The Frequency field in Hz.
     */
    double GetFrequency() const;
    /**
     * Get the the MinDR field contained in this MAC command.
     *
     * \return The MinDR field.
     */
    uint8_t GetMinDataRate() const;
    /**
     * Get the the MaxDR field contained in this MAC command.
     *
     * \return The MaxDR field.
     */
    uint8_t GetMaxDataRate() const;

  private:
    uint8_t m_chIndex;     //!< The ChIndex field
    double m_frequency;    //!< The Frequency field, in Hz
    uint8_t m_minDataRate; //!< The MinDR field
    uint8_t m_maxDataRate; //!< The MaxDR field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the NewChannelAns LoRaWAN MAC command.
 */
class NewChannelAns : public MacCommand
{
  public:
    NewChannelAns(); //!< Default constructor

    /**
     * Constructor providing initialization of all parameters.
     *
     * \param dataRateRangeOk Whether or not the requested data rate range was set
     * correctly.
     * \param channelFrequencyOk Whether or not the requested channel frequency
     * was set correctly.
     */
    NewChannelAns(bool dataRateRangeOk, bool channelFrequencyOk);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

  private:
    bool m_dataRateRangeOk;    //!< The Data-rate range ok field
    bool m_channelFrequencyOk; //!< The Channel frequency ok field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the RxTimingSetupReq LoRaWAN MAC command.
 */
class RxTimingSetupReq : public MacCommand
{
  public:
    RxTimingSetupReq();

    /**
     * Constructor providing initialization of all parameters.
     *
     * \param delay The delay encoded in this MAC command.
     */
    RxTimingSetupReq(uint8_t delay);

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

    /**
     * Get the first window delay as a Time instance.
     *
     * \return The delay.
     */
    Time GetDelay();

  private:
    uint8_t m_delay; //!< The Del field
};

/**
 * \ingroup lorawan
 *
 * Implementation of the RxTimingSetupAns LoRaWAN MAC command.
 *
 * This MAC command has an empty payload.
 */
class RxTimingSetupAns : public MacCommand
{
  public:
    RxTimingSetupAns();

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

  private:
};

/**
 * \ingroup lorawan
 *
 * Implementation of the TxParamSetupAns LoRaWAN MAC command.
 */
class TxParamSetupAns : public MacCommand
{
  public:
    TxParamSetupAns(); //!< Default constructor

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

  private:
};

/**
 * \ingroup lorawan
 *
 * Implementation of the TxParamSetupReq LoRaWAN MAC command.
 */
class TxParamSetupReq : public MacCommand
{
  public:
    TxParamSetupReq();

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

  private:
};

/**
 * \ingroup lorawan
 *
 * Implementation of the DlChannelAns LoRaWAN MAC command.
 */
class DlChannelAns : public MacCommand
{
  public:
    DlChannelAns(); //!< Default constructor

    void Serialize(Buffer::Iterator& start) const override;
    uint8_t Deserialize(Buffer::Iterator& start) override;
    void Print(std::ostream& os) const override;

  private:
};
} // namespace lorawan

} // namespace ns3
#endif /* DEVICE_STATUS_H */
