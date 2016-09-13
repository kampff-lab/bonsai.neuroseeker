/**
 * @file NeuroseekerAPI.h
 * This file defines the api of this project.
 */
#ifndef NeuroseekerAPI_h_
#define NeuroseekerAPI_h_

#include "dll_import_export.h"
#include "NeuroseekerConstants.h"
#include "GeneralConfiguration.h"
#include "AllChannelConfigurations.h"

#include <fstream>
#include <vector>
#include <array>
#include <sstream>

class neuroseeker;
class VciInterface;
class NeuroseekerDataLinkIntf;
class AdcPacket;
class ElectrodePacket;
class ShiftRegister;
class field_command;

/**
 * Version number of the api / fpga.
 */
struct VersionNumber {
  unsigned short major; /**< should be compatible for hardware and software */
  unsigned short minor; /**< indicates small updates that did not affect other
                          side */
};

/**
 * Asic mode
 */
enum AsicMode
{
  ASIC_CONFIGURATION = false, /**< asic configuration mode */
  ASIC_RECORDING     = true   /**< asic recording mode */
};

/**
 * General error code.
 */
enum ErrorCode
{
  SUCCESS = 0, /**< successful */
  FAILURE = 1  /**< an error was detected */
};

/**
 * Error code for open.
 */
enum OpenErrorCode
{
  OPEN_SUCCESS                 = 0, /**< device opened successfully */
  DATA_LINK_FAILED             = 1, /**< error while opening data link */
  CONFIG_LINK_FAILED           = 2, /**< error while opening config link */
  DEVICE_ALREADY_OPEN          = 3, /**< device was already open */
  WRONG_HW_SW_VERSION          = 4, /**< incompatible hw and sw version */
  CONFIG_DESER_FAILED          = 5, /**< error while configuring deserializer */
  CONFIG_SER_FAILED            = 6, /**< error while configuring serializer */
  CONFIG_HS_ND_ENABLE_FAILED   = 7, /**< error while configuring headstage
                                      ND_ENABLE */
  CONFIG_HS_TE_FAILED          = 8, /**< error while configuring headstage TE */
  CONFIG_HS_PRNRST_FAILED      = 9, /**< error while configuring headstage
                                      PR_NRST */
  CONFIG_HS_NRST_FAILED        = 10, /**< error while configuring headstage
                                       NRST */
  CONFIG_HS_MODE_FAILED        = 11, /**< error while configuring headstage
                                       MODE */
  CONFIG_GENERAL_REG_FAILED    = 12, /**< error while writing general shift
                                       register */
  CONFIG_CHANNEL_REGS_FAILED   = 13, /**< error while writing all channel shift
                                       registers */
  CONFIG_CALIB_FAILED          = 14, /**< error while configuring members for
                                       calibration */
  CONFIG_HS_REG_FAILED         = 15, /**< error while configuring headstage
                                       registers */
  CONFIG_DATAMODE_FAILED       = 16  /**< error while setting the datamode */
};

/**
 * Error code for accessing the UART.
 */
enum UartErrorCode
{
  UART_SUCCESS           = 0, /**< data successfully read or written */
  UART_NO_CONFIG_LINK    = 1, /**< no configuration link existing */
  UART_TIMEOUT           = 2, /**< uart timeout */
  UART_ACK_ERROR         = 3, /**< error in received acknowledge byte */
  UART_RX_OVERFLOW_ERROR = 4, /**< uart error: rx overflow */
  UART_FRAME_ERROR       = 5, /**< uart protocol error: wrong rx stopbit */
  UART_PARITY_ERROR      = 6, /**< uart error: rx parity wrong */
  UART_UNDERFLOW_ERROR   = 7  /**< reading the command_readdata failed: fifo
                                empty */
};

/**
 * Error code for accessing the config parameters of the basestation fpga.
 */
enum ConfigAccessErrorCode
{
  CONFIG_SUCCESS          = 0, /**< config access successful */
  CONFIG_ERROR_NO_LINK    = 1, /**< no config link existing  */
  CONFIG_WRITE_VAL_ERROR  = 2, /**< value to write (or its size) out of range */
  CONFIG_READ_VAL_ERROR   = 3  /**< value read out of range */
};

/**
 * Error code for accessing the digital control signals.
 */
enum DigitalControlErrorCode
{
  DIGCTRL_SUCCESS                = 0, /**< digital control access successful*/
  DIGCTRL_READVAL_ERROR          = 1, /**< uart error while dig ctrl read */
  DIGCTRL_WRITEVAL_ERROR         = 2  /**< uart error while dig ctrl write */
};

/**
 * Error code for reading data.
 */
enum ReadErrorCode
{
  READ_SUCCESS           = 0, /**< data successfully read */
  READ_NO_DATA_LINK      = 1, /**< data link not connected */
  READ_NO_CONFIG_LINK    = 2, /**< config link not connected */
  READ_CONFIG_ERROR      = 3, /**< config error while configuring parameters of
                                basestation for reading */
  READ_RECORDING_ERROR   = 4, /**< error while start of stop recording */
  READ_DATA_SIZE         = 5, /**< error reading too large amount of data */
  READ_LINK_ERROR        = 6, /**< fd timeout or playback file empty */

  WRONG_DATA_MODE        = 7, /**< incorrect data mode configured */
  DATA_ERROR             = 8  /**< lfp_ctr doesn't match, or header, ...,
                                 The most probable cause is DRAM FIFO overflow*/
};

/**
 * selection argument DAC for generateSine and generateDC
 */
enum DacSel
{
  DAC_A = 0, /**< HeadStage DAC_A select */
  DAC_B = 1, /**< HeadStage DAC_B select */
  DAC_C = 2, /**< HeadStage DAC_C select */
  DAC_D = 3  /**< HeadStage DAC_D select */
};

/**
 * Error code for accessing the DAC control signals.
 */
enum DacControlErrorCode
{
  DACCTRL_SUCCESS                = 0, /**< DAC control registers access successful*/
  DACCTRL_READVAL_ERROR          = 1, /**< uart error while DAC ctrl read */
  DACCTRL_WRITEVAL_ERROR         = 2, /**< uart error while DAC ctrl write */
  DACCTRL_WRITE_VAL_OUT_OF_RANGE = 3  /**< value to write is unvalid */
};

/**
 * Error code for BIST Test 4
 */
enum Bist4ErrorCode
{
  BIST4_SUCCESS        = 0, /**< BIST Test 4 successful */
  BIST4_NO_DEVICE      = 1, /**< BIST Test 4 no device opened */
  BIST4_LED_ERROR      = 2, /**< BIST Test 4 register read/write failure */
  BIST4_DRAM_ERROR     = 3, /**< BIST Test 4 DRAM selftest failure */
  BIST4_LOOPBACK_ERROR = 4  /**< BIST Test 4 data link loopback failure */
};

/**
 * Error code for BIST Test 6
 */
enum Bist6ErrorCode
{
   BIST6_SUCCESS     = 0, /**< BIST Test 6 (started) successful */
   BIST6_NO_DEVICE   = 1, /**< BIST Test 6 no device opened */
   BIST6_UART_ERROR  = 2, /**< BIST Test 6 UART communication error */
   BIST6_SER_ERROR   = 3, /**< BIST Test 6 serializer status at address 0x04
                                does not equal 0x87 */
   BIST6_DESER_ERROR = 4, /**< BIST Test 6 deserializer status at address
                                0x04 does not equal 0x87 */
   BIST6_PRBS_ERR    = 5  /**< BIST Test 6 PRBS_ERR not zero */
};

/**
 * Error code for BIST Test 5 & 7 & stop 9
 */
