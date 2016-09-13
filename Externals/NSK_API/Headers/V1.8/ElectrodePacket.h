/**
 * @file ElectrodePacket.h
 * This file describes a complete electrode packet,
 * containing for each channel 1 data value.
 */
#ifndef ElectrodePacket_h_
#define ElectrodePacket_h_

#include "dll_import_export.h"
#include "NeuroseekerAPI.h"

class DLL_IMPORT_EXPORT ElectrodePacket
{
public:
  ElectrodePacket();
  ~ElectrodePacket();

  /**
   * This function reads the data chain (expected length is the electrode packet
   * size) and puts the resulting counter and adc values in the corresponding
   * members.
   *
   * @param datachain : the chain of data to process,
   *                    length = bytesize
   *
   * @return DATA_ERROR if a data error occurs (error in status bits 11 or 00),
   *         READ_SUCCESS if successful
   */
  ReadErrorCode getElectrodePacketFromChain(char * datachain);

  /**
   * This function returns the synchronization value of this Electrode packet.
   *
   * @return synchronization : the synchronization value of this packet
   */
  unsigned short getSynchronization() const;

  /**
   * This function returns the counter value of the given counterindex of this
   * Electrode Packet.
   *
   * @param counterindex : the index of the countervalue to return
   *                       (expected range: 0 to 19)
   *
   * @return countervalue : the counter value of the given index,
   *                        or 0 if the counterindex is out of range
   */
  unsigned int getCounter(unsigned int counterindex) const;

  /**
   * This function returns the data value of the given channel number.
   *
   * @param channel : the number of the channel of which the data value should
   *                  be given (expected range: 0 to 1439)
   *
   * @return channelvalue : the data value of the given channel number,
   *                        or 0 if the channel number is out of range
   */
  float getChannelData(unsigned int channel) const;


  static const unsigned int bytesize = 4*501;

private:
  /**
   * The synchronization word.
   */
  unsigned short synchronization;

  /**
   * The 20 counters.
   */
  unsigned int counters[20];

  /**
   * The channels: each channel has 1 data value.
   */
  float channelData[1440];

};

#endif
