/// Copyright (c) 2020 Viktor Lazarev 
//! @version 0.1
//! @author vaksick@gmail.com 

#define TOKEN_CPP
#undef __TOKEN_HPP__

#define ENUM_NAME_1(x) #x
#define ENUM_NAME_2(x, ...) #x, ENUM_NAME_1(__VA_ARGS__)
#define ENUM_NAME_3(x, ...) #x, ENUM_NAME_2(__VA_ARGS__)
#define ENUM_NAME_4(x, ...) #x, ENUM_NAME_3(__VA_ARGS__)
#define ENUM_NAME_5(x, ...) #x, ENUM_NAME_4(__VA_ARGS__)
#define ENUM_NAME_6(x, ...) #x, ENUM_NAME_5(__VA_ARGS__)
#define ENUM_NAME_7(x, ...) #x, ENUM_NAME_6(__VA_ARGS__)
#define ENUM_NAME_8(x, ...) #x, ENUM_NAME_7(__VA_ARGS__)
#define ENUM_NAME_9(x, ...) #x, ENUM_NAME_8(__VA_ARGS__)
#define ENUM_NAME_10(x, ...) #x, ENUM_NAME_9(__VA_ARGS__)
#define ENUM_NAME_11(x, ...) #x, ENUM_NAME_10(__VA_ARGS__)
#define ENUM_NAME_12(x, ...) #x, ENUM_NAME_11(__VA_ARGS__)
#define ENUM_NAME_13(x, ...) #x, ENUM_NAME_12(__VA_ARGS__)
#define ENUM_NAME_14(x, ...) #x, ENUM_NAME_13(__VA_ARGS__)
#define ENUM_NAME_15(x, ...) #x, ENUM_NAME_14(__VA_ARGS__)
#define ENUM_NAME_16(x, ...) #x, ENUM_NAME_15(__VA_ARGS__)
#define ENUM_NAME_17(x, ...) #x, ENUM_NAME_16(__VA_ARGS__)
#define ENUM_NAME_18(x, ...) #x, ENUM_NAME_17(__VA_ARGS__)
#define ENUM_NAME_19(x, ...) #x, ENUM_NAME_18(__VA_ARGS__)
#define ENUM_NAME_20(x, ...) #x, ENUM_NAME_19(__VA_ARGS__)
#define ENUM_NAME_21(x, ...) #x, ENUM_NAME_20(__VA_ARGS__)
#define ENUM_NAME_22(x, ...) #x, ENUM_NAME_21(__VA_ARGS__)
#define ENUM_NAME_23(x, ...) #x, ENUM_NAME_22(__VA_ARGS__)
#define ENUM_NAME_24(x, ...) #x, ENUM_NAME_23(__VA_ARGS__)
#define ENUM_NAME_25(x, ...) #x, ENUM_NAME_24(__VA_ARGS__)
#define ENUM_NAME_26(x, ...) #x, ENUM_NAME_25(__VA_ARGS__)
#define ENUM_NAME_27(x, ...) #x, ENUM_NAME_26(__VA_ARGS__)
#define ENUM_NAME_28(x, ...) #x, ENUM_NAME_27(__VA_ARGS__)
#define ENUM_NAME_29(x, ...) #x, ENUM_NAME_28(__VA_ARGS__)
#define ENUM_NAME_30(x, ...) #x, ENUM_NAME_29(__VA_ARGS__)
#define ENUM_NAME_31(x, ...) #x, ENUM_NAME_30(__VA_ARGS__)
#define ENUM_NAME_32(x, ...) #x, ENUM_NAME_31(__VA_ARGS__)
#define ENUM_NAME_33(x, ...) #x, ENUM_NAME_32(__VA_ARGS__)

#define ENUM_NAME_34(x, ...) #x, ENUM_NAME_33(__VA_ARGS__)
#define ENUM_NAME_35(x, ...) #x, ENUM_NAME_34(__VA_ARGS__)
#define ENUM_NAME_36(x, ...) #x, ENUM_NAME_35(__VA_ARGS__)
#define ENUM_NAME_37(x, ...) #x, ENUM_NAME_36(__VA_ARGS__)
#define ENUM_NAME_38(x, ...) #x, ENUM_NAME_37(__VA_ARGS__)
#define ENUM_NAME_39(x, ...) #x, ENUM_NAME_38(__VA_ARGS__)
#define ENUM_NAME_40(x, ...) #x, ENUM_NAME_39(__VA_ARGS__)
#define ENUM_NAME_41(x, ...) #x, ENUM_NAME_40(__VA_ARGS__)
#define ENUM_NAME_42(x, ...) #x, ENUM_NAME_41(__VA_ARGS__)
#define ENUM_NAME_43(x, ...) #x, ENUM_NAME_42(__VA_ARGS__)

#include "token.hpp"