enum BistErrorCode
{
  BIST_SUCCESS    = 0, /**< BIST Test (started / stopped) successful */
  BIST_NO_DEVICE  = 1, /**< BIST Test no device opened */
  BIST_UART_ERROR = 2  /**< BIST Test UART communication error */
};

/**
 * Error code for BIST Test 8
 */
enum Bist8ErrorCode
{
  BIST8_SUCCESS     = 0, /**< BIST Test 8 started / stopped successful */
  BIST8_NO_DEVICE   = 1, /**< BIST Test 8 no device opened */
  BIST8_UART_ERROR  = 2, /**< BIST Test 8 UART communication error */
  BIST8_RANGE_ERR   = 3, /**< BIST Test 8 SPI line out of range */
  BIST8_DIGCTRL_ERR = 4  /**< BIST Test 8 digital control access failed */
};

/**
 * Error code for BIST Test 9
 */
enum Bist9ErrorCode
{
  BIST9_SUCCESS     = 0, /**< BIST Test 9 started / stopped successful */
  BIST9_NO_DEVICE   = 1, /**< BIST Test 9 no device opened */
  BIST9_DIGCTRL_ERR = 2  /**< BIST Test 9 digital control access failed */
};

/**
 * Error code for accessing the shift registers.
 */
enum ShiftRegisterAccessErrorCode
{
  SHIFTREG_SUCCESS            = 0, /**< access of shiftregister successful */
  SHIFTREG_UART_ERROR         = 1, /**< uart error */
  SHIFTREG_READBACK_ERROR     = 2, /**< readback value incorrect */
  SHIFTREG_CHAIN_MEMBER_ERROR = 3, /**< error converting chain to member or
                                     member to chain */
  SHIFTREG_CHAIN_ERROR        = 4  /**< error making shiftregister var */
};

/**
 * Error code for reading from a csv file.
 */
enum ReadCsvErrorCode
{
  READCSV_SUCCESS            = 0, /**< Reading the csv file was successful*/
  READCSV_FILE_ERR           = 1, /**< Error opening the filestream of the csv*/
  READCSV_NUMBER_OF_ELEMENTS = 2, /**< Invalid number of elements read*/
  READCSV_OUT_OF_RANGE       = 3, /**< Read element is out of range*/
  READCSV_SHIFTREG_CHAIN_MEMBER_ERROR = 4  /**< error converting chain to member
                                             or member to chain */
};

/**
 * Error code for calibration procedures.
 */
enum CalibErrorCode
{
  CALIB_SUCCESS                 = 0, /**< calibration procedure successful */
  CALIB_SETMODE_ERROR           = 1, /**< error while setting the mode */
  CALIB_NO_CONNECTION           = 2, /**< no data link connection */
  CALIB_SETCHANNELCONFIG_ERROR  = 3, /**< error while setting the channel
                                       configuration */
  CALIB_WRITESHIFT_ERROR        = 4, /**< error while writing shift register */
  CALIB_SETDATAMODE_ERROR       = 5, /**< error while setting the data mode */
  CALIB_DACCFG_ERROR            = 6, /**< error while setting the dacs */
  CALIB_READDATA_ERROR          = 7, /**< error while reading data */
  CALIB_RESETDATAPATH_ERROR     = 8  /**< error while resetting datapath */
};


class DLL_IMPORT_EXPORT NeuroseekerAPI
{
public:
  NeuroseekerAPI();
  virtual ~NeuroseekerAPI();

  /**
   * This function establishes a data connection and a config link connection
   * with the basestation FPGA. It initializes the basestation FPGA, the
   * High-Speed Link, and the Headstage.
   *
   * @return OPEN_SUCCESS if successful
   */
  OpenErrorCode open();

  /**
   * This function resets the ASIC.
   *
   * @return OPEN_SUCCESS if successful
   */
  OpenErrorCode init();

  /**
   * This function returns the fpga version number.
   *
   * @param version : the version number to return
   *
   * @return FAILURE if no config link connection, SUCCESS otherwise
   */
  ErrorCode getHardwareVersion(VersionNumber * version);

  /**
   * This function returns the api version number.
   *
   * @return version number
   */
  const struct VersionNumber getAPIVersion();

  /**
   * This function writes the given string to the lcd of the basestation fpga.
   *
   * @param line0 : the data for the first line (must be 16 characters)
   * @param line1 : the data for the second line (must be 16 characters)
   *
   * @return CONFIG_ERROR_NO_LINK if no configuration link existing,
   *         CONFIG_WRITE_VAL_ERROR if length of line1 or line2 is not 16,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode writeLCD(std::string line0 = "  Hello World   ",
                                 std::string line1 = "                ");

  /**
   * This function configures the serializer (to be done once after startup)
   *
   * @return SUCCESS if sucessful
   */
  ErrorCode configureSerializer();

  /**
   * This function configures the deserializer (to be done once after startup)
   *
   * @return SUCCESS if sucessful
   */
  ErrorCode configureDeserializer();

  /**
   * This function reads a register of a device.
   *
   * @param device  : device address
   * @param address : register address
   * @param data    : read register value
   *
   * @return error of UartErrorCode
   */
  UartErrorCode readUart(unsigned char device,
                         unsigned char address,
                         unsigned char & data);

  /**
   * This function writes a register of a device.
   *
   * @param device  : device address
   * @param address : register address
   * @param data    : register value to write
   *
   * @return error of UartErrorCode
   */
  UartErrorCode writeUart(unsigned char device,
                          unsigned char address,
                          unsigned char data);

  /**
   * This function writes the ND_ENABLE field of the Headstage General Control
   * Register #1 of the headstage FPGA.
   *
   * @param ndenable : the desired value for ndenable
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode ndEnable(bool ndenable);

  /**
   * This function writes the TE field of the Headstage General Control
   * Register #1 of the headstage FPGA.
   *
   * @param te : the te value to write
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode te(bool te);

  /**
   * This function writes the ASIC_PR_RESET field of the Headstage General
   * Control Register #2 of the headstage FPGA.
   *
   * @param pr_nrst : the desired value for pr_nrst
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode prnrst(bool pr_nrst);

  /**
   * This function writes the ASIC_RESET field of the Headstage General Control
   * Register #2 of the headstage FPGA.
   *
   * @param nrst : the desired value for nrst
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode nrst(bool nrst);

  /**
   * This function writes the LED OFF field of the Headstage General
   * Control Register #2 of the headstage FPGA.
   *
   * @param led_off : the desired value for led_off
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode ledOff(bool led_off);

  /**
   * This function writes the ASIC MODE field of the Headstage General Control
   * Register #2 of the headstage FPGA.
   *
   * @param mode : the mode value to write (true for RECORDING,
   *               false for CONFIGURATION) or use AsicMode enum
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode mode(bool mode);

  /**
   * This function sets the data mode to the given value.
   *
   * @param mode: adc mode = false, electrode mode = true
   *
   * @return FAILURE when datamodel does not exist, SUCCESS otherwise
   */
  ErrorCode datamode(bool mode);

  /**
   * This function returns the current data mode.
   *
   * @return data mode: 0 is adc mode, 1 is electrode mode
   */
  bool getDatamode();

  /**
   * This function reads the data mode from the basestation.
   *
   * @param mode : the data mode
   *
   * @return NO_DATA_LINK when datamodel does not exist,
   *         READ_SUCCESS if succesful
   */
  ReadErrorCode readDatamodeFromBS(bool & mode);

