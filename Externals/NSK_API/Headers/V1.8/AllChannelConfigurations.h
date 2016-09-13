/**
 * @file AllChannelConfigurations.h
 * This file defines the contents of all 6 channel configuration shift
 * registers.
 */
#ifndef AllChannelConfigurations_h_
#define AllChannelConfigurations_h_

#include "dll_import_export.h"
#include "ChannelConfiguration.h"
#include "NeuroseekerConstants.h"

#include <array>

class ShiftRegister;


class DLL_IMPORT_EXPORT AllChannelConfigurations
{
public:
  AllChannelConfigurations();
  ~AllChannelConfigurations();

  /**
   * This function compares all members of lhs to those of rhs.
   *
   * @param lhs : first all channel configurations for comparison
   * @param rhs : second all channel configurations for comparison
   *
   * @return true if all members are equal, false otherwise.
   */
  friend bool operator==(const AllChannelConfigurations & lhs,
                         const AllChannelConfigurations & rhs);

  /**
   * This function sets all channel configurations to their default values.
   */
  void reset();

  /**
   * This function translates the array of 6 chains from the shift registers to
   * the members of all 6 channel configurations.
   *
   * @param chains : the 6 chains to translate
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_SR_ERROR if Shift Register error,
   *         CHANNELCONFIG_ILLEGAL_CHAIN_VALUE if multiple exlusive bits 1
   */
  ChannelConfigErrorCode
    getAllChannelConfigsFromChains(std::array<ShiftRegister,
                                   NUMBER_OF_CHANNELCONFIGS> & chains);

  /**
   * This function combines all members of all 6 channel configurations to the 6
   * chains for the shift registers.
   *
   * @param chains : the resulting chains
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_SR_ERROR if Shift Register error
   */
  ChannelConfigErrorCode getChains(std::array<ShiftRegister,
                                   NUMBER_OF_CHANNELCONFIGS> & chains) const;

