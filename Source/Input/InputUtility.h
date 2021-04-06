/**
 * @brief ì¸óÕ
 * @author ryota.ssk
 */
#ifndef __INPUT_UTILITY_H__
#define __INPUT_UTILITY_H__

#include "InputInterface.h"

//----------
// É}ÉNÉçíËã`
//----------
#define InputUtil(id) GetInputInterface(id)

#define INP_A		InputInterface::BUTTON_A
#define INP_B		InputInterface::BUTTON_B
#define INP_X		InputInterface::BUTTON_X
#define INP_Y		InputInterface::BUTTON_Y
#define INP_UP		InputInterface::BUTTON_UP
#define INP_DOWN	InputInterface::BUTTON_DOWN
#define INP_LEFT	InputInterface::BUTTON_LEFT
#define INP_RIGHT	InputInterface::BUTTON_RIGHT
#define INP_L1		InputInterface::BUTTON_L1
#define INP_L2		InputInterface::BUTTON_L2
#define INP_L3		InputInterface::BUTTON_L3
#define INP_R1		InputInterface::BUTTON_R1
#define INP_R2		InputInterface::BUTTON_R2
#define INP_R3		InputInterface::BUTTON_R3
#define INP_START	InputInterface::BUTTON_START
#define INP_SELECT	InputInterface::BUTTON_SELECT
#define INP_AXIS_LX	InputInterface::AXIS_LX
#define INP_AXIS_LY	InputInterface::AXIS_LY
#define INP_AXIS_LZ	InputInterface::AXIS_LZ
#define INP_AXIS_RX	InputInterface::AXIS_RX
#define INP_AXIS_RY	InputInterface::AXIS_RY
#define INP_AXIS_RZ	InputInterface::AXIS_RZ

//----------
// ä÷êî
//----------
bool InitializeInputUtility(int num);
void TerminateInputUtility();
void UpdateInputUtility();
InputInterface *GetInputInterface(int id);


#endif // __INPUT_UTILITY_H__