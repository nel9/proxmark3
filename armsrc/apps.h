//-----------------------------------------------------------------------------
// Jonathan Westhues, Aug 2005
// Gerhard de Koning Gans, April 2008, May 2011
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Definitions internal to the app source.
//-----------------------------------------------------------------------------

#ifndef __APPS_H
#define __APPS_H

#include <stdint.h>
#include <stddef.h>
#include "common.h"
#include "hitag2.h"
#include "mifare.h"
#include "../common/crc32.h"
#include "BigBuf.h"
#include "fpgaloader.h"

extern const uint8_t OddByteParity[256];
extern int rsamples;   // = 0;
extern int tracing;    // = TRUE;
extern uint8_t trigger;

// This may be used (sparingly) to declare a function to be copied to
// and executed from RAM
#define RAMFUNC __attribute((long_call, section(".ramfunc")))

/// appmain.h
void ReadMem(int addr);
void __attribute__((noreturn)) AppMain(void);
void SamyRun(void);
//void DbpIntegers(int a, int b, int c);
void DbpString(char *str);
void Dbprintf(const char *fmt, ...);
void Dbhexdump(int len, uint8_t *d, bool bAsci);

// ADC Vref = 3300mV, and an (10M+1M):1M voltage divider on the HF input can measure voltages up to 36300 mV
#define MAX_ADC_HF_VOLTAGE 36300
// ADC Vref = 3300mV, and an (10000k+240k):240k voltage divider on the LF input can measure voltages up to 140800 mV
#define MAX_ADC_LF_VOLTAGE 140800
int AvgAdc(int ch);

void ToSendStuffBit(int b);
void ToSendReset(void);
void ListenReaderField(int limit);
extern int ToSendMax;
extern uint8_t ToSend[];


/// lfops.h
extern uint8_t decimation;
extern uint8_t bits_per_sample ;
extern bool averaging;

void AcquireRawAdcSamples125k(int divisor);
void ModThenAcquireRawAdcSamples125k(int delay_off,int period_0,int period_1,uint8_t *command);
void ReadTItag(void);
void WriteTItag(uint32_t idhi, uint32_t idlo, uint16_t crc);

void AcquireTiType(void);
void AcquireRawBitsTI(void);
void SimulateTagLowFrequency(int period, int gap, int ledcontrol);
void CmdHIDsimTAG(int hi, int lo, int ledcontrol);
void CmdFSKsimTAG(uint16_t arg1, uint16_t arg2, size_t size, uint8_t *BitStream);
void CmdASKsimTag(uint16_t arg1, uint16_t arg2, size_t size, uint8_t *BitStream);
void CmdPSKsimTag(uint16_t arg1, uint16_t arg2, size_t size, uint8_t *BitStream);
void CmdHIDdemodFSK(int findone, int *high, int *low, int ledcontrol);
void CmdAWIDdemodFSK(int findone, int *high, int *low, int ledcontrol); // Realtime demodulation mode for AWID26
void CmdEM410xdemod(int findone, int *high, int *low, int ledcontrol);
void CmdIOdemodFSK(int findone, int *high, int *low, int ledcontrol);
void CopyIOtoT55x7(uint32_t hi, uint32_t lo, uint8_t longFMT); // Clone an ioProx card to T5557/T5567
void SimulateTagLowFrequencyBidir(int divisor, int max_bitlen);
void CopyHIDtoT55x7(uint32_t hi2, uint32_t hi, uint32_t lo, uint8_t longFMT); // Clone an HID card to T5557/T5567
void WriteEM410x(uint32_t card, uint32_t id_hi, uint32_t id_lo);
void CopyIndala64toT55x7(int hi, int lo); // Clone Indala 64-bit tag by UID to T55x7
void CopyIndala224toT55x7(int uid1, int uid2, int uid3, int uid4, int uid5, int uid6, int uid7); // Clone Indala 224-bit tag by UID to T55x7
void T55xxWriteBlock(uint32_t Data, uint32_t Block, uint32_t Pwd, uint8_t PwdMode);
void T55xxReadBlock(uint32_t Block, uint32_t Pwd, uint8_t PwdMode );
void T55xxReadTrace(void);
int DemodPCF7931(uint8_t **outBlocks);
int IsBlock0PCF7931(uint8_t *Block);
int IsBlock1PCF7931(uint8_t *Block);
void ReadPCF7931();
void SendCmdPCF7931(uint32_t * tab);
bool AddBytePCF7931(uint8_t byte, uint32_t * tab, int32_t l, int32_t p);
bool AddBitPCF7931(bool b, uint32_t * tab, int32_t l, int32_t p);
bool AddPatternPCF7931(uint32_t a, uint32_t b, uint32_t c, uint32_t * tab);
void WritePCF7931(uint8_t pass1, uint8_t pass2, uint8_t pass3, uint8_t pass4, uint8_t pass5, uint8_t pass6, uint8_t pass7, uint16_t init_delay, int32_t l, int32_t p, uint8_t address, uint8_t byte, uint8_t data);
void EM4xReadWord(uint8_t Address, uint32_t Pwd, uint8_t PwdMode);
void EM4xWriteWord(uint32_t Data, uint8_t Address, uint32_t Pwd, uint8_t PwdMode);