  /**
   * This function gets the avgL1 of the dmux ext of the given channel
   * configuration number.
   *
   * @param channelconfignumber : the number of the channelconfig of which the
   *                              avgL1 value is wanted (valid range: 0 to 5)
   * @param avgl1               : the avgL1 to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode getAvgL1(unsigned int channelconfignumber,
                                  bool & avgl1) const;

  /**
   * This function sets the avgL1 of the dmux ext of the given channel
   * configuration number.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the avgL1 value (valid range: 0 to 5)
   * @param avgl1               : the value of the avgl1 to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAvgL1(unsigned int channelconfignumber,
                                  bool avgl1);

  /**
   * This function gets the avgL2 of the dmux ext of the given channel
   * configuration number.
   *
   * @param channelconfignumber : the number of the channelconfig of which the
   *                              avgL2 value is wanted (valid range: 0 to 5)
   * @param avgl2               : the avgL2 to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode getAvgL2(unsigned int channelconfignumber,
                                  bool & avgl2) const;

  /**
   * This function sets the avgL2 of the dmux ext of the given channel
   * configuration number.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the avgL2 value (valid range: 0 to 5)
   * @param avgl2               : the value of the avgl2 to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAvgL2(unsigned int channelconfignumber,
                                  bool avgl2);

  /**
   * This function gets the bypassSc of the dmux ext of the given channel
   * configuration number.
   *
   * @param channelconfignumber : the number of the channelconfig of which the
   *                              bypassSc value is wanted (valid range: 0 to 5)
   * @param bypassSc            : the bypassSc to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode getBypassSc(unsigned int channelconfignumber,
                                     bool & bypassSc) const;

  /**
   * This function sets the bypassSc of the dmux ext of the given channel
   * configuration number.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the bypassSc value (valid range: 0 to 5)
   * @param bypassSc            : the value of the bypassSc to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setBypassSc(unsigned int channelconfignumber,
                                     bool bypassSc);

  /**
   * This function gets the refsel of the dmux of the given channel number.
   *
   * @param channelnumber : the channel number of which the refsel is wanted
   *                        (valid range: 0 to 1439)
   * @param refsel        : the reference selection to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode getRefSel(unsigned int channelnumber,
                                   RefSel & refsel) const;

  /**
   * This function sets the refsel of the dmux of the given channel number.
   *
   * @param channelnumber : the channel number of which to set the refsel
   *                        (valid range: 0 to 1439)
   * @param refsel        : the value of the reference selection to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode setRefSel(unsigned int channelnumber,
                                   RefSel refsel);

  /**
   * This function gets the dmuxsel of the dmux of the given channel number.
   *
   * @param channelnumber : the channel number of which the refsel is wanted
   *                        (valid range: 0 to 1439)
   * @param dmuxsel       : the dmux selection to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode getDmuxSel(unsigned int channelnumber,
                                    DmuxSel & dmuxsel) const;

  /**
   * This function sets the dmuxsel of the dmux of the given channel number.
   *
   * @param channelnumber : the channel number of which to set the dmuxsel
   *                        (valid range: 0 to 1439)
   * @param dmuxsel       : the value of the dmux selection to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode setDmuxSel(unsigned int channelnumber,
                                    DmuxSel dmuxsel);

  /**
   * This function gets the bw of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which the bw value is wanted
   *                        (valid range: 0 to 1439)
   * @param bw            : the bw to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode getBw(unsigned int channelnumber,
                               unsigned char & bw) const;

  /**
   * This function sets the bw of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which to set the bw
   *                        (valid range: 0 to 1439)
   * @param bw            : the value of the bw to set
   *                        (valid range: 0 to 3)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setBw(unsigned int channelnumber,
                               unsigned char bw);

  /**
   * This function gets the mode of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which the mode value is wanted
   *                        (valid range: 0 to 1439)
   * @param mode          : the mode to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode getMode(unsigned int channelnumber,
                                 bool & mode) const;

  /**
   * This function sets the mode of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which to set the mode
   *                        (valid range: 0 to 1439)
   * @param mode          : the value of the mode to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode setMode(unsigned int channelnumber, bool mode);

  /**
   * This function gets the gain of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which the gain value is wanted
   *                        (valid range: 0 to 1439)
   * @param gain          : the gain to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode getGain(unsigned int channelnumber,
                                 unsigned char & gain) const;

  /**
   * This function sets the gain of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which to set the gain
   *                        (valid range: 0 to 1439)
   * @param gain          : the value of the gain to set
   *                        (valid range: 0 to 7)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setGain(unsigned int channelnumber,
                                 unsigned char gain);

  /**
   * This function gets the stdb of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which the stdb value is wanted
   *                        (valid range: 0 to 1439)
   * @param stdb          : the stdb to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode getStdb(unsigned int channelnumber,
                                 bool  & stdb) const;

  /**
   * This function sets the stdb of the channel settings of the given channel
   * number.
   *
   * @param channelnumber : the channel number of which to set the stdb
   *                        (valid range: 0 to 1439)
   * @param stdb          : the value of the stdb to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_CHANNEL_OUT_OF_RANGE if channel number out of range
   */
  ChannelConfigErrorCode setStdb(unsigned int channelnumber,
                                 bool stdb);

  /**
   * This function gets the compP of the adc comp of the given adc number.
   *
   * @param adcnumber : the adc number of which the compP value is wanted
   *                    (valid range: 0 to 71)
   * @param compp     : the compP to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc number out of range
   */
  ChannelConfigErrorCode getCompP(unsigned int adcnumber,
                                  unsigned char & compp) const;

  /**
   * This function sets the compP of the adc comp of the given adc number.
   *
   * @param adcnumber : the adc number of which to set the compP value
   *                    (valid range: 0 to 71)
   * @param compp     : the value of the compP to set
                        (valid range: 0 to 63)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc number out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setCompP(unsigned int adcnumber,
                                  unsigned char compp);

  /**
   * This function sets the compP of the adc comp of all adcs.
   *
   * @param compp     : the value of the compP to set
   *                    (valid range: 0 to 63)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAllCompP(unsigned char compp);

  /**
   * This function gets the compN of the adc comp of the given adc number.
   *
   * @param adcnumber : the adc number of which the compN value is wanted
   *                    (valid range: 0 to 71)
   * @param compn     : the compN to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc number out of range
   */
  ChannelConfigErrorCode getCompN(unsigned int adcnumber,
                                  unsigned char & compn) const;

