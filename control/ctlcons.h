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
#endif // CTLCONS

