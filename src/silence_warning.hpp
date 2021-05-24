#ifndef SCLUK_SILENCE_WARNING_HPP
#define SCLUK_SILENCE_WARNING_HPP

//only works with gcc. for clang you need to substitute every 'GCC' in the literals with 'clang'

#define SILENCE_WARNING(warning, x) \
    SILENCE_WARNING_BEGIN(warning) \
    x \
    SILENCE_WARNING_END

#define SILENCE_WARNING_BEGIN(warning) SILENCE_WARNING_##warning##_BEGIN

#define SILENCE_WARNING_END \
    _Pragma("GCC diagnostic pop");



//every time you need a new warning silenced you have to write a couple lines of code to make it
//work like the following, because preprocessor string concatenation does not work in _Pragma()
#define SILENCE_WARNING_exit_time_destructors_BEGIN \
    SILENCE_WARNING_GENERALISED_BEGIN("GCC diagnostic ignored \"-Wexit-time-destructors\"")

#define SILENCE_WARNING_padded_BEGIN \
    SILENCE_WARNING_GENERALISED_BEGIN("GCC diagnostic ignored \"-Wpadded\"")

#define SILENCE_WARNING_GENERALISED_BEGIN(warning) \
    _Pragma("GCC diagnostic push"); \
    _Pragma(warning);

#endif //SILENCE_WARNING_HPP