  /**
   * This function sets the compN of the adc comp of the given adc number.
   *
   * @param adcnumber : the adc number of which to set the compN value
   *                    (valid range: 0 to 71)
   * @param compn     : the value of the compN to set
                        (valid range: 0 to 63)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc number out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setCompN(unsigned int adcnumber,
                                  unsigned char compn);

  /**
   * This function sets the compN of the adc comp of all adcs.
   *
   * @param compn     : the value of the compN to set
   *                    (valid range: 0 to 63)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAllCompN(unsigned char compn);

  /**
   * This function gets the slope of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the number of the adc pair of which the slope value
   *                        is wanted (valid range: 0 to 35)
   * @param slope         : the slope to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number out of range
   */
  ChannelConfigErrorCode getSlope(unsigned int adcpairnumber,
                                  unsigned char & slope) const;

  /**
   * This function sets the slope of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the adc pair number of which to set the slope
   *                        (valid range: 0 to 35)
   * @param slope         : the value of the slope to set
   *                        (valid range: 0 to 7)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number is out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value is out of range
   */
  ChannelConfigErrorCode setSlope(unsigned int adcpairnumber,
                                  unsigned char slope);

  /**
   * This function gets the fine of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the number of the adc pair of which the fine value
   *                        is wanted (valid range: 0 to 35)
   * @param fine          : the fine to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number out of range
   */
  ChannelConfigErrorCode getFine(unsigned int adcpairnumber,
                                 unsigned char & fine) const;

  /**
   * This function sets the fine of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the adc pair number of which to set the fine
   *                        (valid range: 0 to 35)
   * @param fine          : the value of the fine to set
   *                        (valid range: 0 to 7)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number is out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value is out of range
   */
  ChannelConfigErrorCode setFine(unsigned int adcpairnumber,
                                 unsigned char fine);

  /**
   * This function gets the coarse of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the number of the adc pair of which the coarse value
   *                        is wanted (valid range: 0 to 35)
   * @param fine          : the coarse to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number out of range
   */
  ChannelConfigErrorCode getCoarse(unsigned int adcpairnumber,
                                   unsigned char & coarse) const;

  /**
   * This function sets the coarse of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the adc pair number of which to set the coarse
   *                        (valid range: 0 to 35)
   * @param coarse        : the value of the coarse to set
   *                        (valid range: 0 to 7)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number is out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value is out of range
   */
  ChannelConfigErrorCode setCoarse(unsigned int adcpairnumber,
                                   unsigned char coarse);

  /**
   * This function gets the cfix of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the number of the adc pair of which the cfix value
   *                        is wanted (valid range: 0 to 35)
   * @param cfix          : the cfix to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number out of range
   */
  ChannelConfigErrorCode getCfix(unsigned int adcpairnumber,
                                 unsigned char & cfix) const;

  /**
   * This function sets the cfix of the adc common of the given adc pair
   * number.
   *
   * @param adcpairnumber : the adc pair number of which to set the cfix
   *                        (valid range: 0 to 35)
   * @param cfix          : the value of the cfix to set
   *                        (valid range: 0 to 7)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adc pair number is out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value is out of range
   */
  ChannelConfigErrorCode setCfix(unsigned int adcpairnumber,
                                 unsigned char cfix);