/// iso14443.h
void SimulateIso14443bTag(void);
void AcquireRawAdcSamplesIso14443b(uint32_t parameter);
void ReadSTMemoryIso14443b(uint32_t);
void RAMFUNC SnoopIso14443b(void);
void SendRawCommand14443B(uint32_t, uint32_t, uint8_t, uint8_t[]);

/// iso14443a.h
void RAMFUNC SnoopIso14443a(uint8_t param);
void SimulateIso14443aTag(int tagType, int uid_1st, int uid_2nd, byte_t* data);
void ReaderIso14443a(UsbCommand * c);
// Also used in iclass.c
bool RAMFUNC LogTrace(const uint8_t *btBytes, uint16_t len, uint32_t timestamp_start, uint32_t timestamp_end, uint8_t *parity, bool readerToTag);
void GetParity(const uint8_t *pbtCmd, uint16_t len, uint8_t *parity);
void iso14a_set_trigger(bool enable);

void RAMFUNC SniffMifare(uint8_t param);

/// epa.h
void EPA_PACE_Collect_Nonce(UsbCommand * c);
void EPA_PACE_Replay(UsbCommand *c);

// mifarecmd.h
void ReaderMifare(bool first_try);
int32_t dist_nt(uint32_t nt1, uint32_t nt2);
void MifareReadBlock(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t *data);
void MifareUReadBlock(uint8_t arg0, uint8_t arg1, uint8_t *datain);
void MifareUC_Auth(uint8_t arg0, uint8_t *datain);
void MifareUReadCard(uint8_t arg0, uint16_t arg1, uint8_t arg2, uint8_t *datain);
void MifareReadSector(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t *datain);
void MifareWriteBlock(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t *datain);
//void MifareUWriteBlockCompat(uint8_t arg0,uint8_t *datain);
void MifareUWriteBlock(uint8_t arg0, uint8_t arg1, uint8_t *datain);
void MifareNested(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void MifareChkKeys(uint16_t arg0, uint8_t arg1, uint8_t arg2, uint8_t *datain);
void Mifare1ksim(uint8_t arg0, uint8_t arg1, uint8_t arg2, uint8_t *datain);
void MifareSetDbgLvl(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void MifareEMemClr(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void MifareEMemSet(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void MifareEMemGet(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void MifareECardLoad(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void MifareCSetBlock(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);  // Work with "magic Chinese" card
void MifareCGetBlock(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void MifareCIdent();  // is "magic chinese" card?
void MifareUSetPwd(uint8_t arg0, uint8_t *datain);

//desfire
void Mifare_DES_Auth1(uint8_t arg0,uint8_t *datain);
void Mifare_DES_Auth2(uint32_t arg0, uint8_t *datain);					   

// mifaredesfire.h
bool 	InitDesfireCard();
void	MifareSendCommand(uint8_t arg0,uint8_t arg1, uint8_t *datain);
void 	MifareDesfireGetInformation();
void 	MifareDES_Auth1(uint8_t arg0,uint8_t arg1,uint8_t arg2, uint8_t *datain);
void 	ReaderMifareDES(uint32_t param, uint32_t param2, uint8_t * datain);
int 	DesfireAPDU(uint8_t *cmd, size_t cmd_len, uint8_t *dataout);
size_t	CreateAPDU( uint8_t *datain, size_t len, uint8_t *dataout);
void 	OnSuccess();
void 	OnError(uint8_t reason);


/// iso15693.h
void RecordRawAdcSamplesIso15693(void);
void AcquireRawAdcSamplesIso15693(void);
void ReaderIso15693(uint32_t parameter);	// Simulate an ISO15693 reader - greg
void SimTagIso15693(uint32_t parameter, uint8_t *uid);	// simulate an ISO15693 tag - greg
void BruteforceIso15693Afi(uint32_t speed); // find an AFI of a tag - atrox
void DirectTag15693Command(uint32_t datalen,uint32_t speed, uint32_t recv, uint8_t data[]); // send arbitrary commands from CLI - atrox 
void SetDebugIso15693(uint32_t flag);

/// iclass.h
void RAMFUNC SnoopIClass(void);
void SimulateIClass(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint8_t *datain);
void ReaderIClass(uint8_t arg0);
void ReaderIClass_Replay(uint8_t arg0,uint8_t *MAC);
void IClass_iso14443A_GetPublic(uint8_t arg0);
void iClass_Authentication(uint8_t *MAC);
void iClass_WriteBlock(uint8_t blockNo, uint8_t *data);
void iClass_ReadBlk(uint8_t blockNo);
bool iClass_ReadBlock(uint8_t blockNo, uint8_t *readdata);
void iClass_Dump(uint8_t blockno, uint8_t numblks);
void iClass_Clone(uint8_t startblock, uint8_t endblock, uint8_t *data);
void iClass_ReadCheck(uint8_t	blockNo, uint8_t keyType);

// hitag2.h
void SnoopHitag(uint32_t type);
void SimulateHitagTag(bool tag_mem_supplied, byte_t* data);
void ReaderHitag(hitag_function htf, hitag_data* htd);

// cmd.h
bool cmd_receive(UsbCommand* cmd);
bool cmd_send(uint32_t cmd, uint32_t arg0, uint32_t arg1, uint32_t arg2, void* data, size_t len);

/// util.h

#endif
