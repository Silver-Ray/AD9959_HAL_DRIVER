#include "ad9959.h"

// Forced IO definitions
driverIO SDIO0 = {SDIO0_GPIO_Port, SDIO0_Pin};
driverIO SDIO1 = {SDIO1_GPIO_Port, SDIO1_Pin};
driverIO SDIO2 = {SDIO2_GPIO_Port, SDIO2_Pin};
driverIO SDIO3 = {SDIO3_GPIO_Port, SDIO3_Pin};
driverIO PDC = {PDC_GPIO_Port, PDC_Pin};
driverIO RST = {RESET_GPIO_Port, RESET_Pin};
driverIO SCLK = {SCLK_GPIO_Port, SCLK_Pin};
driverIO CS = {CS_GPIO_Port, CS_Pin};
driverIO UPDATE = {UPDATE_GPIO_Port, UPDATE_Pin};
driverIO PS0 = {PS0_GPIO_Port, PS0_Pin};
driverIO PS1 = {PS1_GPIO_Port, PS1_Pin};
driverIO PS2 = {PS2_GPIO_Port, PS2_Pin};
driverIO PS3 = {PS3_GPIO_Port, PS3_Pin};

uint8_t CSR_DATA0[1] = {0x10}; // Enable CH0
uint8_t CSR_DATA1[1] = {0x20}; // Enable CH1
uint8_t CSR_DATA2[1] = {0x40}; // Enable CH2
uint8_t CSR_DATA3[1] = {0x80}; // Enable CH3

uint8_t CSR_DATA[4] = {0x10, 0x20, 0x40, 0x80}; // Enable CH0,CH1,CH2,CH3

uint8_t FR2_DATA[2] = {0x00, 0x00}; // default Value = 0x0000

/*
uint8_t LSRR_DATA[2] = {0x00, 0x00};            // default Value = 0x----
uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // default Value = 0x--------
uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // default Value = 0x--------
*/

// uint32_t SinFre[4] = {10000000, 10000000, 200000000, 40000};
uint32_t SinFre[4] = {1000, 1000, 1000, 1000};
uint32_t SinAmp[4] = {1023, 1023, 1023, 1023};
// uint32_t SinPhr[4] = {0, 4095, 4095 * 3, 4095 * 2};
uint32_t SinPhr[4] = {0, 0, 0, 0};

/**
 * @brief Initializes the AD9959 module.
 */
void Init_AD9959(void)
{
    // GPIO need to be initialized before calling this function

    uint8_t FR1_DATA[3] = {0xD3, 0x00, 0x00}; // 20 frequency doubling
    uint8_t CFR_DATA[3] = {0x00, 0x03, 0x00}; // default Value = 0x000302
    InitIO_9959();
    InitReset();

    WriteData_AD9959(FR1_ADD, 3, FR1_DATA);
    WriteData_AD9959(CFR_ADD, 3, CFR_DATA);

    Write_Phase(3, SinPhr[3]);
    Write_Phase(0, SinPhr[0]);
    Write_Phase(1, SinPhr[1]);
    Write_Phase(2, SinPhr[2]);

    Write_Frequence(0, SinFre[0]);
    Write_Frequence(1, SinFre[1]);
    Write_Frequence(2, SinFre[2]);
    Write_Frequence(3, SinFre[3]);

    Write_Amplitude(3, SinAmp[3]);
    Write_Amplitude(0, SinAmp[0]);
    Write_Amplitude(1, SinAmp[1]);
    Write_Amplitude(2, SinAmp[2]);
}

/**
 * @brief Delays the execution for a specified amount of time.
 *
 * @param length The length of the delay in loop.
 */
void delay_9959(uint32_t length)
{
    length = length * 12;
    while (length--)
        ;
}

/**
 * @brief Initializes the IO for AD9959.
 */
void InitIO_9959(void)
{

    WRT(PDC, 0);
    WRT(CS, 1);
    WRT(SCLK, 0);
    WRT(UPDATE, 0);
    WRT(PS0, 0);
    WRT(PS1, 0);
    WRT(PS2, 0);
    WRT(PS3, 0);
    WRT(SDIO0, 0);
    WRT(SDIO1, 0);
    WRT(SDIO2, 0);
    WRT(SDIO3, 0);
}

void InitReset(void)
{
    WRT(RST, 0);
    delay_9959(1);
    WRT(RST, 1);
    delay_9959(30);
    WRT(RST, 0);
}

/**
 * @brief Updates the IO state of the AD9959 device.
 *
 * This function is responsible for updating the IO state of the AD9959 device.
 * It triggers the IO update operation to ensure that any pending changes in the
 * control registers are applied to the device.
 */
void AD9959_IO_Update(void)
{
    WRT(UPDATE, 1);
    delay_9959(1);
    WRT(UPDATE, 0);
}