  /**
   * This function writes the BIST_RX field of the Headstage General
   * Control Register #1 of the headstage FPGA.
   *
   * @param bist_rx : the desired value for bist_rx
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode bistRx(bool bist_rx);

  /**
   * This function writes the BIST_TX field of the Headstage General
   * Control Register #1 of the headstage FPGA.
   *
   * @param bist_tx : the desired value for bist_tx
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode bistTx(bool bist_tx);

  /**
   * This function writes the SW_HBEAT field of the Headstage General
   * Control Register #1 of the headstage FPGA.
   *
   * @param hs_sw_hbeat : the desired value for hs_sw_hbeat
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode hsSwHbeat(bool hs_sw_hbeat);

  /**
   * This function writes the HS_RESET field of the Headstage General
   * Control Register #2 of the headstage FPGA.
   *
   * @param hs_reset : the desired value for hs_reset
   *
   * @return DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode hsReset(bool hs_reset);

  /**
   * This function gets all digital control values.
   *
   * @param mode            : asic mode value
   * @param te              : te value
   * @param nrst            : value of nrst
   * @param pr_nrst         : value of pr_nrst
   * @param nd_enable       : nd enable value
   * @param sw_hbeat        : sw hbeat value
   * @param headstage_reset : value of the headstage reset
   * @param bist_rx         : value of bist_rx
   * @param bist_tx         : value of bist_tx
   * @param led_off         : value of led_off
   *
   * @return DIGCTRL_READVAL_ERROR when an error occurred while reading a
   * register, DIGCTRL_SUCCESS if successful
   */
  DigitalControlErrorCode getDigCtrl(unsigned char & mode,
                                     bool & te,
                                     bool & nrst,
                                     bool & pr_nrst,
                                     bool & nd_enable,
                                     bool & sw_hbeat,
                                     bool & headstage_reset,
                                     bool & bist_rx,
                                     bool & bist_tx,
                                     bool & led_off);

  /**
   * This function resets the datapath of the basestation fpga:
   * - first the data_generator, decoupling fifos, deserializer_intf, scale,
   *   reorder, dma "functional cores" are put in reset (their corresponding
   *   configuration registers are not reset)
   * - next the DRAM fifo is flushed
   * - then the reset from the first step is released
   *
   * @return SUCCESS if successful
   */
  ErrorCode resetDatapath();

  /**
   * This function checks whether or not the data and config link are connected.
   *
   * @return true if data and config link are connected, else false
   */
  bool isConnected();

  /**
   * This function configures the trigger mode.
   *
   * @param drive_sync_ext_start : if true, SYNC_EXT_START will be driven by the
   * FPGA
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode triggerMode(bool drive_sync_ext_start);

  /**
   * This function closes the data and config link connection with the device.
   */
  virtual void close();

  /**
   * reads the BaseStation onboard temperature sensor and convert to Celsius
   *
   * @param temperature : the temperature of the BaseStation FPGA
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
   ConfigAccessErrorCode readTemperature(float & temperature);

  /**
   * This function starts recording data. All data read from the
   * basestation fpga over the tcp/ip datalink will be recorded to file.
   *
   * @param filename : name of the file to which data will be written
   *                  (optional parameter, default value = "datalog.nsk")
   *
   * @return SUCCESS if successful, FAILURE is no data connection
   */
  ErrorCode startRecording(const std::string filename = "datalog.nsk");

  /**
   * This function stops recording data.
   *
   * @return SUCCESS if successful, FAILURE is no data connection
   */
  ErrorCode stopRecording();

  /**
   * This function sets the bypass datapath flag of the basestation FPGA.
   *
   * @param bypassDatapath : the value to write to the bypass datapath flag,
   *                         true means datapath bypassing is enabled (raw data
   *                         mode), false otherwise
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode setBypassDatapath(bool bypassDatapath);

  /**
   * This function checks whether or not the raw data mode is enabled in the
   * basestation fpga.
   *
   * @param bypassDatapath : false when the raw data mode is disabled, true when
   * the raw data mode is enabled
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getBypassDatapath(bool & bypassDatapath);

  /**
   * This function sets the Uart baudrate to the given value.
   *
   * @param nsk_not_npx: false = 487.5 kbauds, true = 425 kbauds
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode setUartBaudrate(bool nsk_not_npx);

  /**
   * This function checks the Uart baudrate in the basestation fpga.
   *
   * @param nsk_not_npx: false = 487.5, kbauds true = 425 kbauds
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_READ_VAL_ERROR if read data is out of range,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getUartBaudrate(bool & nsk_not_npx);

  /**
   * This function enables / disables Uart parity in the basestation fpga.
   *
   * @param enable : false = parity disabled, true = parity enabled
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode setUartParity(bool enable);

  /**
   * This function checks whether or not Uart parity is enabled in the
   * basestation fpga.
   *
   * @param enabled : false = parity disabled, true = parity enabled
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getUartParity(bool & enabled);

  /**
   * This function resets all FPGA modules except for cgen, t_nsk_reset,
   * vci_busconnect_nsk, tcp_router, hw_tcpip_vci and hw_tcp_ip_minimal
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode resetFPGA();

  /**
   * This function gets the version number of the basestation connect board.
   *
   * @param version_major : the BaseStation Connect board version number
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getBSVersion(unsigned char & version_major);

  /**
   * This function gets the revision number of the basestation connect board.
   *
   * @param version_minor : the BaseStation Connect board revision number
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getBSRevision(unsigned char & version_minor);

  /**
   * This function controls the power of the BaseStation Connect board.
   *
   * @param enable : power enable
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode supplyBS(bool enable);

  /**
   * This function checks whether or not the power of the BaseStation Connect
   * board is enabled.
   *
   * @param enable : power is enabled when true, disabled when false
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getSupplyBS(bool & enable);

  /**
   * This function enables / disables data zeroing in the basestation fpga.
   *
   * @param enable : false = data zeroing disabled, true = data zeroing enabled
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode setDataZeroing(bool enable);

  /**
   * This function checks whether or not data zeroing is enabled in the
   * basestation fpga.
   *
   * @param enabled : false when data zeroing is disabled, true when enabled
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getDataZeroing(bool & enabled);

  /**
   * This function enables / disables lock checking in the basestation fpga.
   *
   * @param enable : false = lock checking disabled, true = lock checking enabled
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode setLockChecking(bool enable);

  /**
   * This function checks whether or not lock checking is enabled in the
   * basestation fpga.
   *
   * @param enable : false when lock checking is disabled, true when enabled
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getLockChecking(bool & enabled);

  /**
   * This functions drives an orange LED on BS-Connect board
   *
   * @param enable : if true the orange LED with be on
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode setBSSwHbeat(bool enable);

  /**
   * This function checks whether or not the orange LED on the BS-Connect board
   * is on.
   *
   * @param enable : if true the orange LED with be on
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getBSSwHbeat(bool & enable);

  /**
   * This function starts logging information in api_log.txt.
   */
  void startLog();

  /**
   * This function stops logging information in api_log.txt.
   */
  void stopLog();

  /**
   * This function starts logging information in uart_headstage_log.txt.
   */
  void startUartLog();

  /**
   * This function stops logging information in uart_headstage_log.txt.
   */
  void stopUartLog();

  /**
   * This function reads the given amount of raw data and records it to the
   * given file.
   *
   * @param size     : the amount of bytes of raw data to read
   * @param filename : the name of the file to write to
   *
   * @return READ_SUCCESS if successful
   */
  ReadErrorCode readRawData(unsigned int size, const std::string & filename);

  /**
   * This function reads an AdcPacket from the given datalink. This datalink can
   * either be the current data connection with the basestation board, or a link
   * to a file.
   *
   * @param resultingPacket : the resulting packet of adc data
   * @param datalink        : the datalink to read from
   *
   * @return READ_SUCCESS if successful
   */
  ReadErrorCode readADCData(AdcPacket & resultingPacket,
                            NeuroseekerDataLinkIntf * datalink);

