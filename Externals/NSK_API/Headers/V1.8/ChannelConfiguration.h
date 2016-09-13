/**
 * @file ChannelConfiguration.h
 * This file defines the contents of a channel configuration shift register.
 */
#ifndef ChannelConfiguration_h_
#define ChannelConfiguration_h_

#include <array>

class ShiftRegister;

/**
 * Channel Configuration error code.
 */
enum ChannelConfigErrorCode
{
  CHANNELCONFIG_SUCCESS              = 0, /**< successful */
  CHANNELCONFIG_SR_ERROR             = 1, /**< error accessing shift register */
  CHANNELCONFIG_CHANNEL_OUT_OF_RANGE = 2, /**< channel number out of valid range */
  CHANNELCONFIG_ADC_OUT_OF_RANGE     = 3, /**< adc number out of valid range */
  CHANNELCONFIG_ILLEGAL_WRITE_VALUE  = 4, /**< write value out of valid range */
  CHANNELCONFIG_ILLEGAL_CHANNELCONFIG = 5, /**< illegal channel config number */
  CHANNELCONFIG_ILLEGAL_CHAIN_VALUE   = 6  /**< illegal value read from chain:
                                             multiple exclusive bits 1 */
};

/**
 * DMUX_EXT.
 */
struct DmuxExt
{
  bool avgL1;    /**< Connect L1 to averaging ref */
  bool avgL2;    /**< Connect L2 to averaging ref */
  bool bypassSc; /**< Bypass the Switch cap filter to make channels full band */
};

/**
 * Reference Selection values.
 */
enum RefSel
{
  REF_SEL_NONE = 0, /**< No reference selected */
  REF_SEL_EXT  = 1, /**< External reference selected */
  REF_SEL_L1   = 2, /**< Reference selection local 1 */
  REF_SEL_L2   = 3  /**< Reference selection local 2 */
};

/**
 * Dmux selection values.
 */
enum DmuxSel
{
  DMUX_NONE = 0, /**< No dmux value selected */
  IMP_DMUX  = 1, /**< Impedance mode selected */
  RES_DMUX  = 2  /**< Frequency response selected */
};

/**
 * DMUX.
 */
struct Dmux
{
  RefSel  refSel;  /**< Reference selection */
  DmuxSel dmuxSel; /**< Dmux value selection */
};

/**
 * Channel Settings.
 */
struct ChannelSettings
{
  unsigned char bw;   /**< high pass corner frequency: 2 bits */
  bool mode;          /**< high pass (AP) when true, low pass (LFP) otherwise */
  unsigned char gain; /**< gain: 3 bits */
  bool stdb;          /**< set channel in standby when true */
};

/**
 * ADC calibration: comparator offsets (separate for all ADCs).
 */
struct AdcComp {
  unsigned char compP; /**< Comparator offset P: 5 bits */
  unsigned char compN; /**< Comparator offset N: 5 bits */
};

/**
 * ADC calibration: subregisters that are common for 2 ADCs.
 */
struct AdcPairCommon {
  unsigned char slope;  /**< Transfer curve slope  correction: 3 bits */
  unsigned char fine;   /**< Transfer curve fine   correction: 2 bits */
  unsigned char coarse; /**< Transfer curve coarse correction: 2 bits */
  unsigned char cfix;   /**< Transfer curve offset correction: 4 bits */
};


class ChannelConfiguration
{
public:
  ChannelConfiguration();
  ChannelConfiguration(const ChannelConfiguration & rhs);
  ChannelConfiguration(bool hasExternalReferences);
  ~ChannelConfiguration();

  const ChannelConfiguration & operator=(const ChannelConfiguration & rhs);

  /**
   * This function compares all members of lhs to those of rhs.
   *
   * @param lhs : first channel configuration for comparison
   * @param rhs : second channel configuration for comparison
   *
   * @return true if all members are equal, false otherwise.
   */
  friend bool operator==(const ChannelConfiguration & lhs,
                         const ChannelConfiguration & rhs);

  /**
   * This function sets all members to their default values.
   */
  void reset();