/**
 * @brief Writes data to the AD9959 device.
 *
 * This function writes data to the specified register address of the AD9959 device.
 *
 * @param RegisterAddress The address of the register to write data to.
 * @param NumberofRegisters The number of registers to write data to.
 * @param RegisterData Pointer to the data to be written to the registers.
 * @param update Flag indicating whether to update the device after writing data.
 *               Set to 1 to update the device, or 0 to skip the update.
 */
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData)
{
    uint8_t ControlValue = 0;
    uint8_t ValueToWrite = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;

    ControlValue = RegisterAddress;
    // Write address
    WRT(SCLK, 0);
    WRT(CS, 0);
    for (i = 0; i < 8; i++)
    {
        WRT(SCLK, 0);
        if (0x80 == (ControlValue & 0x80))
            WRT(SDIO0, 1);
        else
            WRT(SDIO0, 0);
        WRT(SCLK, 1);
        ControlValue <<= 1;
        delay_9959(2);
    }

    WRT(SCLK, 0);
    // Write data
    // 每个字节的数据都是从高位开始传输
    // 第一个传输的字节是最高位的字节
    for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
    {
        ValueToWrite = RegisterData[RegisterIndex];
        for (i = 0; i < 8; i++)
        {
            WRT(SCLK, 0);
            if (0x80 == (ValueToWrite & 0x80))
                WRT(SDIO0, 1);
            else
                WRT(SDIO0, 0);
            WRT(SCLK, 1);
            ValueToWrite <<= 1;
            delay_9959(2);
        }
        WRT(SCLK, 0);
    }
    WRT(CS, 1);
}

/**
 * @brief Writes the frequency value to the specified channel.
 *
 * @param Channel The channel number to write the frequency to.(0 to 3)
 * @param Freq The frequency value to be written.(1 to 500000000)
 */
void Write_Frequence(uint8_t Channel, uint32_t Freq)
{
    if (Freq > 500000000 || Freq < 1)
    {
        Freq = 114514;
        AD9959_error();
    }

    uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};
    Freq2Word(Freq, CFTW0_DATA);
    Channel_Select(Channel);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA); // CTW0 address 0x04.Output CH0 setting frequency
}

/**
 * @brief Writes the amplitude value for a specific channel.
 *
 * @param Channel The channel number.(0 to 3)
 * @param Ampli The amplitude value to be written.(0 to 1023)
 */
void Write_Amplitude(uint8_t Channel, uint16_t Ampli)
{
    // Ampli的取值在0到1023之间
    if (Ampli > 1023 || Ampli < 0)
    {
        Ampli = 114;
        AD9959_error();
    }
    uint8_t ACR_DATA[3] = {0x00, 0x00, 0x00}; // default Value = 0x--0000 Rest = 18.91/Iout
    Amp2Word(Ampli, ACR_DATA);
    Channel_Select(Channel);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA);
}

/**
 * @brief Writes the phase value for a specific channel.
 *
 * @param Channel The channel number.(0 to 3)
 * @param Phase The phase value(degree) to be written.(0 to 359)
 */
void Write_Phase(uint8_t Channel, uint16_t Phase)
{
    // Phase_max = 16383
    if (Phase > 359 || Phase < 0)
    {
        Phase = 0;
        AD9959_error();
    }
    uint8_t CPOW0_DATA[2] = {0x00, 0x00};
    Phase2Word(Phase, CPOW0_DATA);
    Channel_Select(Channel);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA);
}

/**
 * @brief  Select the channel
 * @name  Channel_Select
 * @param  channel
 * @retval None
 */
void Channel_Select(uint8_t Channel)
{
    if (Channel < 0 || Channel > 3)
    {
        AD9959_error();
        return;
    }
    else
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA + Channel);
}

/**
 * @brief  AD9959 Error handler
 * CAN BE MODIFIED
 * @note
 * @name  AD9959_error
 * @param  None
 * @retval None
 */
void AD9959_error(void)
{
}

/**
 * @brief Performs a frequency sweep on the AD9959 device.
 *
 * @param Channel The channel number to perform the sweep on.
 * @param Start_Freq The starting frequency of the sweep.
 * @param Stop_Freq The ending frequency of the sweep.
 * @param Step The frequency step size for each iteration of the sweep.
 * @param time The duration of each frequency step in microsecond.(1-2048)j
 * @param NO_DWELL
 */