  /**
   * This function reads an ElectrodePacket from the given datalink. This
   * datalink can either be the current data connection with the basestation
   * board, or a link to a file.
   *
   * @param resultingPacket : the resulting packet of electrode data
   * @param datalink        : the datalink to read from
   *
   * @return READ_SUCCESS if successful
   */
  ReadErrorCode readElectrodeData(ElectrodePacket & resultingPacket,
                                  NeuroseekerDataLinkIntf * datalink);

  /**
   * This function skips the given number of AdcPacket/ElectrodePacket from
   * the given datalink.
   *
   * @param number          : the number of packets to skip
   * @param datalink        : the datalink to read from
   *
   * @return READ_SUCCESS if successful
   */
  ReadErrorCode skipData(unsigned int number,
                         NeuroseekerDataLinkIntf * datalink);

  /**
   * This function selects the sine wave output channel of DAC A and configures
   * its frequency, offset and amplitude.
   * Note: DAC A output will be divided internally by 201.
   *
   * @param dac:
   * DAC_A for DAC A
   * DAC_B for DAC B
   * DAC_C and DAC_D not allowed
   *
   * @param frequency:
   * Sine wave frequency selection:
   * 0000’b : 6640 Hz
   * 0001’b : 3320 Hz
   * 0010’b : 1660 Hz (power-on default)
   * 0011’b : 830.1 Hz
   * 0100’b : 415.0 Hz
   * 0101’b : 207.5 Hz
   * 0110’b : 103.8 Hz
   * 0111’b : 51.88 Hz
   * 1000’b : 25.94 Hz
   * 1001’b : 12.97 Hz
   * 1010’b : 6.485 Hz
   * 1011’b : 3.242 Hz
   * 1100’b : 1.621 Hz
   * 1101’b : 0.8106 Hz
   * 1110’b : 0.4053 Hz
   * 1111’b : 0.2027 Hz
   *
   * @param dc_offset: DC offset added to the sine wave. Range 0V - 2.5V.
   *
   * @param amplitude:
   * Sine wave amplitude selection:
   * 0000’b : 0mV p.p. voltage level
   * 0001’b : reserved
   * 0010’b : 100mV p.p. voltage level (power-on default)
   * 0011’b : 200mV p.p. voltage level
   * 0100’b : reserved
   * 0101’b : 400mV p.p. voltage level
   * 0110’b : reserved
   * 0111’b : 600mV p.p. voltage level
   * 1000’b : reserved
   * 1001’b : 800mV p.p. voltage level
   * 1010’b : reserved
   * 1011’b : 1000mV p.p. voltage level
   * 1100’b - 1111’b : reserved
   *
   * @return error of DacControlErrorCode
   */
  DacControlErrorCode generateSine(DacSel dac,
                                   int frequency,
                                   float sine_dc_offset,
                                   int amplitude);

  /**
   * This function selects the DC output channel of DAC A and configures its
   * voltage level.
   * Note: DAC A output will be divided internally by 201.
   *
   * @param dac : DAC to configure
   * @param voltage : the DC output voltage in V
   *
   * @return error of DacControlErrorCode
   */
  DacControlErrorCode generateDC(DacSel dac, float voltage);

  /**
   * This function gets the DAC Control values.
   *
   * @param frequency      : sine wave frequency
   * @param amplitude      : sine wave amplitude
   * @param sine_dc_offset : sine wave offset
   * @param dac_a_voltage  : DAC_A DC output voltage level
   * @param dac_b_voltage  : DAC_B DC output voltage level
   * @param dac_c_voltage  : DAC_C DC output voltage level
   * @param dac_d_voltage  : DAC_D DC output voltage level
   * @param dac_a_status   : true if DAC_A generates a sine wave, false otherwise
   * @param dac_b_status   : true if DAC_B generates a sine wave, false otherwise
   *
   * @return error of DacControlErrorCode
   */
  DacControlErrorCode getDacControl(int & frequency,
                                    int & amplitude,
                                    float & sine_dc_offset,
                                    float & dac_a_voltage,
                                    float & dac_b_voltage,
                                    float & dac_c_voltage,
                                    float & dac_d_voltage,
                                    bool & dac_a_status,
                                    bool & dac_b_status);

  /**
   * run BIST test 4 : test leds, test LCD, test DRAM (selftest), test TCP data link
   * (loopback)
   *
   * procedure :
   * - enable loopback mux
   * - reset datapath
   * - start DRAM selftest (write complete DRAM with incremental pattern, read and check complete DRAM)
   * - walking one on 4 Basestation LED's, every 200ms
   * - check DRAM selftest status
   * - TCP loopback test: send packet (length 1, 2, 4, 8, 16, ... up to 1Gbyte) and read it back + check. 
   *
   * @return BIST4_SUCCESS if successful
   *         BIST4_NO_DEVICE if no device opened
   *         BIST4_LED_ERROR if register read/write failure
   *         BIST4_DRAM_ERROR if DRAM selftest failure
   *         BIST4_LOOPBACK_ERROR if data link loopback failure
   */
  Bist4ErrorCode bist4();

  /**
   * run BIST test 5 : read HeadStage FPGA version + SW_HBEAT blink test @ 2Hz
   *
   * procedure :
   * - read HS_BRD_VER
   * - read HS_FPGA_VER
   * - toggle HS_GEN_CTRL1_SW_HBEAT at 2Hz, for the requested time 
   *
   * @param hs_brd_ver  : HeadStage Board Version result
   *        hs_fpga_ver : HeadStage FPGA Version result
   *        time_sec    : length of SW_HBEAT blink test, in seconds
   *
   * @return BIST_SUCCESS if successful
   *         BIST_NO_DEVICE if no device opened
   *         BIST_UART_ERROR if UART communication error
   */
  BistErrorCode bist5(unsigned char & hs_brd_ver,
                          unsigned char & hs_fpga_ver,
                          unsigned int time_sec);

  /**
   * start BIST test 6 : Ser/Des PRBS pattern
   *
   * procedure :
   * - read Serializer address 0x4, and check it is 0x87
   * - read Deserializer address 0x4, and check it is 0x87
   * - write Deserializer address 0x4, 0xa7 (add PRBSEN = 1)
   * - write Serializer address 0x4, 0xa7 (add PRBSEN = 1) 
   *
   * @return BIST6_SUCCESS if started successful
   *         BIST6_NO_DEVICE if no device opened
   *         BIST6_UART_ERROR if UART communication error
   *         BIST6_SER_ERROR if serializer status at address 0x04 does not
   *         equal 0x87
   *         BIST6_DESER_ERROR if deserializer status at address 0x04 does
   *         not equal 0x87
   *         BIST6_PRBS_ERR if PRBS_ERR is not zero
   */
  Bist6ErrorCode startBist6();

  /**
   * stop BIST test 6 : Ser/Des PRBS pattern
   *
   * procedure :
   * - write Serializer address 0x4, 0x87 -> no UART reply expected.
   * - write Deserializer address 0x4, 0x87
   * - read Serializer address 0x4, and check it is 0x87
   * - read PRBS_ERR from Deserializer address 0x13 
   *
   * @param prbs_err : error counter result
   *
   * @return BIST6_SUCCESS if successful,
   *         BIST6_NO_DEVICE if no device opened,
   *         BIST6_UART_ERROR if UART communication error,
   *         BIST6_SER_ERROR if serializer status at address 0x04 does not
   *         equal 0x87
   *         BIST6_DESER_ERROR if deserializer status at address 0x04 does
   *         not equal 0x87
   *         BIST6_PRBS_ERR if PRBS_ERR is not zero
   */
  Bist6ErrorCode stopBist6(unsigned char & prbs_err);

