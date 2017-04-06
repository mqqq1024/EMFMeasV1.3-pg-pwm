#ifndef __DATATYPE_H__
#define	__DATATYPE_H__

#pragma pack(1)

typedef struct general_channel_t {
	uint16_t max;
	uint16_t min;
	uint16_t center;
	uint16_t zeroNum;
	uint16_t zeroPos[15];
	uint16_t RmsHaff;
	uint16_t RmsDat;
	uint16_t RmsVal;
	uint16_t hLevel;
	uint16_t lLevel;
	uint16_t freq;
	uint16_t duty;
} CHANNEL_T;

typedef struct hall_result_t {
	uint32_t AFreq;
	uint32_t ADuty;
	uint32_t BFreq;
	uint32_t BDuty;
	uint32_t CFreq;
	uint32_t CDuty;
	uint32_t ABPhase;
	uint32_t ACPhase;
	uint8_t result;
	uint16_t Current;
	uint32_t AFreqAve;
	uint32_t BFreqAve;
	uint32_t CFreqAve;
	uint32_t ADutyAve;
	uint32_t BDutyAve;
	uint32_t CDutyAve;
} HALL_T;

typedef struct hall_capture_t {
	uint32_t Time[15];
	uint8_t Level[15];
	uint8_t ptr;
} HALLCAP_T;

typedef struct freq_t {
	uint32_t	sTime;
	uint32_t	eTime;
	uint32_t	cnt;
}HALLFREQ_T;

typedef struct power_t {
	int32_t bvoltadge;
	int32_t bcurrent;
	int32_t bpower;
	int32_t voltadge;
	int32_t current;
	int32_t power;
	int32_t ripple;
	int32_t Ivcc;
	int32_t Ivsp;
	uint32_t	std;
	uint32_t	std_1;
	uint32_t reserved[5];
} POWER_T;

typedef struct calibration_t {
	int32_t k;
	int32_t b;
} CALIBRATION_T;

typedef struct sys_calibration_t {
	CALIBRATION_T uCoeff;
	CALIBRATION_T iCoeff;
	CALIBRATION_T pCoeff;
	CALIBRATION_T IvccCoeff;
	CALIBRATION_T IvspCoeff;
}SYSCALIBRATION_T;

#pragma pack()

#endif

