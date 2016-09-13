/**
 * @file GeneralConfiguration.h
 * This file defines the contents of the general configuration shift register.
 */
#ifndef GeneralConfiguration_h_
#define GeneralConfiguration_h_

#include "dll_import_export.h"
#include <array>

typedef std::array<bool, 13> Bias_pix_EN;

class ShiftRegister;

/**
 * General Configuration error code.
 */
enum GeneralConfigErrorCode
{
  GENERALCONFIG_SUCCESS     = 0, /**< successful */
  GENERALCONFIG_SR_ERROR    = 1, /**< error accessing shift register */
  GENERALCONFIG_RANGE_ERROR = 2  /**< index out of range */
};

class DLL_IMPORT_EXPORT GeneralConfiguration
{
public:
  GeneralConfiguration();
  ~GeneralConfiguration();

  /**
   * This function compares all members of lhs to those of rhs.
   *
   * @param lhs : first general configuration for comparison
   * @param rhs : second general configuration for comparison
   *
   * @return true if all members are equal, false otherwise.
   */
  friend bool operator==(const GeneralConfiguration & lhs,
                         const GeneralConfiguration & rhs);

  /**
   * This function sets all members to their default values.
   */
  void reset();

  /**
   * This function translates the chain from the shift register to the members
   * of the general configuration.
   *
   * @param chain : the chain to translate
   *
   * @return GENERALCONFIG_SUCCESS if successful,
   *         GENERALCONFIG_SR_ERROR if Shift Register error
   */
  GeneralConfigErrorCode getGeneralConfigFromChain(ShiftRegister & chain);

  /**
   * This function combines all members of this general configuration to the
   * chain for the shift register.
   *
   * @param chain : the resulting chain
   *
   * @return GENERALCONFIG_SUCCESS if successful,
   *         GENERALCONFIG_SR_ERROR if Shift Register error
   */
  GeneralConfigErrorCode getChain(ShiftRegister & chain) const;

  /**
   * This function gives the bit with the given index of the biasPixEn array.
   *
   * @param index: the index of the bit to return (valid range: 0 to 12)
   * @param bit  : the bit to return
   *
   * @return GENERALCONFIG_SUCCESS if successful
   *         GENERALCONFIG_RANGE_ERROR if index out of range
   */
  GeneralConfigErrorCode getBiasPixEnBit(unsigned int index, bool & bit) const;

  /**
   * This function sets the bit with the given index of the biasPixEn array.
   *
   * @param index: the index of the bit to set (valid range: 0 to 12)
   * @param bit  : the bit to set
   *
   * @return GENERALCONFIG_SUCCESS if successful
   *         GENERALCONFIG_RANGE_ERROR if index out of range
   */
  GeneralConfigErrorCode setBiasPixEnBit(unsigned int index, bool bit);

  /**
   * This function gives the freqGlobal bit.
   *
   * @param bit  : the bit to return
   *
   * @return GENERALCONFIG_SUCCESS if successful
   */
  GeneralConfigErrorCode getTestInputEnBit(bool & bit) const;

  /**
   * This function sets the freqGlobal bit.
   *
   * @param bit  : the bit to set
   *
   * @return GENERALCONFIG_SUCCESS if successful
   */
  GeneralConfigErrorCode setTestInputEnBit(bool bit);


  static const unsigned int bitsize = 53;


  bool selPixBias;       // Select which cascode to use for pixel
  bool sarCal;           // ADC Calibration
  bool sarCalInt;        // ADC Calibration
  bool disconnectIn;     // Disconnect some IA inputs
  bool rOn;              // Safety version of integrator
  bool cOn;              // Normal version of integrator
  bool ocEn;             // Pixel over current protection
  bool freqGlobal;       // Activate Frequency/Calibration globally
  bool impEnGlobal;      // Activate Impedance measurement globally
  bool pX2;              // Speed up integrator
  bool nrstBg;           // Reset bandgap
  bool selBias;          // Select int/ext bias

private:
  Bias_pix_EN biasPixEn_; // Turn on bias in shank, 12+1 regions

};

/**
 * This function compares all members of lhs to those of rhs.
 *
 * @param lhs : first general configuration for comparison
 * @param rhs : second general configuration for comparison
 *
 * @return false if all members are equal, true otherwise.
 */
bool operator!=(const GeneralConfiguration & lhs,
                const GeneralConfiguration & rhs);

#endif