  /**
   * start BIST test 7 : Headstage Neural data test pattern
   *
   * procedure :
   * - write bist7_init in Basestation deserializer_intf.
   * - set BIST_TX in Headstage register HS_GEN_CTRL1 *
   *
   * @return BIST_SUCCESS if started successful
   *         BIST_NO_DEVICE if no device opened
   *         BIST_UART_ERROR if UART communication error
   */
  BistErrorCode startBist7();

  /**
   * start BIST test 7 : Headstage Neural data test pattern
   *
   * procedure :
   * - clear BIST_TX in Headstage register HS_GEN_CTRL1 
   *
   * @return BIST_SUCCESS if stopped successful
   *         BIST_NO_DEVICE if no device opened
   *         BIST_UART_ERROR if UART communication error
   */
  BistErrorCode stopBist7();

  /**
   * BIST test 7 results
   *
   * procedure :
   * - read Basestation deserializer_intf test_data_error_ctr (number of errors), test_data_checked_ctr (numbers of checks done), test_data_error_mask (mask/position of error). 
   *
   * @param error_counter : number of errors
   *        total_counter : total number of patterns checked
   *        error_mask    : mask of errors in data test pattern: bit 0 to 5 is
   *                        respectively SPI line 1 to 6
   *
   * @return BIST_SUCCESS if stopped successful
   *         BIST_NO_DEVICE if no device opened
   *         BIST_UART_ERROR if UART communication error
   */
  BistErrorCode bist7GetResults(unsigned int & error_counter,
                                unsigned int & total_counter,
                                unsigned short & error_mask);

  /**
   * start BIST test 8a (TE = true) or test 8b (TE = false)
   *
   * procedure :
   * - clear NRST
   * - set asic mode to RECORDING
   * - write 0xff to HS_BIST_ERR2 (clear all previous errors)
   * - set TE as requested
   * - read HS_GEN_CTRL1
   * - write HS_GEN_CTRL1 with SPI_LINE as requested and BIST_RX set.
   * - clear active bit: write 1 to HS_GEN_STAT, bit SPI_ACT_STAT
   * - set NRST 
   *
   * @param te : TE value.
   *        spi_line : 0 to 5, the SPI line to test.
   *
   * @return BIST8_SUCCESS if started successful
   *         BIST8_NO_DEVICE if no device opened
   *         BIST8_UART_ERROR if UART communication error
   *         BIST8_RANGE_ERR if SPI line out of range
   *         BIST8_DIGCTRL_ERR if digital control access failed
   */
  Bist8ErrorCode startBist8(bool te, unsigned char spi_line);

  /**
   * stop BIST test 8a/b
   *
   * procedure :
   * - clear BIST_RX
   * - clear TE 
   *
   * @return BIST8_SUCCESS if stopped successful
   *         BIST8_NO_DEVICE if no device opened
   *         BIST8_UART_ERROR if UART communication error
   *         BIST8_DIGCTRL_ERR if digital control access failed
   */
  Bist8ErrorCode stopBist8();

  /**
   * read result of BIST test 8a (TE = true) or test 8b (TE = false)
   *
   * procedure :
   * - read SPI_ACT from HS_GEN_STAT
   * - read spi_adc_err, spi_ctr_err and spi_sync_error from HS_BIST_ERR2
   *
   * @param te : TE value.
   *        spi_act : to store spi_act_stat bit
   *        spi_adc_err : to store spi_adc_err bit
   *        spi_ctr_err : to store spi_ctr_err bit
   *        spi_sync_err : to store spi_sync_err bit
   *
   * @return BIST_SUCCESS if successful
   *         BIST_NO_DEVICE if no device opened
   *         BIST_UART_ERROR if UART communication error
   */
  BistErrorCode bist8GetResults(bool te,
                                bool & spi_act,
                                bool & spi_adc_err,
                                bool & spi_ctr_err,
                                bool & spi_sync_err);


  /**
   * start BIST test 9a (TE = true) or test 9b (TE = false)
   *
   * procedure :
   * - reset datapath
   * - set TE as requested
   * - set check_data_pattern in deserializer_intf high if TE is requested high, low otherwise.
   *
   * @param te : TE value.
   *
   * @return BIST9_SUCCESS if started successful
   *         BIST9_NO_DEVICE if no device opened
   *         BIST9_DIGCTRL_ERR if digital control access failed
   */
  Bist9ErrorCode startBist9(bool te);

  /**
   * stop BIST test 9a/b
   *
   * procedure :
   * - clear check_data_pattern in deserializer_intf 
   *
   * @return BIST_SUCCESS if stopped successful
   *         BIST_NO_DEVICE if no device opened
   *         BIST_UART_ERROR if UART communication error
   */
  BistErrorCode stopBist9();

  /**
   * read result of BIST test 9a/b
   *
   * procedure :
   * - read sync_error_ctr (number of SYNC errors), sync_checked_ctr (number of SYNC checked) from deserializer_intf
   * - read ctr_error_ctr (number of counter errors), ctr_checked_ctr (number of counter checked) from deserializer_intf
   * - read data_pattern_spi_d0_error_ctr (number of TE data errors for spi line 0) from deserializer_intf
   * - repeat previous step for 5 others SPI lines.
   * - read data_pattern_spi_checked_ctr (number of TE data checked) from deserializer_intf. 
   *
   * @param sync_errors    : to store the number of sync errors
   *        sync_total     : to store total number of syncs checks (1 check =
   *                         all 4 syncs are correct in 1 spi packet)
   *        ctr_errors     : to store the number of counter errors
   *        ctr_total      : to store the total number of counter 10 lsbs and 10
   *                         msbs checks (1 check = all 6 10-lsbs or
   *                         all 6 10-msbs are correct)
   *
   *        te_spi0_errors : to store the number of data errors on spi line 0
   *        te_spi1_errors : to store the number of data errors on spi line 1
   *        te_spi2_errors : to store the number of data errors on spi line 2
   *        te_spi3_errors : to store the number of data errors on spi line 3
   *        te_spi4_errors : to store the number of data errors on spi line 4
   *        te_spi5_errors : to store the number of data errors on spi line 5
   *        te_spi_total   : to store the total number of data checked per spi
   *                         line
   */
  void bist9GetResults(unsigned int & sync_errors,
                       unsigned int & sync_total,
                       unsigned int & ctr_errors,
                       unsigned int & ctr_total,
                       unsigned int & te_spi0_errors,
                       unsigned int & te_spi1_errors,
                       unsigned int & te_spi2_errors,
                       unsigned int & te_spi3_errors,
                       unsigned int & te_spi4_errors,
                       unsigned int & te_spi5_errors,
                       unsigned int & te_spi_total);

  /**
   * Enable/Disable the Basestation generator.
   *
   * @param enable : true to enable generator, false to disable
   *        mode   : mode for data generator
   *                 0 : output constantly adc number
   *                 1 : all ADC = r_packet_ctr ( = sync_counter LSB + 1)
   *                 2 : all ADC = r_packet_ctr ( = sync_counter MSB + 1)
   *                 3 : all ADC = triangular signal with configurable period
   *        step   : exponent of step of triangular signal in mode 3
   *                 period = 2 * full_swing * 2^step [40.8MHz clk cycles]
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode enableBasestationGenerator(bool enable,
                                                   unsigned char mode = 0,
                                                   unsigned int step = 0);

  /**
   * This function gets the basestation data generator parameters.
   *
   * @param enable : true if enabled, false if disabled
   * @param mode   : mode for data generator
   * @param period : period of triangular signal in mode 3
   *
   * @return CONFIG_ERROR_NO_LINK if no config link,
   *         CONFIG_SUCCESS if successful
   */
  ConfigAccessErrorCode getBasestationGeneratorEnabled(bool & enable,
                                                       unsigned char &
                                                       mode, unsigned
                                                       int & period);