  /**
   * This function sets the bw of the channel settings of all channels of the
   * given channel config number.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the bw value (valid range: 0 to 5)
   * @param bw                  : the value of the bw to set
   *                              (valid range: 0 to 3)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAllBwOfChannelConfig(unsigned int
                                                 channelconfignumber, unsigned
                                                 char bw);

  /**
   * This function sets the mode of the channel settings of all channels of the
   * given channel config number.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the bw value (valid range: 0 to 5)
   * @param mode                : the value of the mode to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAllModeOfChannelConfig(unsigned int
                                                   channelconfignumber, bool
                                                   mode);

  /**
   * This function sets the gain of the channel settings of all channels.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the bw value (valid range: 0 to 5)
   * @param gain                : the value of the gain to set
   *                              (valid range: 0 to 7)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAllGainOfChannelConfig(unsigned int
                                                   channelconfignumber, unsigned
                                                   char gain);

  /**
   * This function sets the stdb of the channel settings of all channels.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the bw value (valid range: 0 to 5)
   * @param stdb                : the value of the stdb to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAllStdbOfChannelConfig(unsigned int
                                                   channelconfignumber, bool
                                                   stdb);

  /**
   * This function sets the reference selection (refsel) of the dmux of all
   * channels, except for the reference channels.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the bw value (valid range: 0 to 5)
   * @param refsel              : the value of the reference selection to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAllRefSelOfChannelConfig(unsigned int
                                                     channelconfignumber, RefSel
                                                     refsel);

  /**
   * This function sets the dmuxsel of the dmux of all channels.
   *
   * @param channelconfignumber : the number of the channelconfig of which to
   *                              set the bw value (valid range: 0 to 5)
   * @param dmuxsel             : the value of the dmux selection to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_CHANNELCONFIG if channelconfignumber out of
   *         range
   */
  ChannelConfigErrorCode setAllDmuxSelOfChannelConfig(unsigned int
                                                      channelconfignumber,
                                                      DmuxSel dmuxsel);

  /**
   * This function sets the bw of the channel settings of all channels.
   *
   * @param bw : the value of the bw to set (valid range: 0 to 3)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAllBw(unsigned char bw);

  /**
   * This function sets the mode of the channel settings of all channels.
   *
   * @param mode : the value of the mode to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful
   */
  ChannelConfigErrorCode setAllMode(bool mode);

  /**
   * This function sets the gain of the channel settings of all channels.
   *
   * @param gain : the value of the gain to set
   *               (valid range: 0 to 7)
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAllGain(unsigned char gain);

  /**
   * This function sets the stdb of the channel settings of all channels.
   *
   * @param stdb : the value of the stdb to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful
   */
  ChannelConfigErrorCode setAllStdb(bool stdb);

  /**
   * This function sets the reference selection (refsel) of the dmux of all
   * channels, except for the reference channels.
   *
   * @param refsel : the value of the reference selection to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful
   */
  ChannelConfigErrorCode setAllRefSel(RefSel refsel);

  /**
   * This function sets the dmuxsel of the dmux of all channels.
   *
   * @param dmuxsel : the value of the dmux selection to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful
   */
  ChannelConfigErrorCode setAllDmuxSel(DmuxSel dmuxsel);

  /**
   * This function sets the slopecal (slope, fine and coarse) of the given adc
   * pair number.
   *
   * @param adcpairnumber : the number of the adc pair of which the slopecal
   *                        should be set (valid range: 0 to 35)
   * @param slope         : the value of the slope to set
   * @param fine          : the value of the fine to set
   * @param coarse        : the value of the coarse to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adcpairnumber out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setSlopeCal(unsigned int adcpairnumber, unsigned char
                                     slope, unsigned char fine, unsigned char
                                     coarse);

  /**
   * This function sets the slopecal (slope, fine and coarse) of all adc pairs.
   *
   * @param slope         : the value of the slope to set
   * @param fine          : the value of the fine to set
   * @param coarse        : the value of the coarse to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAllSlopeCal(unsigned char slope, unsigned char fine,
                                        unsigned char coarse);

  /**
   * This function sets the cfix of all adc pairs.
   *
   * @param cfix : the value of the cfix to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAllCfix(unsigned char cfix);

  /**
   * This function gets the AdcPair (slope, fine, coarse and cfix) of the given
   * adc pair.
   *
   * @param adcpairnumber : the adc pair number of which to get the adcpair
   *                        (valid range: 0 to 35)
   * @param adcpair : the values of slope, fine, coarse and cfix to return
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adcpairnumber out of range,
   */
  ChannelConfigErrorCode getAdcPair(unsigned int adcpairnumber,
                                    AdcPairCommon & adcpair) const;