  /**
   * This function translates the chain from the shift register to the members
   * of the channel configuration.
   *
   * @param chain : the chain to translate
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_SR_ERROR if Shift Register error,
   *         CHANNELCONFIG_ILLEGAL_CHAIN_VALUE if multiple exlusive bits 1
   */
  ChannelConfigErrorCode getChannelConfigFromChain(ShiftRegister & chain);

  /**
   * This function combines all members of this channel configuration to the
   * chain for the shift register.
   *
   * @param chain : the resulting chain
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_SR_ERROR if Shift Register error
   */
  ChannelConfigErrorCode getChain(ShiftRegister & chain) const;

  /**
   * This function gets the refsel of the dmux of the given channel number.
   *
   * @param channelnumber : the channel number of which the refsel is wanted
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                        (valid range: 0 to 239)
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
   *                    (valid range: 0 to 11)
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
   *                    (valid range: 0 to 11)
   * @param compp     : the value of the compP to set
   *                    (valid range: 0 to 63)
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
   *                    (valid range: 0 to 11)
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
   *                    (valid range: 0 to 11)
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
   *                        is wanted (valid range: 0 to 5)
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
   *                        (valid range: 0 to 5)
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
   *                        is wanted (valid range: 0 to 5)
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
   *                        (valid range: 0 to 5)
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
   *                        is wanted (valid range: 0 to 5)
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
   *                        (valid range: 0 to 5)
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
   *                        is wanted (valid range: 0 to 5)
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
   *                        (valid range: 0 to 5)
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
   *                        should be set (valid range: 0 to 5)
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
   *                        (valid range: 0 to 5)
   * @param adcpair : the values of slope, fine, coarse and cfix to get
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
   *                        (valid range: 0 to 5)
   * @param adcpair : the values of slope, fine, coarse and cfix to set
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adcpairnumber out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAdcPair(unsigned int adcpairnumber,
                                    const AdcPairCommon & adcpair);

  /**
   * This function gets the AdcComp (compP and compN) of the given adc.
   *
   * @param adcnumber : the adc number of which to get the adcpair
   *                        (valid range: 0 to 11)
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
   * @param adcnumber : the adc number of which to get the adcpair
   *                        (valid range: 0 to 11)
   * @param adccomp   : the values of compP and compN to get
   *
   * @return CHANNELCONFIG_SUCCESS if successful,
   *         CHANNELCONFIG_ADC_OUT_OF_RANGE if adcnumber out of range,
   *         CHANNELCONFIG_ILLEGAL_WRITE_VALUE if write value out of range
   */
  ChannelConfigErrorCode setAdcComp(unsigned int adcnumber,
                                    AdcComp & adccomp);


  static const unsigned int bitsize = 3106;


  DmuxExt dmuxExt;

private:
  /**
   * This function checks whether the given channel number is a reference
   * channel or not.
   *
   * @param channelNumber: the channel number to check
   *
   * @return true if the channel is a reference channel, false otherwise
   */
  bool isReferenceChannel(unsigned int channelNumber);

  std::array<Dmux, 240> dmux_;
  std::array<ChannelSettings, 240> channelSettings_;
  std::array<AdcComp, 12> adcComp_;
  std::array<AdcPairCommon, 6> adcCommon_;

  bool hasExternalReferences_;

};

bool operator==(const DmuxExt & lhs, const DmuxExt & rhs);
bool operator==(const Dmux & lhs, const Dmux & rhs);
bool operator==(const ChannelSettings & lhs, const ChannelSettings & rhs);
bool operator==(const AdcComp & lhs, const AdcComp & rhs);
bool operator==(const AdcPairCommon & lhs, const AdcPairCommon & rhs);

/**
 * This function compares all members of lhs to those of rhs.
 *
 * @param lhs : first channel configuration for comparison
 * @param rhs : second channel configuration for comparison
 *
 * @return false if all members are equal, true otherwise.
 */
bool operator!=(const ChannelConfiguration & lhs,
                const ChannelConfiguration & rhs);

#endif