  /**
   * Set the UART baudrate.
   *
   * @param alternateBaudrate: false = 487.5 kbauds
   *                           true = 425 kbauds
   */
  void setAlternateBaudrate(bool alternateBaudrate);

  /**
   * This function reads the filling of the data fifo.
   *
   * @param filling : filling level, in %
   *
   * @return : CONFIG_SUCCESS if successful, CONFIG_ERROR_NO_LINK if no config
   * link
   */
  ConfigAccessErrorCode getFifoFilling(float & filling);

  /**
   * This function writes the general configuration chain to the headstage ASIC.
   *
   * @param readCheck : if enabled, read the general configuration shift
   *                     register back to check
   *
   * @return SHIFTREG_SUCCESS if successful
   */
  ShiftRegisterAccessErrorCode writeGeneralRegister(bool readCheck);

  /**
   * This function reads out the general configuration chain from the headstage
   * ASIC and writes the values to the general configuration member of the api.
   *
   * @param chain : the chain of general register data to return
   *
   * @return SHIFTREG_SUCCESS if successful
   */
  ShiftRegisterAccessErrorCode readGeneralRegister(ShiftRegister & chain);

  /**
   * This function writes the 6 channel configuration chains to the headstage
   * ASIC.
   *
   * @param readCheck : if enabled, read the channel configuration shift
   *                    registers back to check
   *
   * @return SHIFTREG_SUCCESS if successful
   */
  ShiftRegisterAccessErrorCode writeChannelRegisters(bool readCheck);

  /**
   * This function reads out the channel configuration chains from the headstage
   * ASIC and writes the values to the all channel configurations member of the
   * api.
   *
   * @param chains : the chains of channel register data to return
   *
   * @return SHIFTREG_SUCCESS if successful
   */
  ShiftRegisterAccessErrorCode readChannelRegisters(std::array<ShiftRegister,
                                                    NUMBER_OF_CHANNELCONFIGS>
                                                    & chains);

  /**
   * This function organises a burst of uart accesses, controlled by a vector of
   * commands.
   *
   * @param commands : the vector of field_command, with the commands that
   *                   control the uart.
   * @param readdata : the vector of data bytes that are read over uart.
   *
   * @return error of UartErrorCode (the first error that was flagged)
   */
  UartErrorCode uartBurst(std::vector<field_command> & commands,
                          std::vector<unsigned char> & readdata);

  /**
   * This function calibrates the slope of each ASIC ADC, by searching the ideal
   * slope, coarse and fine parameters.
   *
   * @param datalink : the datalink to read the necessary data from
   *                   if nullptr: the real TCP link is used (if connected)
   * @param fileName : the file name to save the results to, should be .csv,
   *                   default file name is results_adcSlopeCalibration.csv
   *
   * @return CALIB_SUCCESS if successful
   */
  CalibErrorCode ADCSlopeCalibration(NeuroseekerDataLinkIntf * datalink,
    const std::string & filename = "results_adcSlopeCalibration.csv");

  /**
   * This function calibrates the slope of each ASIC ADC, by searching the ideal
   * slope, coarse and fine parameters.
   *
   * @param inputfilenuame : the file to read the inputdata from,
   *                         if default "" used: the real TCP link is used if
   *                         connected
   * @param fileName : the file name to save the results to, should be .csv,
   *                   default file name is results_adcSlopeCalibration.csv
   *
   * @return CALIB_SUCCESS if successful
   */
  CalibErrorCode ADCSlopeCalibration(const std::string & inputfilename = "",
    const std::string & filename = "results_adcSlopeCalibration.csv");


  /**
   * This function calibrates the offset of each ASIC ADC, by searching the
   * ideal cfix parameter.
   *
   * @param datalink : the datalink to read the necessary data from
   *                   if nullptr: the real TCP link is used (if connected)
   * @param filename : the file name to save the results to, should be .csv,
   *                   default file name is results_adcOffsetCalibration.csv
   *
   * @return CALIB_SUCCESS if successful
   */
  CalibErrorCode ADCOffsetCalibration(NeuroseekerDataLinkIntf * datalink,
                                      const std::string & filename =
                                      "results_adcOffsetCalibration.csv");

  /**
   * This function calibrates the offset of each ASIC ADC, by searching the
   * ideal cfix parameter.
   *
   * @param inputfilename : the file to read the inputdata from,
   *                         if default "" used: the real TCP link is used if
   *                         connected
   * @param filename : the file name to save the results to, should be .csv,
   *                   default file name is results_adcOffsetCalibration.csv
   *
   * @return CALIB_SUCCESS if successful
   */
  CalibErrorCode ADCOffsetCalibration(const std::string & inputfilename = "",
                                      const std::string & filename =
                                      "results_adcOffsetCalibration.csv");


  /**
   * This function calibrates the comparator of each ASIC ADC, by searching the
   * ideal compP and compN parameters.
   *
   * @param datalink : the datalink to read the necessary data from
   *                   if nullptr: the real TCP link is used (if connected)
   * @param filename : the file name to save the results to, should be .csv,
   *                   default file name is results_comparatorCalibration.csv
   *
   * @return CALIB_SUCCESS if successful
   */
  CalibErrorCode comparatorCalibration(NeuroseekerDataLinkIntf * datalink,
                                       const std::string & filename =
                                       "results_comparatorCalibration.csv");

  /**
   * This function calibrates the comparator of each ASIC ADC, by searching the
   * ideal compP and compN parameters.
   *
   * @param inputfilename : the file to read the inputdata from,
   *                         if default "" used: the real TCP link is used if
   *                         connected
   * @param filename : the file name to save the results to, should be .csv,
   *                   default file name is results_comparatorCalibration.csv
   *
   * @return CALIB_SUCCESS if successful
   */
  CalibErrorCode comparatorCalibration(const std::string & inputfilename = "",
                                       const std::string & filename =
                                       "results_comparatorCalibration.csv");

  /**
   * This function sets the adcPairCommonCalibration_ member of the api.
   *
   * @param adcPairCommonCalibration : new value, array of 36 adcPairCommon
   */
  void setADCPairCommonCalibration(const
                                   std::array<AdcPairCommon, NUMBER_OF_ADCPAIRS>
                                   & adcPairCommonCalibration);

  /**
   * This function returns the adcPairCommonCalibration_ member of the api.
   *
   * @return adcPairCommonCalibration : current value, array of 36.
   */
  std::array<AdcPairCommon, NUMBER_OF_ADCPAIRS> getADCPairCommonCalibration()
    const;

  /**
   * This function sets the adcCompCalibration_ member of the api.
   *
   * @param adcCompCalibration : new value, array of 72 adcComp
   */
  void setADCCompCalibration(const std::array<AdcComp, NUMBER_OF_ADCS> &
                             adcCompCalibration);

  /**
   * This function returns the adcCompCalibration_ member of the api.
   *
   * @return adcCompCalibration : current value, vector of 72
   */
  std::array<AdcComp, NUMBER_OF_ADCS> getADCCompCalibration() const;

  /**
   * This function reads the ADC slope calibration values (slope, coarse and
   * fine) from the given csv file.
   *
   * @param filename : the filename to read from (should be .csv), default is
   *                   adcSlopeCalValues.csv
   *
   * @return READCSV_SUCCES if succesful
   */
  ReadCsvErrorCode readADCSlopeCalibrationFromCsv(std::string filename = "adcSlopeCalValues.csv");

  /**
   * This function reads the ADC offset calibration values (cfix) from the given
   * csv file.
   *
   * @param filename : the filename to read from (should be .csv), default is
   *                   adcOffsetCalValues.csv
   *
   * @return READCSV_SUCCES if succesful
   */
  ReadCsvErrorCode readADCOffsetCalibrationFromCsv(std::string filename = "adcOffsetCalValues.csv");