void Sweep_Frequency(uint8_t Channel, uint32_t Start_Freq, uint32_t Stop_Freq, uint32_t Step, uint32_t time, uint8_t NO_DWELL)
{
    Channel_Select(Channel);
    switch (Channel)
    {
    case 0:
        WRT(PS0, 0);
    case 1:
        WRT(PS1, 0);
    case 2:
        WRT(PS2, 0);
    case 3:
        WRT(PS3, 0);
    }

    uint8_t CFR_GET[3] = {0x00, 0x00, 0x00};
    ReadData_AD9959(CFR_ADD, 3, CFR_GET);
    uint8_t CFR_DATA_Freq[3] = {CFR_GET[0] | 0x80, CFR_GET[1] | 0x40, CFR_GET[2] | 0x00};
    // Disable no-dwell bit by writing 0 to CFR[13]
    if (NO_DWELL)
        CFR_DATA_Freq[1] = CFR_DATA_Freq[1] | 0x80;

    uint8_t FR1_DATA_sweep[3] = {0xD3, 0x00, 0x00};
    WriteData_AD9959(CFR_ADD, 3, CFR_DATA_Freq);
    WriteData_AD9959(FR1_ADD, 3, FR1_DATA_sweep);

    uint32_t Start_Freq_word, Stop_Freq_Word, Step_Word;
    uint8_t Time_word;
    Start_Freq_word = (uint32_t)Start_Freq * 8.589934592;
    Stop_Freq_Word = (uint32_t)Stop_Freq * 8.589934592;
    Step_Word = (uint32_t)Step * 8.589934592;
    Time_word = (uint8_t)time / 8.0; // ns

    uint8_t CFTW0_DATA_START[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t CFTW0_DATA_STOP[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};

    CFTW0_DATA_START[3] = (uint8_t)Start_Freq_word;
    CFTW0_DATA_START[2] = (uint8_t)(Start_Freq_word >> 8);
    CFTW0_DATA_START[1] = (uint8_t)(Start_Freq_word >> 16);
    CFTW0_DATA_START[0] = (uint8_t)(Start_Freq_word >> 24);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA_START);

    CFTW0_DATA_STOP[3] = (uint8_t)Stop_Freq_Word;
    CFTW0_DATA_STOP[2] = (uint8_t)(Stop_Freq_Word >> 8);
    CFTW0_DATA_STOP[1] = (uint8_t)(Stop_Freq_Word >> 16);
    CFTW0_DATA_STOP[0] = (uint8_t)(Stop_Freq_Word >> 24);
    WriteData_AD9959(CW1, 4, CFTW0_DATA_STOP);

    RDW_DATA[3] = (uint8_t)Step_Word;
    RDW_DATA[2] = (uint8_t)(Step_Word >> 8);
    RDW_DATA[1] = (uint8_t)(Step_Word >> 16);
    RDW_DATA[0] = (uint8_t)(Step_Word >> 24);
    WriteData_AD9959(RDW_ADD, 4, RDW_DATA);

    uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};
    FDW_DATA[3] = (uint8_t)Step_Word;
    FDW_DATA[2] = (uint8_t)(Step_Word >> 8);
    FDW_DATA[1] = (uint8_t)(Step_Word >> 16);
    FDW_DATA[0] = (uint8_t)(Step_Word >> 24);
    WriteData_AD9959(FDW_ADD, 4, FDW_DATA);

    uint8_t LSRR_DATA[2] = {0x00, 0x00};
    LSRR_DATA[1] = (uint8_t)Time_word;
    LSRR_DATA[0] = (uint8_t)Time_word;
    WriteData_AD9959(LSRR_ADD, 2, LSRR_DATA); // UPDATE

    switch (Channel)
    {
    case 0:
        WRT(PS0, 1);
    case 1:
        WRT(PS1, 1);
    case 2:
        WRT(PS2, 1);
    case 3:
        WRT(PS3, 1);
    }
}

uint32_t Get_Freq(void)
{
    uint8_t data[4] = {0};
    ReadData_AD9959(CFTW0_ADD, 4, data);
    int Freq = (data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]) / 8.589934592;
    return Freq;
}

uint8_t Get_Amp(void)
{
    uint8_t data[3] = {0};
    ReadData_AD9959(ACR_ADD, 3, data);
    return data[2];
}

void ReadData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData)
{
    uint8_t ControlValue = 0;
    uint8_t ValueToRead = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;
    ControlValue = RegisterAddress | 0x80;
    // Write address
    WRT(CS, 0);
    WRT(SCLK, 0);
    delay_9959(0x20);
    for (i = 0; i < 8; i++)
    {
        WRT(SCLK, 0);
        if (0x80 == (ControlValue & 0x80))
            WRT(SDIO0, 1);
        else
            WRT(SDIO0, 0);
        WRT(SCLK, 1);
        ControlValue <<= 1;
        delay_9959(2);
    }

    WRT(SCLK, 0);

    // Input mode
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SDIO0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SDIO0_GPIO_Port, &GPIO_InitStruct);
    delay_9959(0x20);
    // Read data
    // MSB first
    for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
    {
        ValueToRead = 0;
        for (i = 0; i < 8; i++)
        {
            ValueToRead <<= 1;
            WRT(SCLK, 1);
            if (GET(SDIO0) == 1)
                ValueToRead |= 0x01;
            else
                ValueToRead &= 0xFE;
            WRT(SCLK, 0);
        }
        WRT(SCLK, 0);
        RegisterData[RegisterIndex] = ValueToRead;
    }
    delay_9959(0x20);
    WRT(CS, 1);
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(SDIO0_GPIO_Port, &GPIO_InitStruct);
}

