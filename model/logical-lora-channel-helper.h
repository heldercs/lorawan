/*
 * Copyright (c) 2017 University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 */

#ifndef LOGICAL_LORA_CHANNEL_HELPER_H
#define LOGICAL_LORA_CHANNEL_HELPER_H

#include "logical-lora-channel.h"
#include "sub-band.h"

#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/packet.h"

#include <iterator>
#include <list>
#include <vector>

namespace ns3
{
namespace lorawan
{

/**
 * \ingroup lorawan
 *
 * This class supports LorawanMac instances by managing a list of the logical
 * channels that the device is supposed to be using, and establishes their
 * relationship with SubBands.
 *
 * This class also takes into account duty cycle limitations, by updating a list
 * of SubBand objects and providing methods to query whether transmission on a
 * set channel is admissible or not.
 */
class LogicalLoraChannelHelper : public Object
{
  public:
    /**
     *  Register this type.
     *  \return The object TypeId.
     */
    static TypeId GetTypeId();

    LogicalLoraChannelHelper();           //!< Default constructor
    ~LogicalLoraChannelHelper() override; //!< Destructor

    /**
     * Get the time it is necessary to wait before transmitting again, according
     * to the aggregate duty cycle timer.
     *
     * \return The aggregate waiting time.
     */
    Time GetAggregatedWaitingTime();

    /**
     * Get the time it is necessary to wait for before transmitting on a given
     * channel.
     *
     * \remark This function does not take into account aggregate waiting time.
     * Check on this should be performed before calling this function.
     *
     * \param channel A pointer to the channel we want to know the waiting time.
     * for.
     * \return A Time instance containing the waiting time before transmission is.
     * allowed on the channel.
     */
    Time GetWaitingTime(Ptr<LogicalLoraChannel> channel);

    /**
     * Register the transmission of a packet.
     *
     * \param duration The duration of the transmission event.
     * \param channel The channel the transmission was made on.
     */
    void AddEvent(Time duration, Ptr<LogicalLoraChannel> channel);

    /**
     * Get the list of LogicalLoraChannels currently registered on this helper.
     *
     * \return A list of the managed channels.
     */
    std::vector<Ptr<LogicalLoraChannel>> GetChannelList();

    /**
     * Get the list of LogicalLoraChannels currently registered on this helper
     * that have been enabled for Uplink transmission with the channel mask.
     *
     * \return A list of the managed channels enabled for Uplink transmission.
     */
    std::vector<Ptr<LogicalLoraChannel>> GetEnabledChannelList();

    /**
     * Add a new channel to the list.
     *
     * \param frequency The frequency of the channel to create.
     */
    void AddChannel(double frequency);

    /**
     * Add a new channel to the list.
     *
     * \param logicalChannel A pointer to the channel to add to the list.
     */
    void AddChannel(Ptr<LogicalLoraChannel> logicalChannel);

    /**
     * Set a new channel at a fixed index.
     *
     * \param chIndex The index of the channel to substitute.
     * \param logicalChannel A pointer to the channel to add to the list.
     */
    void SetChannel(uint8_t chIndex, Ptr<LogicalLoraChannel> logicalChannel);

    /**
     * Add a new SubBand to this helper.
     *
     * \param firstFrequency The first frequency of the subband, in MHz.
     * \param lastFrequency The last frequency of the subband, in MHz.
     * \param dutyCycle The duty cycle that needs to be enforced on this subband.
     * \param maxTxPowerDbm The maximum transmission power [dBm] that can be used.
     * on this SubBand.
     */
    void AddSubBand(double firstFrequency,
                    double lastFrequency,
                    double dutyCycle,
                    double maxTxPowerDbm);

    /**
     * Add a new SubBand.
     *
     * \param subBand A pointer to the SubBand that needs to be added.
     */
    void AddSubBand(Ptr<SubBand> subBand);

    /**
     * Remove a channel.
     *
     * \param channel A pointer to the channel we want to remove.
     */
    void RemoveChannel(Ptr<LogicalLoraChannel> channel);

    /**
     * Returns the maximum transmission power [dBm] that is allowed on a channel.
     *
     * \param logicalChannel The power for which to check the maximum allowed.
     * transmission power.
     * \return The power in dBm.
     */
    double GetTxPowerForChannel(Ptr<LogicalLoraChannel> logicalChannel);

    /**
     * Get the SubBand a channel belongs to.
     *
     * \param channel The channel whose SubBand we want to get.
     * \return The SubBand the channel belongs to.
     */
    Ptr<SubBand> GetSubBandFromChannel(Ptr<LogicalLoraChannel> channel);

    /**
     * Get the SubBand a frequency belongs to.
     *
     * \param frequency The frequency we want to check.
     * \return The SubBand the frequency belongs to.
     */
    Ptr<SubBand> GetSubBandFromFrequency(double frequency);

    /**
     * Disable the channel at a specified index.
     *
     * \param index The index of the channel to disable.
     */
    void DisableChannel(int index);

  private:
    /**
     * A list of the SubBands that are currently registered within this helper.
     */
    std::list<Ptr<SubBand>> m_subBandList;

    /**
     * A vector of the LogicalLoraChannels that are currently registered within
     * this helper. This vector represents the node's channel mask. The first N
     * channels are the default ones for a fixed region.
     */
    std::vector<Ptr<LogicalLoraChannel>> m_channelList;

    Time m_nextAggregatedTransmissionTime; //!< The next time at which
    //! transmission will be possible
    //! according to the aggregated
    //! transmission timer

    double m_aggregatedDutyCycle; //!< The next time at which
                                  //! transmission will be possible
    //! according to the aggregated
    //! transmission timer
};
} // namespace lorawan

} // namespace ns3
#endif /* LOGICAL_LORA_CHANNEL_HELPER_H */