  /**
   * This function reads the comparator calibration values (compP and compN)
   * from the given csv file.
   *
   * @param filename : the filename to read from (should be .csv), default is
   *                   comparatorCalValues.csv
   *
   * @return READCSV_SUCCES if succesful
   */
  ReadCsvErrorCode readComparatorCalibrationFromCsv(std::string filename = "comparatorCalValues.csv");

  /**
   * This function writes the ADC slope calibration values (slope, coarse and
   * fine) to the given csv file.
   *
   * @param filename : the filename to write to, (should be .csv) default is
   *                   adcSlopeCalValues.csv
   */
  void writeADCSlopeCalibrationToCsv(std::string filename = "adcSlopeCalValues.csv");

  /**
   * This function writes the ADC offset calibration values (cfix) to the given
   * csv file.
   *
   * @param filename : the filename to write to (should be .csv), default is
   *                   adcOffsetCalValues.csv
   */
  void writeADCOffsetCalibrationToCsv(std::string filename = "adcOffsetCalValues.csv");

  /**
   * This function writes the comparator offset calibration values (compP and
   * compN) to the given csv file.
   *
   * @param filename : the filename to write to (should be .csv), default is
   *                   comparatorCalValues.csv
   */
  void writeComparatorCalibrationToCsv(std::string filename = "comparatorCalValues.csv");

  /**
   * This function writes the general configuration register as a bit chain to the
   * given csv file.
   *
   * @param filename : the filename to write to (should be .csv), default is
   *                   generalConfiguration.csv
   * @return SHIFTREG_SUCCESS if successful
   */
  ShiftRegisterAccessErrorCode writeGeneralConfigurationToCsv(std::string filename = "generalConfiguration.csv");

  /**
   * This function writes the channel configuration registers as bit chains to the
   * given csv file.
   *
   * @param filename : the filename to write to (should be .csv), default is
   *                   channelConfiguration.csv
   * @return SHIFTREG_SUCCESS if successful
   */
  ShiftRegisterAccessErrorCode writeChannelConfigurationToCsv(std::string filename = "channelConfiguration.csv");

  /**
   * This function reads the general configuration register as a bit chain from the
   * given csv file, and loads it into the general configuration member of the api.
   *
   * @param filename : the filename to read from (should be .csv), default is
   *                   generalConfiguration.csv
   *
   * @return READCSV_SUCCESS if succesful
   */
  ReadCsvErrorCode readGeneralConfigurationFromCsv(std::string filename = "generalConfiguration.csv");

  /**
   * This function reads the channel configuration registers as bit chains from
   * the given csv file, and loads it into the channel configuration member of the
   * api.
   *
   * @param filename : the filename to read from (should be .csv), default is
   *                   channelConfiguration.csv
   *
   * @return READCSV_SUCCESS if succesful
   */
  ReadCsvErrorCode readChannelConfigurationFromCsv(std::string filename = "channelConfiguration.csv");


  GeneralConfiguration generalConfiguration;
  AllChannelConfigurations allChannelConfigurations;

protected:
  /**
   * This function sets up a configuration link.
   *
   * @return SUCCESS if succesful
   */
  virtual ErrorCode setupConfigLink();

  /**
   * This function sets up a data link.
   *
   * @return SUCCESS if succesful
   */
  virtual ErrorCode setupDataLink();

  /**
   * This function closes the data link.
   *
   * @return SUCCESS if succesful
   */
  virtual void closeDataLink();

  /**
   * This function makes a field_command for UART burst
   *
   * @return field_command structure
   */
  field_command makeFieldCommand(bool read_not_write,
                                 unsigned char device,
                                 unsigned char address,
                                 unsigned char data = 0);


  neuroseeker * datamodel_;
  VciInterface * vciIntf_;
  std::ofstream logFile_;
  std::ofstream uartLogFile_;
  NeuroseekerDataLinkIntf * tcpDataLink_;

private:
  bool electrodeMode_; // false is ADC mode
  bool alternateBaudrate_;
  std::array<AdcPairCommon,NUMBER_OF_ADCPAIRS> adcPairCommonCalibration_;
  std::array<AdcComp, NUMBER_OF_ADCS> adcCompCalibration_;

  // variable related to TCP buffer mechanism
  unsigned int datachain_size_; // buffer size for electrode/adc packets as read from DataLinkIntf
  char * datachain_; // buffer for electrode/adc packets as read from DataLinkIntf
  unsigned int datachain_index_; // iterator through buffer

  /**
   * This function waits x milliseconds.
   *
   * @param milliseconds: the amount of milliseconds to wait
   */
  virtual void milliSleep(unsigned int milliseconds);

  /**
   * This function resets all relevant headstage registers.
   *
   * @return UART_SUCCESS if succesful
   */
  UartErrorCode resetHeadstageRegisters();

  /**
   * This function flushes the tx fifo of the data link.
   * For this it closes and reopens the data link.
   */
  virtual void flushTxFifo();

  /**
   * This function makes a field_command to write the headstage register
   * HS_CONF_SC3 (GEN_RECEIVE) over uart, with the given value as the data to
   * write.
   *
   * @param data: the data value to write
   *
   * @return : the field_command for the write
   */
  field_command makeCommandWriteGeneralConfig(unsigned char data) const;

  /**
   * This function makes a field_command to write a 1 to the GEN_SHIFT bit of
   * the HS_CONF_SC1 headstage register (0 to CH_SHIFT).
   *
   * @return : the field_command for the write
   */
  field_command makeCommandWriteGeneralShift() const;

  /**
   * This function makes a field_command to read the headstage register
   * HS_CONF_SC2 (GEN_TRANSMIT) over uart.
   *
   * @return : the field_command for the read
   */
  field_command makeCommandReadGeneralConfig() const;

  /**
   * This function makes a field_command to write one of the CH_TRANSMIT
   * registers over uart, with the given channelconfig number and the given
   * data.
   *
   * @param channelconfignumber : the number of the channel configuration
   *                              (valid range: 0 to 5)
   * @param data                : the data value to write
   *
   * @return : the field_command for the write
   */
  field_command makeCommandWriteChannelConfig(unsigned int channelconfignumber,
                                              unsigned char data) const;

  /**
   * This function makes a field_command to write a 1 to the CH_SHIFT bit of the
   * HS_CONF_SC1 headstage register (0 to GEN_SHIFT).
   *
   * @return : the field_command for the write
   */
  field_command makeCommandWriteChannelShift() const;

  /**
   * This fu nction makes a field_command to read one of the CH_RECEIVE
   * registers over uart.
   *
   * @return : the field_command for the read
   */
  field_command makeCommandReadChannelConfig(unsigned int channelconfignumber)
    const;

  /**
   * This function shifts the given vector of unsigned chars with the given
   * amount of bits.
   *
   * @param vectorToShift: the vector to shift
   * @param shiftedVector: the resulting vector
   * @param numberOfBits : the amount of bits to shift
   */
  void leftShiftVector(const std::vector<unsigned char> & vectorToShift,
                       std::vector<unsigned char> & shiftedVector,
                       unsigned char numberOfBits);

  /**
   * This function copies the elements of a vector to a ShiftRegister.
   *
   * @param vectorToCopy: the vector to copy
   * @param shiftRegister: the shift register to put the result in
   *
   * @return SUCCESS if successful,
   *         FAILURE if accessing  the shift register caused an error
   */
  ErrorCode
    copyVectorToShiftRegister(const std::vector<unsigned char> & vectorToCopy,
                              ShiftRegister & shiftRegister);

