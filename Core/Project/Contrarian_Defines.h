#ifndef __CONTRARIAN_1PH_DEFINES_H__
#define __CONTRARIAN_1PH_DEFINES_H__

#define ON  true
#define OFF false

#define PRINPUT 1
#define DUTYOUT 2
#define CURRREF 3

#define PR_ 1
#define PI_ 0
//选择性编译
#define OFFEST_GET OFF
#define VALUEOUT_PRINT ON

#define SingleCurrentLoop OFF
#define LOOPOUT_PRINT  0
#define labrun 0
#define LOOPTEST OFF


#define Device 1
//常数
#define SQRT_2 1.414213562373f

#define IT_FRE 25000.0f
#define AC_FRE 50.0f
#define CurLoop_FRE 25000.0f
#define VolLoop_FRE 5000.0f
#define SPLL_B0     66.69341721f
#define SPLL_B1     -66.6027648f
#define Detect_Time 0.1f

//保护
#define VoltMax  (SQRT_2*25.0f)
#define CurrMax  (SQRT_2*2.3f)
#define OVPStartNum 200
#define OIPStartNum 200
#define NIPStartNum 200

//分频
#define VoltFlag (uint8_t)(IT_FRE/VolLoop_FRE)
#define CurrFlag (uint8_t)(IT_FRE/CurLoop_FRE)

//期望
#define FirstVoltRef (10.0f*SQRT_2)

#endif