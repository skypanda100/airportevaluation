#ifndef CTLCONS
#define CTLCONS
enum SourceType{
    /*
     * 月总薄要素
     */
    SUMMARY = 1,

    /*
     * 月总薄极值
     */
    EXTREMUM,

    /*
     * 大陆自动站风
     */
    MAINLAND_AUTOMATICWIND,

    /*
     * 澳门自动站风
     */
    MACAO_AUTOMATICWIND,

    /*
     * 大陆自动站温度气压
     */
    MAINLAND_AUTOMATICTEMPERATURE,

    /*
     * 未知
     */
    UNKNOWN,
};

enum ReturnCode{
    /*
     * 成功
     */
    MSG_SUCCESS = 0,

    /*
     * 警告
     */
    MSG_WARN,

    /*
     * 错误
     */
    MSG_ERROR,

    /*
     * 未定义
     */
    MSG_UNDEFINE,
};

/***东经110度以东***/
const int JAN_DAY_E[11] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int FEB_DAY_E[12] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int MAR_DAY_E[13] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int APR_DAY_E[14] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int MAY_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int JUN_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int JUL_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int AUG_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int SEP_DAY_E[13] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int OCT_DAY_E[13] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int NOV_DAY_E[11] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int DEC_DAY_E[11] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int HALF_DAY_E = 2;
const int WHOLE_DAY_E = 6;
/***东经95度以东~东经110度***/
const int JAN_DAY_M[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int FEB_DAY_M[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int MAR_DAY_M[13] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int APR_DAY_M[14] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int MAY_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int JUN_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int JUL_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int AUG_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int SEP_DAY_M[13] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int OCT_DAY_M[13] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int NOV_DAY_M[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int DEC_DAY_M[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int HALF_DAY_M = 3;
const int WHOLE_DAY_M = 7;
/***东经95度以西***/
const int JAN_DAY_W[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int FEB_DAY_W[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int MAR_DAY_W[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int APR_DAY_W[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int MAY_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int JUN_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int JUL_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int AUG_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int SEP_DAY_W[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int OCT_DAY_W[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int NOV_DAY_W[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int DEC_DAY_W[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int HALF_DAY_W = 4;
const int WHOLE_DAY_W = 8;
/***影响可飞天的气象要素的各个颜色***/
const int COLORS[24] = {0xDC143C, 0x7B68EE, 0xDAA520, 0x00BFFF
                     , 0x5F9EA0, 0x008B8B, 0x2E8B57, 0x32CD32
                     , 0xFAFAD2, 0xE0AAAA, 0xEEE8AA, 0xFFA500
                     , 0xFF8C00, 0xFF4500, 0xCD5C5C, 0xFFB6C1
                     , 0xFF69B4, 0xEE82EE, 0x6A5ACD, 0x6495ED
                     , 0x4682B4, 0xB0E0E6, 0x1E90FF, 0xD2B48C};
#endif // CTLCONS