  /**
   * This function sets the AdcPair (slope, fine, coarse and cfix) of the given
   * adc pair.
   *
   * @param adcpairnumber : the adc pair number of which to set the adcpair
   *                        (valid range: 0 to 35)
   * @param adcpair : the values of slope, fine, coarse and cfix to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adcpairnumber out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAdcPair(unsigned int adcpairnumber,
                                    const AdcPairCommon & adcpair);

  /**
   * This function gets the adcpairs (slope, fine, coarse and cfix) of all adc
   * pairs.
   *
   * @param adcpairs : the array of AdcPairCommon values of which to return the
   *                   slope, fine, coarse and cfix values
   *
   * @return CHANNELCONFIG_SUCCESS if successful
   */
  ChannelConfigErrorCode getAllAdcPairs(std::array<AdcPairCommon,
                                        NUMBER_OF_ADCPAIRS> & adcpairs) const;

  /**
   * This function sets the adcpairs (slope, fine, coarse and cfix) of all adc
   * pairs.
   *
   * @param adcpairs : the array of AdcPairCommon values of which to use the
   *                   slope, fine, coarse and cfix values
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAllAdcPairs(const std::array<AdcPairCommon,
                                        NUMBER_OF_ADCPAIRS> & adcpairs);

  /**
   * This function gets the AdcComp (compP and compN) of the given adc.
   *
   * @param adcnumber : the adc number of which to get the adccomp
   *                        (valid range: 0 to 71)
   * @param adccomp   : the values of compP and compN to get
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adcnumber out of range
   */
  ChannelConfigErrorCode getAdcComp(unsigned int adcnumber,
                                    AdcComp & adccomp) const;

  /**
   * This function sets the AdcComp (compP and compN) of the given adc.
   *
   * @param adcnumber : the adc number of which to get the adccomp
   *                        (valid range: 0 to 71)
   * @param adccomp   : the values of compP and compN to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adcnumber out of range
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAdcComp(unsigned int adcnumber,
                                    AdcComp & adccomp);

  /**
   * This function gets the adccomps (compP and compN) of all adcs.
   *
   * @param adccomps : the array of AdcComp values of which to return the
   *                   compP and compN values
   *
   * @return CHANNELCONFIG_SUCCESS if successful
   */
  ChannelConfigErrorCode getAllAdcComps(std::array<AdcComp,
                                        NUMBER_OF_ADCS> & adccomps) const;

  /**
   * This function sets the adccomps (compP and compN) of all adcs.
   *
   * @param adccomps : the array of AdcComp values of which to use the
   *                   compP and compN values
   *
   * @return CHANNELCONFIG_SUCCESS if successful
   */
  ChannelConfigErrorCode setAllAdcComps(std::array<AdcComp,
                                        NUMBER_OF_ADCS> & adccomps);

private:
  /**
   * This function returns the number of the channel configuration in which the
   * information of the given channel number is kept.
   *
   * @param channelnumber: the number of the channel to check
   *
   * @return number of the channel configuration
   */
  unsigned int getChannelConfigNumberOfChannel(unsigned int channelnumber)
    const;

  /**
   * This function returns the number of the channel configuration in which the
   * information of the given adc number is kept.
   *
   * @param adcnumber: the number of the adc to check
   *
   * @return number of the channel configuration
   */
  unsigned int getChannelConfigNumberOfAdc(unsigned int adcnumber) const;

  std::array<ChannelConfiguration, NUMBER_OF_CHANNELCONFIGS> channelConfigs_;
};

/**
 * This function compares all members of lhs to those of rhs.
 *
 * @param lhs : first all channel configurations for comparison
 * @param rhs : second all channel configurations for comparison
 *
 * @return false if all members are equal, true otherwise.
 */
bool operator!=(const AllChannelConfigurations & lhs,
                const AllChannelConfigurations & rhs);

#endif
