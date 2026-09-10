#include "adc.h"

int16 ad_max[4] = {3860,3860,3860,3860};
int16 ad_min[4] = {0,0,0,0};
float ad_left_x,ad_left_y,ad_right_y,ad_right_x;//ADC四个通道的采样值
float ADC_temp0,ADC_temp1,ADC_temp2,ADC_temp3;

uint16 ad_result = 0;           //引脚电压
float battery_voltage = 0;     //电池电压
uint32 temp;
float err_dianci;
float num,den,k;
float LRM_fabs;


/***************************中值平均滤波*********************************
函数：  uint16 Median_Average_Filter(uint16* arr, uint8 times)
功能：  去除最大值和最小值后求平均
参数：  arr     ADC采样数组
参数：  times   采样次数
说明：  用于抑制偶发尖峰干扰，提高ADC稳定性
返回值：滤波后的平均值
************************************************************************/
uint16 Median_Average_Filter(uint16* arr, uint8 times)
{
    uint16 min = arr[0];
    uint16 max = arr[0];
    uint32 sum = 0;
    uint8 i;
    
    if(times <= 2)
        return 0;

    for(i = 0; i < times; i++)
    {
        if(arr[i] < min)
            min = arr[i];

        if(arr[i] > max)
            max = arr[i];

        sum += arr[i];
    }

    return (sum - min - max) / (times - 2);
}
/***************************adc电感采集****************************
函数：  void adc_sampling()  
功能：  电感数据采集
参数：  void
说明：  对四路ADC进行滤波采样
返回值：无
************************************************************************/
void adc_sampling(void)
{
    int j;
    uint16 AD_value0[5],AD_value1[5],AD_value2[5],AD_value3[5];
    
    for(j=0;j<5;j++)
    {
        AD_value0[j]= adc_convert(ADC_CHANNEL0);       
        AD_value1[j]= adc_convert(ADC_CHANNEL1);           
        AD_value2[j]= adc_convert(ADC_CHANNEL2);   
        AD_value3[j]= adc_convert(ADC_CHANNEL3);  
    }
    ADC_temp0 = Median_Average_Filter(AD_value0, 5);
    ADC_temp1 = Median_Average_Filter(AD_value1, 5);
    ADC_temp2 = Median_Average_Filter(AD_value2, 5);
    ADC_temp3 = Median_Average_Filter(AD_value3, 5);
}
/***************************adc归一化函数*********************************
函数：  float Adc_Normalize(float value, float min, float max)
功能：  ADC数据归一化
参数：  value   当前ADC值
参数：  min     ADC最小值
参数：  max     ADC最大值
说明：  将ADC数据映射到1~100范围
返回值：归一化后的结果
************************************************************************/
float Adc_Normalize(float value, float min, float max)
{
    float normalized;

    normalized = (value - min) / (max - min) * 100.0f;

    normalized = func_limit_ab(normalized, 1.0f, 100.0f);

    return normalized;
}

/***************************adc电感归一化*********************************
函数：  void adc_normalization()  
功能：  电感归一化
参数：  void
说明：  将ADC数据映射为0~100
返回值：无
************************************************************************/
void adc_normalization(void)
{
    ad_left_x  = Adc_Normalize(ADC_temp0, ad_min[0], ad_max[0]);
    ad_left_y  = Adc_Normalize(ADC_temp1, ad_min[1], ad_max[1]);
    ad_right_y = Adc_Normalize(ADC_temp2, ad_min[2], ad_max[2]);
    ad_right_x = Adc_Normalize(ADC_temp3, ad_min[3], ad_max[3]);
}
/***************************差比和误差计算*********************************
函数：  void err_update()  
功能：  电感误差计算
参数：  void
说明：  根据四路电感计算循迹偏差
返回值：无
************************************************************************/
void err_update(void)
{
    num = (ad_left_x - ad_right_x) + 2 * (ad_left_y - ad_right_y);
    den = (ad_left_x + ad_right_x) + 2 * (ad_left_y + ad_right_y);

    err_dianci = 100 * (num / den);
    err_dianci = func_limit_ab(err_dianci,-100,100);
}


void adc_update(void)
{
    adc_sampling();
    adc_normalization();
    err_update();
}