  /**
   * This function splits the given vector into 6 vectors, for the 6 channel
   * configurations.
   *
   * @param vectorToSplit : the vector to split
   * @param resultingVectors : the array of 6 resulting vectors
   */
  void splitVector(const std::vector<unsigned char> &
                   vectorToSplit, std::array<std::vector<unsigned char>,
                   NUMBER_OF_CHANNELCONFIGS> & resultingVectors) const;

  /**
   * This function handles the startup configuration steps for adc calibration.
   *
   * @return CALIB_SUCCESS if successful
   */
  CalibErrorCode calibrationStartupConfiguration();

  /**
   * This function writes the calibrated slope, coarse and fine values for each
   * ADC pair to a csv file, with the ADC slope that resulted from the
   * calibration calculations.
   *
   * @param adcSlopes : the slopes per ADC that resulted in the best combined
   *                    distances
   * @param fileName  : the name of the file to save to
   */
  void writeSlopeCalibrationToCsv(const std::array<float, NUMBER_OF_ADCS> &
                                  adcSlopes,
                                  const std::string & fileName) const;

  /**
   * This function adds the slope, coarse and fine values, as stored in the
   * calibration member, to the given (opened) filestream.
   *
   * @param filestream : the opened filestream to add the values to.
   */
  void addSlopeCalibrationValuesToFilestream(std::ofstream & filestream) const;

  /**
   * This function writes the calibrated cfix values for each ADC pair to a csv
   * file, with the ADC offset code that resulted from the calibration
   * calculations.
   *
   * @param adcPairOutputCode : the digital output codes per ADC pair that
   *                            resulted in the optimal cfix values
   * @param fileName          : the name of the file to save to
   */
  void writeOffsetCalibrationToCsv(const std::array<float, NUMBER_OF_ADCS> &
                                   adcPairOutput,
                                   const std::string fileName) const;

  /**
   * This function adds the cfix values as stored in the calibration member, to
   * the given (opened) filestream.
   *
   * @param filestream : the opened filestream to add the values to.
   */
  void addOffsetCalibrationValuesToFilestream(std::ofstream & filestream) const;

  /**
   * This function writes the calibrated comparator CompP and CompN values to a
   * csv file, with the corresponding percentage of ones that resulted from the
   * calibration calculations.
   *
   * @param onesPerAdc : the percentage of ones that resulted in the optimal
   *                     comparator values
   * @param fileName   : the name of the file to save to
   */
  void writeComparatorCalibrationToCsv(const std::array<double, NUMBER_OF_ADCS>
                                       & onesPerAdc,
                                       const std::string & fileName) const;

  /**
   * This function adds the compP and compN values, as stored in the calibration
   * member, to the given (opened) filestream.
   *
   * @param filestream : the opened filestream to add the values to.
   */
  void addComparatorCalibrationValuesToFilestream(std::ofstream & filestream)
    const;

  /**
   * This function handles one string line of a csv file, and splits it into a
   * vector of strings.
   *
   * @param line   : the string line to split
   * @param vector : the string vector of the splitted cells
   */
  void splitCsvLineIntoCells(std::string line, std::vector<std::string> &
                             cells);

  /**
   * This function handles the writing of a chain to a csv file.
   *
   * @param filename : the file to write to (should be .csv)
   * @param chain    : the chain to write
   */
  void writeChainToCsv(std::string filename, std::vector<bool> & chain);

  /**
   * This function handles the writing of a chain of length n to a csv file.
   *
   * @param filename : the file to write to (should be .csv)
   * @param chain    : the chain to write (in a std::array)
   */
  template<int n>
    void writeChainToCsv(std::string filename, std::array<bool, n> & chain);

  /**
   * This function handles the writing of NUMBER_OF_CHANNELCONFIGS chains of length n to a csv file.
   *
   * @param filename : the file to write to (should be .csv)
   * @param chains   : the chains to write (in a std::array of std::arrays)
   */
  template<int n>
    void writeChainsToCsv(std::string filename, std::array<std::array<bool, n>,
                          NUMBER_OF_CHANNELCONFIGS> & chains);

  /**
   * This function handles the reading a chain from a csv file.
   *
   * @param filename : the file to read from (should be .csv)
   * @param chain    : the chain that was read
   *
   * @return READCSV_SUCCESS if succesful
   */
  ReadCsvErrorCode readChainFromCsv(std::string filename, std::vector<bool> & chain);

  /**
   * This function handles the reading a chain of length n from a csv file.
   *
   * @param filename : the file to read from (should be .csv)
   * @param chain    : the chain that was read (in a std::array)
   *
   * @return READCSV_SUCCESS if succesful
   */
  template<int n>
    ReadCsvErrorCode readChainFromCsv(std::string filename, std::array<bool, n> & chain);

  /**
   * This function handles the reading NUMBER_OF_CHANNELCONFIGS chains of length n from a csv file.
   *
   * @param filename : the file to read from (should be .csv)
   * @param chains   : the chains that were read (in a std::array of std::arrays)
   *
   * @return READCSV_SUCCESS if succesful
   */
  template<int n>
    ReadCsvErrorCode readChainsFromCsv(std::string filename,
                                       std::array<std::array<bool, n>,
                                       NUMBER_OF_CHANNELCONFIGS> & chains);

};

template<int n>
void NeuroseekerAPI::writeChainToCsv(std::string filename, std::array<bool, n> &
                                     chain)
{
  std::vector<bool> v;
  v.resize(n);
  for (unsigned int i = 0; i < n; i++)
    {
      v[i] = chain[i];
    }
  writeChainToCsv(filename, v);
}

template<int n>
void NeuroseekerAPI::writeChainsToCsv(std::string filename, std::array<std::array<bool, n>,
                      NUMBER_OF_CHANNELCONFIGS> & chains)
{
  std::ofstream outputfile;
  outputfile.open(filename.c_str());

  for (unsigned int i = 0; i < n; i++)
    {
      for (unsigned int j = 0; j < NUMBER_OF_CHANNELCONFIGS; j++)
        outputfile << chains[j][i] << ",";
      outputfile << "\n";
    }

  outputfile.close();
}

template<int n>
ReadCsvErrorCode NeuroseekerAPI::readChainFromCsv(std::string filename, std::array<bool, n>
                                      & chain)
{
  std::vector<bool> v;
  ReadCsvErrorCode feedback = readChainFromCsv(filename, v);
  if (feedback != READCSV_SUCCESS)
    return feedback;
  if (v.size() != n)
    return READCSV_NUMBER_OF_ELEMENTS;
  for (unsigned int i = 0; i < n; i++)
    {
      chain[i] = v[i];
    }
  return READCSV_SUCCESS;
}

template<int n>
ReadCsvErrorCode NeuroseekerAPI::readChainsFromCsv(std::string filename,
                                                   std::array<std::array<bool,
                                                   n>, NUMBER_OF_CHANNELCONFIGS>
                                      & chains)
{
  std::vector<bool> v;

  std::string line;
  std::string element;

  std::ifstream inputfile;
  inputfile.open(filename.c_str());
  if (!inputfile.is_open())
    return READCSV_FILE_ERR;

  v.resize(0);
  std::getline(inputfile, line);
  while (!inputfile.eof())
    {
      std::stringstream ss(line);
      std::getline(ss, element, ',');
      v.push_back(element == "1");
      std::getline(inputfile, line);
    }

  inputfile.close();

  if (v.size() != NUMBER_OF_CHANNELCONFIGS*n)
    return READCSV_NUMBER_OF_ELEMENTS;
  for (unsigned int j = 0; j < NUMBER_OF_CHANNELCONFIGS; j++)
    {
      for (unsigned int i = 0; i < n; i++)
        chains[j][i] = v[j*n + i];
    }
  return READCSV_SUCCESS;
}

#endif