void Stop_AD9959(void)
{
    uint8_t CFR_data[3] = {0};
    ReadData_AD9959(CFTW0_ADD, 3, CFR_data);
    CFR_data[0] = CFR_data[0] | 0x02;
    WriteData_AD9959(CFR_ADD, 3, CFR_data);
}

/**
 * @brief Set the 2fsk object
 *
 * @param Channel The channel number to write the frequency to.(0 to 3)
 * @param f_start The start frequency value(Hz) to be written when profile pin is reset.
 * @param f_stop The stop frequency value(Hz) to be written when profile pin is set.
 * @return * void
 */
void SET_2FSK(uint8_t Channel, double f_start, double f_stop)
{
    uint8_t f_startWord[4];
    uint8_t f_stopWord[4];
    uint8_t CFR_data[3] = {0x80, 0x23, 0x30}; // none RU/RD
    uint8_t FR_data[3] = {0xD0, 0x00, 0x00};

    Channel_Select(Channel);

    WriteData_AD9959(FR1_ADD, 3, FR_data);
    WriteData_AD9959(CFR_ADD, 3, CFR_data);

    Freq2Word(f_start, f_startWord);
    Freq2Word(f_stop, f_stopWord);

    WriteData_AD9959(CFTW0_ADD, 4, f_startWord);
    WriteData_AD9959(CW1, 4, f_stopWord);
}

/**
 * @brief Set the 2ask object
 *
 * @param Channel The channel number to write the frequency to.(0 to 3)
 * @param f The frequency value(Hz) to be written.(1 to 500000000)
 * @param A_start The start amplitude value to be written when profile pin is reset.(0 to 1023)
 * @param A_stop The stop amplitude value to be written when profile pin is set.(0 to 1023)
 * @return * void
 */
void SET_2ASK(uint8_t Channel, double f, uint16_t A_start, uint16_t A_stop)
{
    uint8_t fWord[4];
    uint8_t A_startWord[3] = {0x00, 0x00, 0x00};
    uint8_t A_stopWord[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t A_stopWord_temp[3] = {0x00, 0x00, 0x00};

    uint8_t CFR_data[3] = {0x40, 0x03, 0x30};
    uint8_t FR_data[3] = {0xD0, 0x00, 0x00};

    Amp2Word(A_start, A_startWord);
    Amp2Word(A_stop, A_stopWord_temp);

    // A_stopWord_teep[9:0]->A_stopWord[31:22]
    uint32_t Temp = ((uint32_t)(A_stopWord_temp[1] | 0x03)) << 8 | (uint32_t)(A_stopWord_temp[2]);
    Temp = Temp << 22;
    A_stopWord[0] = (uint8_t)(Temp >> 24);
    A_stopWord[1] = (uint8_t)(Temp >> 22);

    Channel_Select(Channel);

    WriteData_AD9959(FR1_ADD, 3, FR_data);
    WriteData_AD9959(CFR_ADD, 3, CFR_data);

    WriteData_AD9959(ACR_ADD, 3, A_startWord);
    WriteData_AD9959(CW1, 4, A_stopWord);

    Freq2Word(f, fWord);
    WriteData_AD9959(CFTW0_ADD, 4, fWord);
}

void Freq2Word(double f, uint8_t *fWord)
{
    // fWord 4 bytes
    uint32_t Temp;
    Temp = (uint32_t)f * 8.589934592; // The input frequency factor is divided into four bytes.  8.589934592=(2^32)/500000000
    fWord[3] = (uint8_t)Temp;
    fWord[2] = (uint8_t)(Temp >> 8);
    fWord[1] = (uint8_t)(Temp >> 16);
    fWord[0] = (uint8_t)(Temp >> 24);
}

void Amp2Word(uint16_t A, uint8_t *AWord)
{
    // AWord 3 bytes
    uint16_t Temp;
    Temp = (uint16_t)A | 0x1000; // Enable amplitude multiplier
    AWord[2] = (uint8_t)Temp;
    AWord[1] = (uint8_t)(Temp >> 8);
    AWord[0] = 0x00;
}

void Phase2Word(uint16_t Phase, uint8_t *PWord)
{
    // PWord 2 bytes
    uint16_t Temp = (uint16_t)(45.511111111 * Phase); // 360/2^16 = 45.511111111
    PWord[1] = (uint8_t)Temp;
    PWord[0] = (uint8_t)(Temp >> 8);
}
