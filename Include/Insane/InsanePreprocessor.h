#pragma once
#ifndef INSANE_PREPROCESSOR_H
    #define INSANE_PREPROCESSOR_H

    #include <map>
    #define NODISCARD_ATTRIB [[nodiscard]]
    #define MAYBE_UNUSED_ATTRIB [[maybe_unused]]

    #define STDCALL _stdcall
    #define CDECL _cdecl
    #define EXTERNC extern "C"

    // WINDOWS
    #if (_WIN32)
        #define NOMINMAX
        #include <Windows.h>
        #include <conio.h>
        #if _WIN64
            #define IS64 1
        #else
            #define IS32 1
        #endif
        #define WINDOWS_PLATFORM 1
        #define DLLCALL STDCALL
        #define DLLIMPORT _declspec(dllimport)
        #define DLLEXPORT _declspec(dllexport)
        #define DLLPRIVATE

    // EMSCRIPTEN
    #elif (__EMSCRIPTEN__)
        #include <emscripten/bind.h>
        #include <emscripten/emscripten.h>
        #include <termios.h>
        #include <unistd.h>
        #if (__SIZEOF_POINTER__ == 4)
            #define IS32 1
        #else
            #define IS64 1
        #endif
        #define EMSCRIPTEN_PLATFORM 1
        #define DLLCALL
        #define DLLIMPORT
        #define DLLEXPORT __attribute__((visibility("default")))
        #define DLLPRIVATE __attribute__((visibility("hidden")))

    // ANDROID
    #elif (__ANDROID__ || ANDROID)
        #include <termios.h>
        #include <unistd.h>
        #if (_LP64_ || _LP64)
            #define IS64 1
        #else
            #define IS32 1
        #endif
        #define ANDROID_PLATFORM 1
        #define DLLCALL
        #define DLLIMPORT
        #define DLLEXPORT __attribute__((visibility("default")))
        #define DLLPRIVATE __attribute__((visibility("hidden")))

    // LINUX - Ubuntu, Fedora, , Centos, Debian, RedHat
    #elif (__LINUX__ || __gnu_linux__ || __linux__ || __linux || linux)
        #define LINUX_PLATFORM 1
        #include <termios.h>
        #include <unistd.h>

        #define DLLCALL CDECL
        #define DLLIMPORT
        #define DLLEXPORT __attribute__((visibility("default")))
        #define DLLPRIVATE __attribute__((visibility("hidden")))
        #define CoTaskMemAlloc(p) malloc(p)
        #define CoTaskMemFree(p) free(p)
        #if (_LP64_ || _LP64)
            #define IS64 1
        #else
            #define IS32 1
        #endif

    // MACOS
    #elif defined(__APPLE__)
        #include <termios.h>
        #include <unistd.h>

        #define DLLCALL
        #define DLLIMPORT
        #define DLLEXPORT __attribute__((visibility("default")))
        #define DLLPRIVATE __attribute__((visibility("hidden")))
        #include "TargetConditionals.h"
        #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
            #define IOS_SIMULATOR_PLATFORM 1
        #elif TARGET_OS_IPHONE
            #define IOS_PLATFORM 1
        #elif TARGET_OS_MAC
            #define MACOS_PLATFORM 1
        #else

        #endif

    #endif

    #ifdef INSANE_EXPORTS
        #define INSANE_API DLLEXPORT
    #else
        #define INSANE_API DLLIMPORT
    #endif

    #if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
        #define INSANE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
    #elif defined(_MSC_VER)
        #define INSANE_FUNCTION_SIGNATURE __FUNCSIG__
    #elif defined(__IBMCPP__) || defined(__xlC__)
        #define INSANE_FUNCTION_SIGNATURE __FUNCTION__
    #elif defined(__INTEL_COMPILER)
        #define INSANE_FUNCTION_SIGNATURE __FUNCTION__
    #elif defined(__MINGW32__) || defined(__MINGW64__)
        #define INSANE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
    #elif defined(__BORLANDC__)
        #define INSANE_FUNCTION_SIGNATURE __FUNC__
    #elif defined(__HP_aCC)
        #define INSANE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
    #else
        static_assert(false, "INSANE_FUNCTION_SIGNATURE macro not defined for unknown compiler");
    #endif

    #define INSANE_REPEAT_MAX 512

    #define FIVE_HUNDRED_AND_FOURTEENTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127, a128, a129, a130, a131, a132, a133, a134, a135, a136, a137, a138, a139, a140, a141, a142, a143, a144, a145, a146, a147, a148, a149, a150, a151, a152, a153, a154, a155, a156, a157, a158, a159, a160, a161, a162, a163, a164, a165, a166, a167, a168, a169, a170, a171, a172, a173, a174, a175, a176, a177, a178, a179, a180, a181, a182, a183, a184, a185, a186, a187, a188, a189, a190, a191, a192, a193, a194, a195, a196, a197, a198, a199, a200, a201, a202, a203, a204, a205, a206, a207, a208, a209, a210, a211, a212, a213, a214, a215, a216, a217, a218, a219, a220, a221, a222, a223, a224, a225, a226, a227, a228, a229, a230, a231, a232, a233, a234, a235, a236, a237, a238, a239, a240, a241, a242, a243, a244, a245, a246, a247, a248, a249, a250, a251, a252, a253, a254, a255, a256, a257, a258, a259, a260, a261, a262, a263, a264, a265, a266, a267, a268, a269, a270, a271, a272, a273, a274, a275, a276, a277, a278, a279, a280, a281, a282, a283, a284, a285, a286, a287, a288, a289, a290, a291, a292, a293, a294, a295, a296, a297, a298, a299, a300, a301, a302, a303, a304, a305, a306, a307, a308, a309, a310, a311, a312, a313, a314, a315, a316, a317, a318, a319, a320, a321, a322, a323, a324, a325, a326, a327, a328, a329, a330, a331, a332, a333, a334, a335, a336, a337, a338, a339, a340, a341, a342, a343, a344, a345, a346, a347, a348, a349, a350, a351, a352, a353, a354, a355, a356, a357, a358, a359, a360, a361, a362, a363, a364, a365, a366, a367, a368, a369, a370, a371, a372, a373, a374, a375, a376, a377, a378, a379, a380, a381, a382, a383, a384, a385, a386, a387, a388, a389, a390, a391, a392, a393, a394, a395, a396, a397, a398, a399, a400, a401, a402, a403, a404, a405, a406, a407, a408, a409, a410, a411, a412, a413, a414, a415, a416, a417, a418, a419, a420, a421, a422, a423, a424, a425, a426, a427, a428, a429, a430, a431, a432, a433, a434, a435, a436, a437, a438, a439, a440, a441, a442, a443, a444, a445, a446, a447, a448, a449, a450, a451, a452, a453, a454, a455, a456, a457, a458, a459, a460, a461, a462, a463, a464, a465, a466, a467, a468, a469, a470, a471, a472, a473, a474, a475, a476, a477, a478, a479, a480, a481, a482, a483, a484, a485, a486, a487, a488, a489, a490, a491, a492, a493, a494, a495, a496, a497, a498, a499, a500, a501, a502, a503, a504, a505, a506, a507, a508, a509, a510, a511, a512, a513, a514, ...) a514
    #define COUNT_ARGUMENTS(...) FIVE_HUNDRED_AND_FOURTEENTH_ARGUMENT(dummy, __VA_ARGS__, 512, 511, 510, 509, 508, 507, 506, 505, 504, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488, 487, 486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469, 468, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 451, 450, 449, 448, 447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428, 427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408, 407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380, 379, 378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368, 367, 366, 365, 364, 363, 362, 361, 360, 359, 358, 357, 356, 355, 354, 353, 352, 351, 350, 349, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 338, 337, 336, 335, 334, 333, 332, 331, 330, 329, 328, 327, 326, 325, 324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307, 306, 305, 304, 303, 302, 301, 300, 299, 298, 297, 296, 295, 294, 293, 292, 291, 290, 289, 288, 287, 286, 285, 284, 283, 282, 281, 280, 279, 278, 277, 276, 275, 274, 273, 272, 271, 270, 269, 268, 267, 266, 265, 264, 263, 262, 261, 260, 259, 258, 257, 256, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

    #define EXPAND_VALUE(x) x
    #define EXPAND_EXPRESSION(x) x()

    #define COMMA_VALUE_EXP() ,
    #define EMPTY_VALUE_EXP()
    #define EQUALS_VALUE_EXP() =
    #define SPACE_VALUE_EXP() EXPAND_VALUE() EXPAND_VALUE()
    #define SEMICOLON_VALUE_EXP() ;
    #define COLON_VALUE_EXP() :
    #define HYPHEN_VALUE_EXP() -
    #define UNDERSCORE_VALUE_EXP() _

    #define STRINGIFY(x) #x##s
    #define CSTRINGIFY(x) #x

    #define STRINGIFY_DEFER(x) STRINGIFY(x)
    #define CSTRINGIFY_DEFER(x) CSTRINGIFY(x)

    #define MERGE_EXPAND(a, b) a##b
    #define MERGE_VALUE(a, b) MERGE_EXPAND(a, b)

    #define ADD_EXPRESSION_0(exp)
    #define ADD_EXPRESSION_1(exp) exp()
    #define REMOVE_EXPRESSION_0(exp) exp()
    #define REMOVE_EXPRESSION_1(exp)

    #define ADD_VALUE_0(value)
    #define ADD_VALUE_1(value) value
    #define REMOVE_VALUE_0(value) value
    #define REMOVE_VALUE_1(value)

    #define ADD_COMMA_0()
    #define ADD_COMMA_1() ,
    #define REMOVE_COMMA_0()
    #define REMOVE_COMMA_1() ,

    #define VOID_TYPE_EXP() void
    #define STRING_TYPE_EXP() String
    #define INT32_TYPE_EXP() SignedInt32
    #define Int64_TYPE_EXP() SignedInt64

    #define EMOJI_UPWARDS_BUTTON_STRING ("🔼"s)
    #define EMOJI_DOWNWARDS_BUTTON_STRING ("🔽"s)
    #define EMOJI_SQUARE_VS_STRING ("🆚"s)
    #define EMOJI_RED_TRIANGLE_POINTED_UP_STRING ("🔺"s)
    #define EMOJI_RED_TRIANGLE_POINTED_DOWN_STRING ("🔻"s)

    #define EMPTY_STRING (""s)
    #define NULL_STRING ("\000"s)
    #define CR_STRING ("\r"s)
    #define CARRIAGE_RETURN_STRING ("\r"s)
    #define LF_STRING ("\n"s)
    #define LINE_FEED_STRING ("\n"s)
    #define CRLF_STRING ("\r\n"s)
    #define TAB_STRING ("\t"s)
    #define VERTICAL_TAB_STRING ("\v"s)
    #define FORM_FEED_STRING ("\f"s)
    #define QUOTATION_MARK_STRING ("\""s)
    #define UNDERSCORE_STRING ("_"s)
    #define SPACE_STRING (" "s)
    #define BACKSLASH_STRING ("\\"s)
    #define FULL_STOP_STRING ("."s)
    #define QUESTION_MARK_STRING ("\?"s)
    #define LESS_THAN_SIGN_STRING ("<"s)
    #define GREATER_THAN_SIGN_STRING (">"s)

    #define TRUE_LITERAL_STRING ("true"s)
    #define FALSE_LITERAL_STRING ("false"s)

    #define URL_ENCODED_PLUS_SIGN_STRING ("%2B"s)
    #define URL_ENCODED_SLASH_STRING ("%2F"s)
    #define URL_ENCODED_EQUAL_SIGN_STRING ("%3D"s)
    #define PLUS_SIGN_STRING ("+"s)
    #define MINUS_SIGN_STRING ("-"s)
    #define SLASH_STRING ("/"s)
    #define UNDERSCORE_STRING ("_"s)
    #define EQUAL_SIGN_STRING ("="s)

    #define EMPTY_CHAR_ARRAY ("")
    #define NULL_CHAR ('\000')
    #define CR_CHAR ('\r')
    #define CARRIAGE_RETURN_CHAR ('\r')
    #define LF_CHAR ('\n')
    #define LINE_FEED_CHAR ('\n')
    #define CRLF_CHAR_ARRAY ("\r\n")
    #define TAB_CHAR ('\t')
    #define VERTICAL_TAB_CHAR ('\v')
    #define FORM_FEED_CHAR ('\f')
    #define QUOTATION_MARK_CHAR ('\"')
    #define UNDERSCORE_CHAR ('_')
    #define SPACE_CHAR (' ')
    #define BACKSLASH_CHAR ('\\')
    #define QUESTION_MARK_CHAR ('\?')

    #define URL_ENCODED_PLUS_SIGN_CHAR_ARRAY ("%2B")
    #define URL_ENCODED_SLASH_CHAR_ARRAY ("%2F")
    #define URL_ENCODED_EQUAL_SIGN_CHAR_ARRAY ("%3D")
    #define PLUS_SIGN_CHAR ('+')
    #define MINUS_SIGN_CHAR ('-')
    #define SLASH_CHAR ('/')
    #define UNDERSCORE_CHAR ('_')
    #define EQUAL_SIGN_CHAR ('=')

    #define DEFAULT_LOCALE_STRING ("en_US"s)
    #define UTF8_CHARSET_NAME_STRING ("UTF-8"s)
    #define ISO_8859_1_CHARSET_NAME ("ISO-8859-1"s)
    #define VA_ARGS_SEPARATOR_STRING (", "s)
    #define NAMESPACE_SEPARATOR_STRING ("::"s)

    #ifdef WINDOWS_PLATFORM
        #define NEW_LINE_STRING ("\r\n"s)
    #elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
        #define NEW_LINE_STR ("\n"s)
    #endif

    #define ASCII_NEGATIVE_128_OCTAL_STRING ("\200"s)
    #define ASCII_NEGATIVE_127_OCTAL_STRING ("\201"s)
    #define ASCII_NEGATIVE_126_OCTAL_STRING ("\202"s)
    #define ASCII_NEGATIVE_125_OCTAL_STRING ("\203"s)
    #define ASCII_NEGATIVE_124_OCTAL_STRING ("\204"s)
    #define ASCII_NEGATIVE_123_OCTAL_STRING ("\205"s)
    #define ASCII_NEGATIVE_122_OCTAL_STRING ("\206"s)
    #define ASCII_NEGATIVE_121_OCTAL_STRING ("\207"s)
    #define ASCII_NEGATIVE_120_OCTAL_STRING ("\210"s)
    #define ASCII_NEGATIVE_119_OCTAL_STRING ("\211"s)
    #define ASCII_NEGATIVE_118_OCTAL_STRING ("\212"s)
    #define ASCII_NEGATIVE_117_OCTAL_STRING ("\213"s)
    #define ASCII_NEGATIVE_116_OCTAL_STRING ("\214"s)
    #define ASCII_NEGATIVE_115_OCTAL_STRING ("\215"s)
    #define ASCII_NEGATIVE_114_OCTAL_STRING ("\216"s)
    #define ASCII_NEGATIVE_113_OCTAL_STRING ("\217"s)
    #define ASCII_NEGATIVE_112_OCTAL_STRING ("\220"s)
    #define ASCII_NEGATIVE_111_OCTAL_STRING ("\221"s)
    #define ASCII_NEGATIVE_110_OCTAL_STRING ("\222"s)
    #define ASCII_NEGATIVE_109_OCTAL_STRING ("\223"s)
    #define ASCII_NEGATIVE_108_OCTAL_STRING ("\224"s)
    #define ASCII_NEGATIVE_107_OCTAL_STRING ("\225"s)
    #define ASCII_NEGATIVE_106_OCTAL_STRING ("\226"s)
    #define ASCII_NEGATIVE_105_OCTAL_STRING ("\227"s)
    #define ASCII_NEGATIVE_104_OCTAL_STRING ("\230"s)
    #define ASCII_NEGATIVE_103_OCTAL_STRING ("\231"s)
    #define ASCII_NEGATIVE_102_OCTAL_STRING ("\232"s)
    #define ASCII_NEGATIVE_101_OCTAL_STRING ("\233"s)
    #define ASCII_NEGATIVE_100_OCTAL_STRING ("\234"s)
    #define ASCII_NEGATIVE_99_OCTAL_STRING ("\235"s)
    #define ASCII_NEGATIVE_98_OCTAL_STRING ("\236"s)
    #define ASCII_NEGATIVE_97_OCTAL_STRING ("\237"s)
    #define ASCII_NEGATIVE_96_OCTAL_STRING ("\240"s)
    #define ASCII_NEGATIVE_95_OCTAL_STRING ("\241"s)
    #define ASCII_NEGATIVE_94_OCTAL_STRING ("\242"s)
    #define ASCII_NEGATIVE_93_OCTAL_STRING ("\243"s)
    #define ASCII_NEGATIVE_92_OCTAL_STRING ("\244"s)
    #define ASCII_NEGATIVE_91_OCTAL_STRING ("\245"s)
    #define ASCII_NEGATIVE_90_OCTAL_STRING ("\246"s)
    #define ASCII_NEGATIVE_89_OCTAL_STRING ("\247"s)
    #define ASCII_NEGATIVE_88_OCTAL_STRING ("\250"s)
    #define ASCII_NEGATIVE_87_OCTAL_STRING ("\251"s)
    #define ASCII_NEGATIVE_86_OCTAL_STRING ("\252"s)
    #define ASCII_NEGATIVE_85_OCTAL_STRING ("\253"s)
    #define ASCII_NEGATIVE_84_OCTAL_STRING ("\254"s)
    #define ASCII_NEGATIVE_83_OCTAL_STRING ("\255"s)
    #define ASCII_NEGATIVE_82_OCTAL_STRING ("\256"s)
    #define ASCII_NEGATIVE_81_OCTAL_STRING ("\257"s)
    #define ASCII_NEGATIVE_80_OCTAL_STRING ("\260"s)
    #define ASCII_NEGATIVE_79_OCTAL_STRING ("\261"s)
    #define ASCII_NEGATIVE_78_OCTAL_STRING ("\262"s)
    #define ASCII_NEGATIVE_77_OCTAL_STRING ("\263"s)
    #define ASCII_NEGATIVE_76_OCTAL_STRING ("\264"s)
    #define ASCII_NEGATIVE_75_OCTAL_STRING ("\265"s)
    #define ASCII_NEGATIVE_74_OCTAL_STRING ("\266"s)
    #define ASCII_NEGATIVE_73_OCTAL_STRING ("\267"s)
    #define ASCII_NEGATIVE_72_OCTAL_STRING ("\270"s)
    #define ASCII_NEGATIVE_71_OCTAL_STRING ("\271"s)
    #define ASCII_NEGATIVE_70_OCTAL_STRING ("\272"s)
    #define ASCII_NEGATIVE_69_OCTAL_STRING ("\273"s)
    #define ASCII_NEGATIVE_68_OCTAL_STRING ("\274"s)
    #define ASCII_NEGATIVE_67_OCTAL_STRING ("\275"s)
    #define ASCII_NEGATIVE_66_OCTAL_STRING ("\276"s)
    #define ASCII_NEGATIVE_65_OCTAL_STRING ("\277"s)
    #define ASCII_NEGATIVE_64_OCTAL_STRING ("\300"s)
    #define ASCII_NEGATIVE_63_OCTAL_STRING ("\301"s)
    #define ASCII_NEGATIVE_62_OCTAL_STRING ("\302"s)
    #define ASCII_NEGATIVE_61_OCTAL_STRING ("\303"s)
    #define ASCII_NEGATIVE_60_OCTAL_STRING ("\304"s)
    #define ASCII_NEGATIVE_59_OCTAL_STRING ("\305"s)
    #define ASCII_NEGATIVE_58_OCTAL_STRING ("\306"s)
    #define ASCII_NEGATIVE_57_OCTAL_STRING ("\307"s)
    #define ASCII_NEGATIVE_56_OCTAL_STRING ("\310"s)
    #define ASCII_NEGATIVE_55_OCTAL_STRING ("\311"s)
    #define ASCII_NEGATIVE_54_OCTAL_STRING ("\312"s)
    #define ASCII_NEGATIVE_53_OCTAL_STRING ("\313"s)
    #define ASCII_NEGATIVE_52_OCTAL_STRING ("\314"s)
    #define ASCII_NEGATIVE_51_OCTAL_STRING ("\315"s)
    #define ASCII_NEGATIVE_50_OCTAL_STRING ("\316"s)
    #define ASCII_NEGATIVE_49_OCTAL_STRING ("\317"s)
    #define ASCII_NEGATIVE_48_OCTAL_STRING ("\320"s)
    #define ASCII_NEGATIVE_47_OCTAL_STRING ("\321"s)
    #define ASCII_NEGATIVE_46_OCTAL_STRING ("\322"s)
    #define ASCII_NEGATIVE_45_OCTAL_STRING ("\323"s)
    #define ASCII_NEGATIVE_44_OCTAL_STRING ("\324"s)
    #define ASCII_NEGATIVE_43_OCTAL_STRING ("\325"s)
    #define ASCII_NEGATIVE_42_OCTAL_STRING ("\326"s)
    #define ASCII_NEGATIVE_41_OCTAL_STRING ("\327"s)
    #define ASCII_NEGATIVE_40_OCTAL_STRING ("\330"s)
    #define ASCII_NEGATIVE_39_OCTAL_STRING ("\331"s)
    #define ASCII_NEGATIVE_38_OCTAL_STRING ("\332"s)
    #define ASCII_NEGATIVE_37_OCTAL_STRING ("\333"s)
    #define ASCII_NEGATIVE_36_OCTAL_STRING ("\334"s)
    #define ASCII_NEGATIVE_35_OCTAL_STRING ("\335"s)
    #define ASCII_NEGATIVE_34_OCTAL_STRING ("\336"s)
    #define ASCII_NEGATIVE_33_OCTAL_STRING ("\337"s)
    #define ASCII_NEGATIVE_32_OCTAL_STRING ("\340"s)
    #define ASCII_NEGATIVE_31_OCTAL_STRING ("\341"s)
    #define ASCII_NEGATIVE_30_OCTAL_STRING ("\342"s)
    #define ASCII_NEGATIVE_29_OCTAL_STRING ("\343"s)
    #define ASCII_NEGATIVE_28_OCTAL_STRING ("\344"s)
    #define ASCII_NEGATIVE_27_OCTAL_STRING ("\345"s)
    #define ASCII_NEGATIVE_26_OCTAL_STRING ("\346"s)
    #define ASCII_NEGATIVE_25_OCTAL_STRING ("\347"s)
    #define ASCII_NEGATIVE_24_OCTAL_STRING ("\350"s)
    #define ASCII_NEGATIVE_23_OCTAL_STRING ("\351"s)
    #define ASCII_NEGATIVE_22_OCTAL_STRING ("\352"s)
    #define ASCII_NEGATIVE_21_OCTAL_STRING ("\353"s)
    #define ASCII_NEGATIVE_20_OCTAL_STRING ("\354"s)
    #define ASCII_NEGATIVE_19_OCTAL_STRING ("\355"s)
    #define ASCII_NEGATIVE_18_OCTAL_STRING ("\356"s)
    #define ASCII_NEGATIVE_17_OCTAL_STRING ("\357"s)
    #define ASCII_NEGATIVE_16_OCTAL_STRING ("\360"s)
    #define ASCII_NEGATIVE_15_OCTAL_STRING ("\361"s)
    #define ASCII_NEGATIVE_14_OCTAL_STRING ("\362"s)
    #define ASCII_NEGATIVE_13_OCTAL_STRING ("\363"s)
    #define ASCII_NEGATIVE_12_OCTAL_STRING ("\364"s)
    #define ASCII_NEGATIVE_11_OCTAL_STRING ("\365"s)
    #define ASCII_NEGATIVE_10_OCTAL_STRING ("\366"s)
    #define ASCII_NEGATIVE_9_OCTAL_STRING ("\367"s)
    #define ASCII_NEGATIVE_8_OCTAL_STRING ("\370"s)
    #define ASCII_NEGATIVE_7_OCTAL_STRING ("\371"s)
    #define ASCII_NEGATIVE_6_OCTAL_STRING ("\372"s)
    #define ASCII_NEGATIVE_5_OCTAL_STRING ("\373"s)
    #define ASCII_NEGATIVE_4_OCTAL_STRING ("\374"s)
    #define ASCII_NEGATIVE_3_OCTAL_STRING ("\375"s)
    #define ASCII_NEGATIVE_2_OCTAL_STRING ("\376"s)
    #define ASCII_NEGATIVE_1_OCTAL_STRING ("\377"s)

    #define ASCII_0_OCTAL_STRING ("\000"s)
    #define ASCII_1_OCTAL_STRING ("\001"s)
    #define ASCII_2_OCTAL_STRING ("\002"s)
    #define ASCII_3_OCTAL_STRING ("\003"s)
    #define ASCII_4_OCTAL_STRING ("\004"s)
    #define ASCII_5_OCTAL_STRING ("\005"s)
    #define ASCII_6_OCTAL_STRING ("\006"s)
    #define ASCII_7_OCTAL_STRING ("\007"s)
    #define ASCII_8_OCTAL_STRING ("\010"s)
    #define ASCII_9_OCTAL_STRING ("\011"s)
    #define ASCII_10_OCTAL_STRING ("\012"s)
    #define ASCII_11_OCTAL_STRING ("\013"s)
    #define ASCII_12_OCTAL_STRING ("\014"s)
    #define ASCII_13_OCTAL_STRING ("\015"s)
    #define ASCII_14_OCTAL_STRING ("\016"s)
    #define ASCII_15_OCTAL_STRING ("\017"s)
    #define ASCII_16_OCTAL_STRING ("\020"s)
    #define ASCII_17_OCTAL_STRING ("\021"s)
    #define ASCII_18_OCTAL_STRING ("\022"s)
    #define ASCII_19_OCTAL_STRING ("\023"s)
    #define ASCII_20_OCTAL_STRING ("\024"s)
    #define ASCII_21_OCTAL_STRING ("\025"s)
    #define ASCII_22_OCTAL_STRING ("\026"s)
    #define ASCII_23_OCTAL_STRING ("\027"s)
    #define ASCII_24_OCTAL_STRING ("\030"s)
    #define ASCII_25_OCTAL_STRING ("\031"s)
    #define ASCII_26_OCTAL_STRING ("\032"s)
    #define ASCII_27_OCTAL_STRING ("\033"s)
    #define ASCII_28_OCTAL_STRING ("\034"s)
    #define ASCII_29_OCTAL_STRING ("\035"s)
    #define ASCII_30_OCTAL_STRING ("\036"s)
    #define ASCII_31_OCTAL_STRING ("\037"s)
    #define ASCII_32_OCTAL_STRING ("\040"s)
    #define ASCII_33_OCTAL_STRING ("\041"s)
    #define ASCII_34_OCTAL_STRING ("\042"s)
    #define ASCII_35_OCTAL_STRING ("\043"s)
    #define ASCII_36_OCTAL_STRING ("\044"s)
    #define ASCII_37_OCTAL_STRING ("\045"s)
    #define ASCII_38_OCTAL_STRING ("\046"s)
    #define ASCII_39_OCTAL_STRING ("\047"s)
    #define ASCII_40_OCTAL_STRING ("\050"s)
    #define ASCII_41_OCTAL_STRING ("\051"s)
    #define ASCII_42_OCTAL_STRING ("\052"s)
    #define ASCII_43_OCTAL_STRING ("\053"s)
    #define ASCII_44_OCTAL_STRING ("\054"s)
    #define ASCII_45_OCTAL_STRING ("\055"s)
    #define ASCII_46_OCTAL_STRING ("\056"s)
    #define ASCII_47_OCTAL_STRING ("\057"s)
    #define ASCII_48_OCTAL_STRING ("\060"s)
    #define ASCII_49_OCTAL_STRING ("\061"s)
    #define ASCII_50_OCTAL_STRING ("\062"s)
    #define ASCII_51_OCTAL_STRING ("\063"s)
    #define ASCII_52_OCTAL_STRING ("\064"s)
    #define ASCII_53_OCTAL_STRING ("\065"s)
    #define ASCII_54_OCTAL_STRING ("\066"s)
    #define ASCII_55_OCTAL_STRING ("\067"s)
    #define ASCII_56_OCTAL_STRING ("\070"s)
    #define ASCII_57_OCTAL_STRING ("\071"s)
    #define ASCII_58_OCTAL_STRING ("\072"s)
    #define ASCII_59_OCTAL_STRING ("\073"s)
    #define ASCII_60_OCTAL_STRING ("\074"s)
    #define ASCII_61_OCTAL_STRING ("\075"s)
    #define ASCII_62_OCTAL_STRING ("\076"s)
    #define ASCII_63_OCTAL_STRING ("\077"s)
    #define ASCII_64_OCTAL_STRING ("\100"s)
    #define ASCII_65_OCTAL_STRING ("\101"s)
    #define ASCII_66_OCTAL_STRING ("\102"s)
    #define ASCII_67_OCTAL_STRING ("\103"s)
    #define ASCII_68_OCTAL_STRING ("\104"s)
    #define ASCII_69_OCTAL_STRING ("\105"s)
    #define ASCII_70_OCTAL_STRING ("\106"s)
    #define ASCII_71_OCTAL_STRING ("\107"s)
    #define ASCII_72_OCTAL_STRING ("\110"s)
    #define ASCII_73_OCTAL_STRING ("\111"s)
    #define ASCII_74_OCTAL_STRING ("\112"s)
    #define ASCII_75_OCTAL_STRING ("\113"s)
    #define ASCII_76_OCTAL_STRING ("\114"s)
    #define ASCII_77_OCTAL_STRING ("\115"s)
    #define ASCII_78_OCTAL_STRING ("\116"s)
    #define ASCII_79_OCTAL_STRING ("\117"s)
    #define ASCII_80_OCTAL_STRING ("\120"s)
    #define ASCII_81_OCTAL_STRING ("\121"s)
    #define ASCII_82_OCTAL_STRING ("\122"s)
    #define ASCII_83_OCTAL_STRING ("\123"s)
    #define ASCII_84_OCTAL_STRING ("\124"s)
    #define ASCII_85_OCTAL_STRING ("\125"s)
    #define ASCII_86_OCTAL_STRING ("\126"s)
    #define ASCII_87_OCTAL_STRING ("\127"s)
    #define ASCII_88_OCTAL_STRING ("\130"s)
    #define ASCII_89_OCTAL_STRING ("\131"s)
    #define ASCII_90_OCTAL_STRING ("\132"s)
    #define ASCII_91_OCTAL_STRING ("\133"s)
    #define ASCII_92_OCTAL_STRING ("\134"s)
    #define ASCII_93_OCTAL_STRING ("\135"s)
    #define ASCII_94_OCTAL_STRING ("\136"s)
    #define ASCII_95_OCTAL_STRING ("\137"s)
    #define ASCII_96_OCTAL_STRING ("\140"s)
    #define ASCII_97_OCTAL_STRING ("\141"s)
    #define ASCII_98_OCTAL_STRING ("\142"s)
    #define ASCII_99_OCTAL_STRING ("\143"s)
    #define ASCII_100_OCTAL_STRING ("\144"s)
    #define ASCII_101_OCTAL_STRING ("\145"s)
    #define ASCII_102_OCTAL_STRING ("\146"s)
    #define ASCII_103_OCTAL_STRING ("\147"s)
    #define ASCII_104_OCTAL_STRING ("\150"s)
    #define ASCII_105_OCTAL_STRING ("\151"s)
    #define ASCII_106_OCTAL_STRING ("\152"s)
    #define ASCII_107_OCTAL_STRING ("\153"s)
    #define ASCII_108_OCTAL_STRING ("\154"s)
    #define ASCII_109_OCTAL_STRING ("\155"s)
    #define ASCII_110_OCTAL_STRING ("\156"s)
    #define ASCII_111_OCTAL_STRING ("\157"s)
    #define ASCII_112_OCTAL_STRING ("\160"s)
    #define ASCII_113_OCTAL_STRING ("\161"s)
    #define ASCII_114_OCTAL_STRING ("\162"s)
    #define ASCII_115_OCTAL_STRING ("\163"s)
    #define ASCII_116_OCTAL_STRING ("\164"s)
    #define ASCII_117_OCTAL_STRING ("\165"s)
    #define ASCII_118_OCTAL_STRING ("\166"s)
    #define ASCII_119_OCTAL_STRING ("\167"s)
    #define ASCII_120_OCTAL_STRING ("\170"s)
    #define ASCII_121_OCTAL_STRING ("\171"s)
    #define ASCII_122_OCTAL_STRING ("\172"s)
    #define ASCII_123_OCTAL_STRING ("\173"s)
    #define ASCII_124_OCTAL_STRING ("\174"s)
    #define ASCII_125_OCTAL_STRING ("\175"s)
    #define ASCII_126_OCTAL_STRING ("\176"s)
    #define ASCII_127_OCTAL_STRING ("\177"s)
    #define ASCII_128_OCTAL_STRING ("\200"s)
    #define ASCII_129_OCTAL_STRING ("\201"s)
    #define ASCII_130_OCTAL_STRING ("\202"s)
    #define ASCII_131_OCTAL_STRING ("\203"s)
    #define ASCII_132_OCTAL_STRING ("\204"s)
    #define ASCII_133_OCTAL_STRING ("\205"s)
    #define ASCII_134_OCTAL_STRING ("\206"s)
    #define ASCII_135_OCTAL_STRING ("\207"s)
    #define ASCII_136_OCTAL_STRING ("\210"s)
    #define ASCII_137_OCTAL_STRING ("\211"s)
    #define ASCII_138_OCTAL_STRING ("\212"s)
    #define ASCII_139_OCTAL_STRING ("\213"s)
    #define ASCII_140_OCTAL_STRING ("\214"s)
    #define ASCII_141_OCTAL_STRING ("\215"s)
    #define ASCII_142_OCTAL_STRING ("\216"s)
    #define ASCII_143_OCTAL_STRING ("\217"s)
    #define ASCII_144_OCTAL_STRING ("\220"s)
    #define ASCII_145_OCTAL_STRING ("\221"s)
    #define ASCII_146_OCTAL_STRING ("\222"s)
    #define ASCII_147_OCTAL_STRING ("\223"s)
    #define ASCII_148_OCTAL_STRING ("\224"s)
    #define ASCII_149_OCTAL_STRING ("\225"s)
    #define ASCII_150_OCTAL_STRING ("\226"s)
    #define ASCII_151_OCTAL_STRING ("\227"s)
    #define ASCII_152_OCTAL_STRING ("\230"s)
    #define ASCII_153_OCTAL_STRING ("\231"s)
    #define ASCII_154_OCTAL_STRING ("\232"s)
    #define ASCII_155_OCTAL_STRING ("\233"s)
    #define ASCII_156_OCTAL_STRING ("\234"s)
    #define ASCII_157_OCTAL_STRING ("\235"s)
    #define ASCII_158_OCTAL_STRING ("\236"s)
    #define ASCII_159_OCTAL_STRING ("\237"s)
    #define ASCII_160_OCTAL_STRING ("\240"s)
    #define ASCII_161_OCTAL_STRING ("\241"s)
    #define ASCII_162_OCTAL_STRING ("\242"s)
    #define ASCII_163_OCTAL_STRING ("\243"s)
    #define ASCII_164_OCTAL_STRING ("\244"s)
    #define ASCII_165_OCTAL_STRING ("\245"s)
    #define ASCII_166_OCTAL_STRING ("\246"s)
    #define ASCII_167_OCTAL_STRING ("\247"s)
    #define ASCII_168_OCTAL_STRING ("\250"s)
    #define ASCII_169_OCTAL_STRING ("\251"s)
    #define ASCII_170_OCTAL_STRING ("\252"s)
    #define ASCII_171_OCTAL_STRING ("\253"s)
    #define ASCII_172_OCTAL_STRING ("\254"s)
    #define ASCII_173_OCTAL_STRING ("\255"s)
    #define ASCII_174_OCTAL_STRING ("\256"s)
    #define ASCII_175_OCTAL_STRING ("\257"s)
    #define ASCII_176_OCTAL_STRING ("\260"s)
    #define ASCII_177_OCTAL_STRING ("\261"s)
    #define ASCII_178_OCTAL_STRING ("\262"s)
    #define ASCII_179_OCTAL_STRING ("\263"s)
    #define ASCII_180_OCTAL_STRING ("\264"s)
    #define ASCII_181_OCTAL_STRING ("\265"s)
    #define ASCII_182_OCTAL_STRING ("\266"s)
    #define ASCII_183_OCTAL_STRING ("\267"s)
    #define ASCII_184_OCTAL_STRING ("\270"s)
    #define ASCII_185_OCTAL_STRING ("\271"s)
    #define ASCII_186_OCTAL_STRING ("\272"s)
    #define ASCII_187_OCTAL_STRING ("\273"s)
    #define ASCII_188_OCTAL_STRING ("\274"s)
    #define ASCII_189_OCTAL_STRING ("\275"s)
    #define ASCII_190_OCTAL_STRING ("\276"s)
    #define ASCII_191_OCTAL_STRING ("\277"s)
    #define ASCII_192_OCTAL_STRING ("\300"s)
    #define ASCII_193_OCTAL_STRING ("\301"s)
    #define ASCII_194_OCTAL_STRING ("\302"s)
    #define ASCII_195_OCTAL_STRING ("\303"s)
    #define ASCII_196_OCTAL_STRING ("\304"s)
    #define ASCII_197_OCTAL_STRING ("\305"s)
    #define ASCII_198_OCTAL_STRING ("\306"s)
    #define ASCII_199_OCTAL_STRING ("\307"s)
    #define ASCII_200_OCTAL_STRING ("\310"s)
    #define ASCII_201_OCTAL_STRING ("\311"s)
    #define ASCII_202_OCTAL_STRING ("\312"s)
    #define ASCII_203_OCTAL_STRING ("\313"s)
    #define ASCII_204_OCTAL_STRING ("\314"s)
    #define ASCII_205_OCTAL_STRING ("\315"s)
    #define ASCII_206_OCTAL_STRING ("\316"s)
    #define ASCII_207_OCTAL_STRING ("\317"s)
    #define ASCII_208_OCTAL_STRING ("\320"s)
    #define ASCII_209_OCTAL_STRING ("\321"s)
    #define ASCII_210_OCTAL_STRING ("\322"s)
    #define ASCII_211_OCTAL_STRING ("\323"s)
    #define ASCII_212_OCTAL_STRING ("\324"s)
    #define ASCII_213_OCTAL_STRING ("\325"s)
    #define ASCII_214_OCTAL_STRING ("\326"s)
    #define ASCII_215_OCTAL_STRING ("\327"s)
    #define ASCII_216_OCTAL_STRING ("\330"s)
    #define ASCII_217_OCTAL_STRING ("\331"s)
    #define ASCII_218_OCTAL_STRING ("\332"s)
    #define ASCII_219_OCTAL_STRING ("\333"s)
    #define ASCII_220_OCTAL_STRING ("\334"s)
    #define ASCII_221_OCTAL_STRING ("\335"s)
    #define ASCII_222_OCTAL_STRING ("\336"s)
    #define ASCII_223_OCTAL_STRING ("\337"s)
    #define ASCII_224_OCTAL_STRING ("\340"s)
    #define ASCII_225_OCTAL_STRING ("\341"s)
    #define ASCII_226_OCTAL_STRING ("\342"s)
    #define ASCII_227_OCTAL_STRING ("\343"s)
    #define ASCII_228_OCTAL_STRING ("\344"s)
    #define ASCII_229_OCTAL_STRING ("\345"s)
    #define ASCII_230_OCTAL_STRING ("\346"s)
    #define ASCII_231_OCTAL_STRING ("\347"s)
    #define ASCII_232_OCTAL_STRING ("\350"s)
    #define ASCII_233_OCTAL_STRING ("\351"s)
    #define ASCII_234_OCTAL_STRING ("\352"s)
    #define ASCII_235_OCTAL_STRING ("\353"s)
    #define ASCII_236_OCTAL_STRING ("\354"s)
    #define ASCII_237_OCTAL_STRING ("\355"s)
    #define ASCII_238_OCTAL_STRING ("\356"s)
    #define ASCII_239_OCTAL_STRING ("\357"s)
    #define ASCII_240_OCTAL_STRING ("\360"s)
    #define ASCII_241_OCTAL_STRING ("\361"s)
    #define ASCII_242_OCTAL_STRING ("\362"s)
    #define ASCII_243_OCTAL_STRING ("\363"s)
    #define ASCII_244_OCTAL_STRING ("\364"s)
    #define ASCII_245_OCTAL_STRING ("\365"s)
    #define ASCII_246_OCTAL_STRING ("\366"s)
    #define ASCII_247_OCTAL_STRING ("\367"s)
    #define ASCII_248_OCTAL_STRING ("\370"s)
    #define ASCII_249_OCTAL_STRING ("\371"s)
    #define ASCII_250_OCTAL_STRING ("\372"s)
    #define ASCII_251_OCTAL_STRING ("\373"s)
    #define ASCII_252_OCTAL_STRING ("\374"s)
    #define ASCII_253_OCTAL_STRING ("\375"s)
    #define ASCII_254_OCTAL_STRING ("\376"s)
    #define ASCII_255_OCTAL_STRING ("\377"s)

    #define ASCII_NEGATIVE_128_OCTAL_CHAR ('\200')
    #define ASCII_NEGATIVE_127_OCTAL_CHAR ('\201')
    #define ASCII_NEGATIVE_126_OCTAL_CHAR ('\202')
    #define ASCII_NEGATIVE_125_OCTAL_CHAR ('\203')
    #define ASCII_NEGATIVE_124_OCTAL_CHAR ('\204')
    #define ASCII_NEGATIVE_123_OCTAL_CHAR ('\205')
    #define ASCII_NEGATIVE_122_OCTAL_CHAR ('\206')
    #define ASCII_NEGATIVE_121_OCTAL_CHAR ('\207')
    #define ASCII_NEGATIVE_120_OCTAL_CHAR ('\210')
    #define ASCII_NEGATIVE_119_OCTAL_CHAR ('\211')
    #define ASCII_NEGATIVE_118_OCTAL_CHAR ('\212')
    #define ASCII_NEGATIVE_117_OCTAL_CHAR ('\213')
    #define ASCII_NEGATIVE_116_OCTAL_CHAR ('\214')
    #define ASCII_NEGATIVE_115_OCTAL_CHAR ('\215')
    #define ASCII_NEGATIVE_114_OCTAL_CHAR ('\216')
    #define ASCII_NEGATIVE_113_OCTAL_CHAR ('\217')
    #define ASCII_NEGATIVE_112_OCTAL_CHAR ('\220')
    #define ASCII_NEGATIVE_111_OCTAL_CHAR ('\221')
    #define ASCII_NEGATIVE_110_OCTAL_CHAR ('\222')
    #define ASCII_NEGATIVE_109_OCTAL_CHAR ('\223')
    #define ASCII_NEGATIVE_108_OCTAL_CHAR ('\224')
    #define ASCII_NEGATIVE_107_OCTAL_CHAR ('\225')
    #define ASCII_NEGATIVE_106_OCTAL_CHAR ('\226')
    #define ASCII_NEGATIVE_105_OCTAL_CHAR ('\227')
    #define ASCII_NEGATIVE_104_OCTAL_CHAR ('\230')
    #define ASCII_NEGATIVE_103_OCTAL_CHAR ('\231')
    #define ASCII_NEGATIVE_102_OCTAL_CHAR ('\232')
    #define ASCII_NEGATIVE_101_OCTAL_CHAR ('\233')
    #define ASCII_NEGATIVE_100_OCTAL_CHAR ('\234')
    #define ASCII_NEGATIVE_99_OCTAL_CHAR ('\235')
    #define ASCII_NEGATIVE_98_OCTAL_CHAR ('\236')
    #define ASCII_NEGATIVE_97_OCTAL_CHAR ('\237')
    #define ASCII_NEGATIVE_96_OCTAL_CHAR ('\240')
    #define ASCII_NEGATIVE_95_OCTAL_CHAR ('\241')
    #define ASCII_NEGATIVE_94_OCTAL_CHAR ('\242')
    #define ASCII_NEGATIVE_93_OCTAL_CHAR ('\243')
    #define ASCII_NEGATIVE_92_OCTAL_CHAR ('\244')
    #define ASCII_NEGATIVE_91_OCTAL_CHAR ('\245')
    #define ASCII_NEGATIVE_90_OCTAL_CHAR ('\246')
    #define ASCII_NEGATIVE_89_OCTAL_CHAR ('\247')
    #define ASCII_NEGATIVE_88_OCTAL_CHAR ('\250')
    #define ASCII_NEGATIVE_87_OCTAL_CHAR ('\251')
    #define ASCII_NEGATIVE_86_OCTAL_CHAR ('\252')
    #define ASCII_NEGATIVE_85_OCTAL_CHAR ('\253')
    #define ASCII_NEGATIVE_84_OCTAL_CHAR ('\254')
    #define ASCII_NEGATIVE_83_OCTAL_CHAR ('\255')
    #define ASCII_NEGATIVE_82_OCTAL_CHAR ('\256')
    #define ASCII_NEGATIVE_81_OCTAL_CHAR ('\257')
    #define ASCII_NEGATIVE_80_OCTAL_CHAR ('\260')
    #define ASCII_NEGATIVE_79_OCTAL_CHAR ('\261')
    #define ASCII_NEGATIVE_78_OCTAL_CHAR ('\262')
    #define ASCII_NEGATIVE_77_OCTAL_CHAR ('\263')
    #define ASCII_NEGATIVE_76_OCTAL_CHAR ('\264')
    #define ASCII_NEGATIVE_75_OCTAL_CHAR ('\265')
    #define ASCII_NEGATIVE_74_OCTAL_CHAR ('\266')
    #define ASCII_NEGATIVE_73_OCTAL_CHAR ('\267')
    #define ASCII_NEGATIVE_72_OCTAL_CHAR ('\270')
    #define ASCII_NEGATIVE_71_OCTAL_CHAR ('\271')
    #define ASCII_NEGATIVE_70_OCTAL_CHAR ('\272')
    #define ASCII_NEGATIVE_69_OCTAL_CHAR ('\273')
    #define ASCII_NEGATIVE_68_OCTAL_CHAR ('\274')
    #define ASCII_NEGATIVE_67_OCTAL_CHAR ('\275')
    #define ASCII_NEGATIVE_66_OCTAL_CHAR ('\276')
    #define ASCII_NEGATIVE_65_OCTAL_CHAR ('\277')
    #define ASCII_NEGATIVE_64_OCTAL_CHAR ('\300')
    #define ASCII_NEGATIVE_63_OCTAL_CHAR ('\301')
    #define ASCII_NEGATIVE_62_OCTAL_CHAR ('\302')
    #define ASCII_NEGATIVE_61_OCTAL_CHAR ('\303')
    #define ASCII_NEGATIVE_60_OCTAL_CHAR ('\304')
    #define ASCII_NEGATIVE_59_OCTAL_CHAR ('\305')
    #define ASCII_NEGATIVE_58_OCTAL_CHAR ('\306')
    #define ASCII_NEGATIVE_57_OCTAL_CHAR ('\307')
    #define ASCII_NEGATIVE_56_OCTAL_CHAR ('\310')
    #define ASCII_NEGATIVE_55_OCTAL_CHAR ('\311')
    #define ASCII_NEGATIVE_54_OCTAL_CHAR ('\312')
    #define ASCII_NEGATIVE_53_OCTAL_CHAR ('\313')
    #define ASCII_NEGATIVE_52_OCTAL_CHAR ('\314')
    #define ASCII_NEGATIVE_51_OCTAL_CHAR ('\315')
    #define ASCII_NEGATIVE_50_OCTAL_CHAR ('\316')
    #define ASCII_NEGATIVE_49_OCTAL_CHAR ('\317')
    #define ASCII_NEGATIVE_48_OCTAL_CHAR ('\320')
    #define ASCII_NEGATIVE_47_OCTAL_CHAR ('\321')
    #define ASCII_NEGATIVE_46_OCTAL_CHAR ('\322')
    #define ASCII_NEGATIVE_45_OCTAL_CHAR ('\323')
    #define ASCII_NEGATIVE_44_OCTAL_CHAR ('\324')
    #define ASCII_NEGATIVE_43_OCTAL_CHAR ('\325')
    #define ASCII_NEGATIVE_42_OCTAL_CHAR ('\326')
    #define ASCII_NEGATIVE_41_OCTAL_CHAR ('\327')
    #define ASCII_NEGATIVE_40_OCTAL_CHAR ('\330')
    #define ASCII_NEGATIVE_39_OCTAL_CHAR ('\331')
    #define ASCII_NEGATIVE_38_OCTAL_CHAR ('\332')
    #define ASCII_NEGATIVE_37_OCTAL_CHAR ('\333')
    #define ASCII_NEGATIVE_36_OCTAL_CHAR ('\334')
    #define ASCII_NEGATIVE_35_OCTAL_CHAR ('\335')
    #define ASCII_NEGATIVE_34_OCTAL_CHAR ('\336')
    #define ASCII_NEGATIVE_33_OCTAL_CHAR ('\337')
    #define ASCII_NEGATIVE_32_OCTAL_CHAR ('\340')
    #define ASCII_NEGATIVE_31_OCTAL_CHAR ('\341')
    #define ASCII_NEGATIVE_30_OCTAL_CHAR ('\342')
    #define ASCII_NEGATIVE_29_OCTAL_CHAR ('\343')
    #define ASCII_NEGATIVE_28_OCTAL_CHAR ('\344')
    #define ASCII_NEGATIVE_27_OCTAL_CHAR ('\345')
    #define ASCII_NEGATIVE_26_OCTAL_CHAR ('\346')
    #define ASCII_NEGATIVE_25_OCTAL_CHAR ('\347')
    #define ASCII_NEGATIVE_24_OCTAL_CHAR ('\350')
    #define ASCII_NEGATIVE_23_OCTAL_CHAR ('\351')
    #define ASCII_NEGATIVE_22_OCTAL_CHAR ('\352')
    #define ASCII_NEGATIVE_21_OCTAL_CHAR ('\353')
    #define ASCII_NEGATIVE_20_OCTAL_CHAR ('\354')
    #define ASCII_NEGATIVE_19_OCTAL_CHAR ('\355')
    #define ASCII_NEGATIVE_18_OCTAL_CHAR ('\356')
    #define ASCII_NEGATIVE_17_OCTAL_CHAR ('\357')
    #define ASCII_NEGATIVE_16_OCTAL_CHAR ('\360')
    #define ASCII_NEGATIVE_15_OCTAL_CHAR ('\361')
    #define ASCII_NEGATIVE_14_OCTAL_CHAR ('\362')
    #define ASCII_NEGATIVE_13_OCTAL_CHAR ('\363')
    #define ASCII_NEGATIVE_12_OCTAL_CHAR ('\364')
    #define ASCII_NEGATIVE_11_OCTAL_CHAR ('\365')
    #define ASCII_NEGATIVE_10_OCTAL_CHAR ('\366')
    #define ASCII_NEGATIVE_9_OCTAL_CHAR ('\367')
    #define ASCII_NEGATIVE_8_OCTAL_CHAR ('\370')
    #define ASCII_NEGATIVE_7_OCTAL_CHAR ('\371')
    #define ASCII_NEGATIVE_6_OCTAL_CHAR ('\372')
    #define ASCII_NEGATIVE_5_OCTAL_CHAR ('\373')
    #define ASCII_NEGATIVE_4_OCTAL_CHAR ('\374')
    #define ASCII_NEGATIVE_3_OCTAL_CHAR ('\375')
    #define ASCII_NEGATIVE_2_OCTAL_CHAR ('\376')
    #define ASCII_NEGATIVE_1_OCTAL_CHAR ('\377')

    #define ASCII_0_OCTAL_CHAR ('\000')
    #define ASCII_1_OCTAL_CHAR ('\001')
    #define ASCII_2_OCTAL_CHAR ('\002')
    #define ASCII_3_OCTAL_CHAR ('\003')
    #define ASCII_4_OCTAL_CHAR ('\004')
    #define ASCII_5_OCTAL_CHAR ('\005')
    #define ASCII_6_OCTAL_CHAR ('\006')
    #define ASCII_7_OCTAL_CHAR ('\007')
    #define ASCII_8_OCTAL_CHAR ('\010')
    #define ASCII_9_OCTAL_CHAR ('\011')
    #define ASCII_10_OCTAL_CHAR ('\012')
    #define ASCII_11_OCTAL_CHAR ('\013')
    #define ASCII_12_OCTAL_CHAR ('\014')
    #define ASCII_13_OCTAL_CHAR ('\015')
    #define ASCII_14_OCTAL_CHAR ('\016')
    #define ASCII_15_OCTAL_CHAR ('\017')
    #define ASCII_16_OCTAL_CHAR ('\020')
    #define ASCII_17_OCTAL_CHAR ('\021')
    #define ASCII_18_OCTAL_CHAR ('\022')
    #define ASCII_19_OCTAL_CHAR ('\023')
    #define ASCII_20_OCTAL_CHAR ('\024')
    #define ASCII_21_OCTAL_CHAR ('\025')
    #define ASCII_22_OCTAL_CHAR ('\026')
    #define ASCII_23_OCTAL_CHAR ('\027')
    #define ASCII_24_OCTAL_CHAR ('\030')
    #define ASCII_25_OCTAL_CHAR ('\031')
    #define ASCII_26_OCTAL_CHAR ('\032')
    #define ASCII_27_OCTAL_CHAR ('\033')
    #define ASCII_28_OCTAL_CHAR ('\034')
    #define ASCII_29_OCTAL_CHAR ('\035')
    #define ASCII_30_OCTAL_CHAR ('\036')
    #define ASCII_31_OCTAL_CHAR ('\037')
    #define ASCII_32_OCTAL_CHAR ('\040')
    #define ASCII_33_OCTAL_CHAR ('\041')
    #define ASCII_34_OCTAL_CHAR ('\042')
    #define ASCII_35_OCTAL_CHAR ('\043')
    #define ASCII_36_OCTAL_CHAR ('\044')
    #define ASCII_37_OCTAL_CHAR ('\045')
    #define ASCII_38_OCTAL_CHAR ('\046')
    #define ASCII_39_OCTAL_CHAR ('\047')
    #define ASCII_40_OCTAL_CHAR ('\050')
    #define ASCII_41_OCTAL_CHAR ('\051')
    #define ASCII_42_OCTAL_CHAR ('\052')
    #define ASCII_43_OCTAL_CHAR ('\053')
    #define ASCII_44_OCTAL_CHAR ('\054')
    #define ASCII_45_OCTAL_CHAR ('\055')
    #define ASCII_46_OCTAL_CHAR ('\056')
    #define ASCII_47_OCTAL_CHAR ('\057')
    #define ASCII_48_OCTAL_CHAR ('\060')
    #define ASCII_49_OCTAL_CHAR ('\061')
    #define ASCII_50_OCTAL_CHAR ('\062')
    #define ASCII_51_OCTAL_CHAR ('\063')
    #define ASCII_52_OCTAL_CHAR ('\064')
    #define ASCII_53_OCTAL_CHAR ('\065')
    #define ASCII_54_OCTAL_CHAR ('\066')
    #define ASCII_55_OCTAL_CHAR ('\067')
    #define ASCII_56_OCTAL_CHAR ('\070')
    #define ASCII_57_OCTAL_CHAR ('\071')
    #define ASCII_58_OCTAL_CHAR ('\072')
    #define ASCII_59_OCTAL_CHAR ('\073')
    #define ASCII_60_OCTAL_CHAR ('\074')
    #define ASCII_61_OCTAL_CHAR ('\075')
    #define ASCII_62_OCTAL_CHAR ('\076')
    #define ASCII_63_OCTAL_CHAR ('\077')
    #define ASCII_64_OCTAL_CHAR ('\100')
    #define ASCII_65_OCTAL_CHAR ('\101')
    #define ASCII_66_OCTAL_CHAR ('\102')
    #define ASCII_67_OCTAL_CHAR ('\103')
    #define ASCII_68_OCTAL_CHAR ('\104')
    #define ASCII_69_OCTAL_CHAR ('\105')
    #define ASCII_70_OCTAL_CHAR ('\106')
    #define ASCII_71_OCTAL_CHAR ('\107')
    #define ASCII_72_OCTAL_CHAR ('\110')
    #define ASCII_73_OCTAL_CHAR ('\111')
    #define ASCII_74_OCTAL_CHAR ('\112')
    #define ASCII_75_OCTAL_CHAR ('\113')
    #define ASCII_76_OCTAL_CHAR ('\114')
    #define ASCII_77_OCTAL_CHAR ('\115')
    #define ASCII_78_OCTAL_CHAR ('\116')
    #define ASCII_79_OCTAL_CHAR ('\117')
    #define ASCII_80_OCTAL_CHAR ('\120')
    #define ASCII_81_OCTAL_CHAR ('\121')
    #define ASCII_82_OCTAL_CHAR ('\122')
    #define ASCII_83_OCTAL_CHAR ('\123')
    #define ASCII_84_OCTAL_CHAR ('\124')
    #define ASCII_85_OCTAL_CHAR ('\125')
    #define ASCII_86_OCTAL_CHAR ('\126')
    #define ASCII_87_OCTAL_CHAR ('\127')
    #define ASCII_88_OCTAL_CHAR ('\130')
    #define ASCII_89_OCTAL_CHAR ('\131')
    #define ASCII_90_OCTAL_CHAR ('\132')
    #define ASCII_91_OCTAL_CHAR ('\133')
    #define ASCII_92_OCTAL_CHAR ('\134')
    #define ASCII_93_OCTAL_CHAR ('\135')
    #define ASCII_94_OCTAL_CHAR ('\136')
    #define ASCII_95_OCTAL_CHAR ('\137')
    #define ASCII_96_OCTAL_CHAR ('\140')
    #define ASCII_97_OCTAL_CHAR ('\141')
    #define ASCII_98_OCTAL_CHAR ('\142')
    #define ASCII_99_OCTAL_CHAR ('\143')
    #define ASCII_100_OCTAL_CHAR ('\144')
    #define ASCII_101_OCTAL_CHAR ('\145')
    #define ASCII_102_OCTAL_CHAR ('\146')
    #define ASCII_103_OCTAL_CHAR ('\147')
    #define ASCII_104_OCTAL_CHAR ('\150')
    #define ASCII_105_OCTAL_CHAR ('\151')
    #define ASCII_106_OCTAL_CHAR ('\152')
    #define ASCII_107_OCTAL_CHAR ('\153')
    #define ASCII_108_OCTAL_CHAR ('\154')
    #define ASCII_109_OCTAL_CHAR ('\155')
    #define ASCII_110_OCTAL_CHAR ('\156')
    #define ASCII_111_OCTAL_CHAR ('\157')
    #define ASCII_112_OCTAL_CHAR ('\160')
    #define ASCII_113_OCTAL_CHAR ('\161')
    #define ASCII_114_OCTAL_CHAR ('\162')
    #define ASCII_115_OCTAL_CHAR ('\163')
    #define ASCII_116_OCTAL_CHAR ('\164')
    #define ASCII_117_OCTAL_CHAR ('\165')
    #define ASCII_118_OCTAL_CHAR ('\166')
    #define ASCII_119_OCTAL_CHAR ('\167')
    #define ASCII_120_OCTAL_CHAR ('\170')
    #define ASCII_121_OCTAL_CHAR ('\171')
    #define ASCII_122_OCTAL_CHAR ('\172')
    #define ASCII_123_OCTAL_CHAR ('\173')
    #define ASCII_124_OCTAL_CHAR ('\174')
    #define ASCII_125_OCTAL_CHAR ('\175')
    #define ASCII_126_OCTAL_CHAR ('\176')
    #define ASCII_127_OCTAL_CHAR ('\177')
    #define ASCII_128_OCTAL_CHAR ('\200')
    #define ASCII_129_OCTAL_CHAR ('\201')
    #define ASCII_130_OCTAL_CHAR ('\202')
    #define ASCII_131_OCTAL_CHAR ('\203')
    #define ASCII_132_OCTAL_CHAR ('\204')
    #define ASCII_133_OCTAL_CHAR ('\205')
    #define ASCII_134_OCTAL_CHAR ('\206')
    #define ASCII_135_OCTAL_CHAR ('\207')
    #define ASCII_136_OCTAL_CHAR ('\210')
    #define ASCII_137_OCTAL_CHAR ('\211')
    #define ASCII_138_OCTAL_CHAR ('\212')
    #define ASCII_139_OCTAL_CHAR ('\213')
    #define ASCII_140_OCTAL_CHAR ('\214')
    #define ASCII_141_OCTAL_CHAR ('\215')
    #define ASCII_142_OCTAL_CHAR ('\216')
    #define ASCII_143_OCTAL_CHAR ('\217')
    #define ASCII_144_OCTAL_CHAR ('\220')
    #define ASCII_145_OCTAL_CHAR ('\221')
    #define ASCII_146_OCTAL_CHAR ('\222')
    #define ASCII_147_OCTAL_CHAR ('\223')
    #define ASCII_148_OCTAL_CHAR ('\224')
    #define ASCII_149_OCTAL_CHAR ('\225')
    #define ASCII_150_OCTAL_CHAR ('\226')
    #define ASCII_151_OCTAL_CHAR ('\227')
    #define ASCII_152_OCTAL_CHAR ('\230')
    #define ASCII_153_OCTAL_CHAR ('\231')
    #define ASCII_154_OCTAL_CHAR ('\232')
    #define ASCII_155_OCTAL_CHAR ('\233')
    #define ASCII_156_OCTAL_CHAR ('\234')
    #define ASCII_157_OCTAL_CHAR ('\235')
    #define ASCII_158_OCTAL_CHAR ('\236')
    #define ASCII_159_OCTAL_CHAR ('\237')
    #define ASCII_160_OCTAL_CHAR ('\240')
    #define ASCII_161_OCTAL_CHAR ('\241')
    #define ASCII_162_OCTAL_CHAR ('\242')
    #define ASCII_163_OCTAL_CHAR ('\243')
    #define ASCII_164_OCTAL_CHAR ('\244')
    #define ASCII_165_OCTAL_CHAR ('\245')
    #define ASCII_166_OCTAL_CHAR ('\246')
    #define ASCII_167_OCTAL_CHAR ('\247')
    #define ASCII_168_OCTAL_CHAR ('\250')
    #define ASCII_169_OCTAL_CHAR ('\251')
    #define ASCII_170_OCTAL_CHAR ('\252')
    #define ASCII_171_OCTAL_CHAR ('\253')
    #define ASCII_172_OCTAL_CHAR ('\254')
    #define ASCII_173_OCTAL_CHAR ('\255')
    #define ASCII_174_OCTAL_CHAR ('\256')
    #define ASCII_175_OCTAL_CHAR ('\257')
    #define ASCII_176_OCTAL_CHAR ('\260')
    #define ASCII_177_OCTAL_CHAR ('\261')
    #define ASCII_178_OCTAL_CHAR ('\262')
    #define ASCII_179_OCTAL_CHAR ('\263')
    #define ASCII_180_OCTAL_CHAR ('\264')
    #define ASCII_181_OCTAL_CHAR ('\265')
    #define ASCII_182_OCTAL_CHAR ('\266')
    #define ASCII_183_OCTAL_CHAR ('\267')
    #define ASCII_184_OCTAL_CHAR ('\270')
    #define ASCII_185_OCTAL_CHAR ('\271')
    #define ASCII_186_OCTAL_CHAR ('\272')
    #define ASCII_187_OCTAL_CHAR ('\273')
    #define ASCII_188_OCTAL_CHAR ('\274')
    #define ASCII_189_OCTAL_CHAR ('\275')
    #define ASCII_190_OCTAL_CHAR ('\276')
    #define ASCII_191_OCTAL_CHAR ('\277')
    #define ASCII_192_OCTAL_CHAR ('\300')
    #define ASCII_193_OCTAL_CHAR ('\301')
    #define ASCII_194_OCTAL_CHAR ('\302')
    #define ASCII_195_OCTAL_CHAR ('\303')
    #define ASCII_196_OCTAL_CHAR ('\304')
    #define ASCII_197_OCTAL_CHAR ('\305')
    #define ASCII_198_OCTAL_CHAR ('\306')
    #define ASCII_199_OCTAL_CHAR ('\307')
    #define ASCII_200_OCTAL_CHAR ('\310')
    #define ASCII_201_OCTAL_CHAR ('\311')
    #define ASCII_202_OCTAL_CHAR ('\312')
    #define ASCII_203_OCTAL_CHAR ('\313')
    #define ASCII_204_OCTAL_CHAR ('\314')
    #define ASCII_205_OCTAL_CHAR ('\315')
    #define ASCII_206_OCTAL_CHAR ('\316')
    #define ASCII_207_OCTAL_CHAR ('\317')
    #define ASCII_208_OCTAL_CHAR ('\320')
    #define ASCII_209_OCTAL_CHAR ('\321')
    #define ASCII_210_OCTAL_CHAR ('\322')
    #define ASCII_211_OCTAL_CHAR ('\323')
    #define ASCII_212_OCTAL_CHAR ('\324')
    #define ASCII_213_OCTAL_CHAR ('\325')
    #define ASCII_214_OCTAL_CHAR ('\326')
    #define ASCII_215_OCTAL_CHAR ('\327')
    #define ASCII_216_OCTAL_CHAR ('\330')
    #define ASCII_217_OCTAL_CHAR ('\331')
    #define ASCII_218_OCTAL_CHAR ('\332')
    #define ASCII_219_OCTAL_CHAR ('\333')
    #define ASCII_220_OCTAL_CHAR ('\334')
    #define ASCII_221_OCTAL_CHAR ('\335')
    #define ASCII_222_OCTAL_CHAR ('\336')
    #define ASCII_223_OCTAL_CHAR ('\337')
    #define ASCII_224_OCTAL_CHAR ('\340')
    #define ASCII_225_OCTAL_CHAR ('\341')
    #define ASCII_226_OCTAL_CHAR ('\342')
    #define ASCII_227_OCTAL_CHAR ('\343')
    #define ASCII_228_OCTAL_CHAR ('\344')
    #define ASCII_229_OCTAL_CHAR ('\345')
    #define ASCII_230_OCTAL_CHAR ('\346')
    #define ASCII_231_OCTAL_CHAR ('\347')
    #define ASCII_232_OCTAL_CHAR ('\350')
    #define ASCII_233_OCTAL_CHAR ('\351')
    #define ASCII_234_OCTAL_CHAR ('\352')
    #define ASCII_235_OCTAL_CHAR ('\353')
    #define ASCII_236_OCTAL_CHAR ('\354')
    #define ASCII_237_OCTAL_CHAR ('\355')
    #define ASCII_238_OCTAL_CHAR ('\356')
    #define ASCII_239_OCTAL_CHAR ('\357')
    #define ASCII_240_OCTAL_CHAR ('\360')
    #define ASCII_241_OCTAL_CHAR ('\361')
    #define ASCII_242_OCTAL_CHAR ('\362')
    #define ASCII_243_OCTAL_CHAR ('\363')
    #define ASCII_244_OCTAL_CHAR ('\364')
    #define ASCII_245_OCTAL_CHAR ('\365')
    #define ASCII_246_OCTAL_CHAR ('\366')
    #define ASCII_247_OCTAL_CHAR ('\367')
    #define ASCII_248_OCTAL_CHAR ('\370')
    #define ASCII_249_OCTAL_CHAR ('\371')
    #define ASCII_250_OCTAL_CHAR ('\372')
    #define ASCII_251_OCTAL_CHAR ('\373')
    #define ASCII_252_OCTAL_CHAR ('\374')
    #define ASCII_253_OCTAL_CHAR ('\375')
    #define ASCII_254_OCTAL_CHAR ('\376')
    #define ASCII_255_OCTAL_CHAR ('\377')

    #define ASCII_NEGATIVE_128_HEX_STRING ("\x80"s)
    #define ASCII_NEGATIVE_127_HEX_STRING ("\x81"s)
    #define ASCII_NEGATIVE_126_HEX_STRING ("\x82"s)
    #define ASCII_NEGATIVE_125_HEX_STRING ("\x83"s)
    #define ASCII_NEGATIVE_124_HEX_STRING ("\x84"s)
    #define ASCII_NEGATIVE_123_HEX_STRING ("\x85"s)
    #define ASCII_NEGATIVE_122_HEX_STRING ("\x86"s)
    #define ASCII_NEGATIVE_121_HEX_STRING ("\x87"s)
    #define ASCII_NEGATIVE_120_HEX_STRING ("\x88"s)
    #define ASCII_NEGATIVE_119_HEX_STRING ("\x89"s)
    #define ASCII_NEGATIVE_118_HEX_STRING ("\x8a"s)
    #define ASCII_NEGATIVE_117_HEX_STRING ("\x8b"s)
    #define ASCII_NEGATIVE_116_HEX_STRING ("\x8c"s)
    #define ASCII_NEGATIVE_115_HEX_STRING ("\x8d"s)
    #define ASCII_NEGATIVE_114_HEX_STRING ("\x8e"s)
    #define ASCII_NEGATIVE_113_HEX_STRING ("\x8f"s)
    #define ASCII_NEGATIVE_112_HEX_STRING ("\x90"s)
    #define ASCII_NEGATIVE_111_HEX_STRING ("\x91"s)
    #define ASCII_NEGATIVE_110_HEX_STRING ("\x92"s)
    #define ASCII_NEGATIVE_109_HEX_STRING ("\x93"s)
    #define ASCII_NEGATIVE_108_HEX_STRING ("\x94"s)
    #define ASCII_NEGATIVE_107_HEX_STRING ("\x95"s)
    #define ASCII_NEGATIVE_106_HEX_STRING ("\x96"s)
    #define ASCII_NEGATIVE_105_HEX_STRING ("\x97"s)
    #define ASCII_NEGATIVE_104_HEX_STRING ("\x98"s)
    #define ASCII_NEGATIVE_103_HEX_STRING ("\x99"s)
    #define ASCII_NEGATIVE_102_HEX_STRING ("\x9a"s)
    #define ASCII_NEGATIVE_101_HEX_STRING ("\x9b"s)
    #define ASCII_NEGATIVE_100_HEX_STRING ("\x9c"s)
    #define ASCII_NEGATIVE_99_HEX_STRING ("\x9d"s)
    #define ASCII_NEGATIVE_98_HEX_STRING ("\x9e"s)
    #define ASCII_NEGATIVE_97_HEX_STRING ("\x9f"s)
    #define ASCII_NEGATIVE_96_HEX_STRING ("\xa0"s)
    #define ASCII_NEGATIVE_95_HEX_STRING ("\xa1"s)
    #define ASCII_NEGATIVE_94_HEX_STRING ("\xa2"s)
    #define ASCII_NEGATIVE_93_HEX_STRING ("\xa3"s)
    #define ASCII_NEGATIVE_92_HEX_STRING ("\xa4"s)
    #define ASCII_NEGATIVE_91_HEX_STRING ("\xa5"s)
    #define ASCII_NEGATIVE_90_HEX_STRING ("\xa6"s)
    #define ASCII_NEGATIVE_89_HEX_STRING ("\xa7"s)
    #define ASCII_NEGATIVE_88_HEX_STRING ("\xa8"s)
    #define ASCII_NEGATIVE_87_HEX_STRING ("\xa9"s)
    #define ASCII_NEGATIVE_86_HEX_STRING ("\xaa"s)
    #define ASCII_NEGATIVE_85_HEX_STRING ("\xab"s)
    #define ASCII_NEGATIVE_84_HEX_STRING ("\xac"s)
    #define ASCII_NEGATIVE_83_HEX_STRING ("\xad"s)
    #define ASCII_NEGATIVE_82_HEX_STRING ("\xae"s)
    #define ASCII_NEGATIVE_81_HEX_STRING ("\xaf"s)
    #define ASCII_NEGATIVE_80_HEX_STRING ("\xb0"s)
    #define ASCII_NEGATIVE_79_HEX_STRING ("\xb1"s)
    #define ASCII_NEGATIVE_78_HEX_STRING ("\xb2"s)
    #define ASCII_NEGATIVE_77_HEX_STRING ("\xb3"s)
    #define ASCII_NEGATIVE_76_HEX_STRING ("\xb4"s)
    #define ASCII_NEGATIVE_75_HEX_STRING ("\xb5"s)
    #define ASCII_NEGATIVE_74_HEX_STRING ("\xb6"s)
    #define ASCII_NEGATIVE_73_HEX_STRING ("\xb7"s)
    #define ASCII_NEGATIVE_72_HEX_STRING ("\xb8"s)
    #define ASCII_NEGATIVE_71_HEX_STRING ("\xb9"s)
    #define ASCII_NEGATIVE_70_HEX_STRING ("\xba"s)
    #define ASCII_NEGATIVE_69_HEX_STRING ("\xbb"s)
    #define ASCII_NEGATIVE_68_HEX_STRING ("\xbc"s)
    #define ASCII_NEGATIVE_67_HEX_STRING ("\xbd"s)
    #define ASCII_NEGATIVE_66_HEX_STRING ("\xbe"s)
    #define ASCII_NEGATIVE_65_HEX_STRING ("\xbf"s)
    #define ASCII_NEGATIVE_64_HEX_STRING ("\xc0"s)
    #define ASCII_NEGATIVE_63_HEX_STRING ("\xc1"s)
    #define ASCII_NEGATIVE_62_HEX_STRING ("\xc2"s)
    #define ASCII_NEGATIVE_61_HEX_STRING ("\xc3"s)
    #define ASCII_NEGATIVE_60_HEX_STRING ("\xc4"s)
    #define ASCII_NEGATIVE_59_HEX_STRING ("\xc5"s)
    #define ASCII_NEGATIVE_58_HEX_STRING ("\xc6"s)
    #define ASCII_NEGATIVE_57_HEX_STRING ("\xc7"s)
    #define ASCII_NEGATIVE_56_HEX_STRING ("\xc8"s)
    #define ASCII_NEGATIVE_55_HEX_STRING ("\xc9"s)
    #define ASCII_NEGATIVE_54_HEX_STRING ("\xca"s)
    #define ASCII_NEGATIVE_53_HEX_STRING ("\xcb"s)
    #define ASCII_NEGATIVE_52_HEX_STRING ("\xcc"s)
    #define ASCII_NEGATIVE_51_HEX_STRING ("\xcd"s)
    #define ASCII_NEGATIVE_50_HEX_STRING ("\xce"s)
    #define ASCII_NEGATIVE_49_HEX_STRING ("\xcf"s)
    #define ASCII_NEGATIVE_48_HEX_STRING ("\xd0"s)
    #define ASCII_NEGATIVE_47_HEX_STRING ("\xd1"s)
    #define ASCII_NEGATIVE_46_HEX_STRING ("\xd2"s)
    #define ASCII_NEGATIVE_45_HEX_STRING ("\xd3"s)
    #define ASCII_NEGATIVE_44_HEX_STRING ("\xd4"s)
    #define ASCII_NEGATIVE_43_HEX_STRING ("\xd5"s)
    #define ASCII_NEGATIVE_42_HEX_STRING ("\xd6"s)
    #define ASCII_NEGATIVE_41_HEX_STRING ("\xd7"s)
    #define ASCII_NEGATIVE_40_HEX_STRING ("\xd8"s)
    #define ASCII_NEGATIVE_39_HEX_STRING ("\xd9"s)
    #define ASCII_NEGATIVE_38_HEX_STRING ("\xda"s)
    #define ASCII_NEGATIVE_37_HEX_STRING ("\xdb"s)
    #define ASCII_NEGATIVE_36_HEX_STRING ("\xdc"s)
    #define ASCII_NEGATIVE_35_HEX_STRING ("\xdd"s)
    #define ASCII_NEGATIVE_34_HEX_STRING ("\xde"s)
    #define ASCII_NEGATIVE_33_HEX_STRING ("\xdf"s)
    #define ASCII_NEGATIVE_32_HEX_STRING ("\xe0"s)
    #define ASCII_NEGATIVE_31_HEX_STRING ("\xe1"s)
    #define ASCII_NEGATIVE_30_HEX_STRING ("\xe2"s)
    #define ASCII_NEGATIVE_29_HEX_STRING ("\xe3"s)
    #define ASCII_NEGATIVE_28_HEX_STRING ("\xe4"s)
    #define ASCII_NEGATIVE_27_HEX_STRING ("\xe5"s)
    #define ASCII_NEGATIVE_26_HEX_STRING ("\xe6"s)
    #define ASCII_NEGATIVE_25_HEX_STRING ("\xe7"s)
    #define ASCII_NEGATIVE_24_HEX_STRING ("\xe8"s)
    #define ASCII_NEGATIVE_23_HEX_STRING ("\xe9"s)
    #define ASCII_NEGATIVE_22_HEX_STRING ("\xea"s)
    #define ASCII_NEGATIVE_21_HEX_STRING ("\xeb"s)
    #define ASCII_NEGATIVE_20_HEX_STRING ("\xec"s)
    #define ASCII_NEGATIVE_19_HEX_STRING ("\xed"s)
    #define ASCII_NEGATIVE_18_HEX_STRING ("\xee"s)
    #define ASCII_NEGATIVE_17_HEX_STRING ("\xef"s)
    #define ASCII_NEGATIVE_16_HEX_STRING ("\xf0"s)
    #define ASCII_NEGATIVE_15_HEX_STRING ("\xf1"s)
    #define ASCII_NEGATIVE_14_HEX_STRING ("\xf2"s)
    #define ASCII_NEGATIVE_13_HEX_STRING ("\xf3"s)
    #define ASCII_NEGATIVE_12_HEX_STRING ("\xf4"s)
    #define ASCII_NEGATIVE_11_HEX_STRING ("\xf5"s)
    #define ASCII_NEGATIVE_10_HEX_STRING ("\xf6"s)
    #define ASCII_NEGATIVE_9_HEX_STRING ("\xf7"s)
    #define ASCII_NEGATIVE_8_HEX_STRING ("\xf8"s)
    #define ASCII_NEGATIVE_7_HEX_STRING ("\xf9"s)
    #define ASCII_NEGATIVE_6_HEX_STRING ("\xfa"s)
    #define ASCII_NEGATIVE_5_HEX_STRING ("\xfb"s)
    #define ASCII_NEGATIVE_4_HEX_STRING ("\xfc"s)
    #define ASCII_NEGATIVE_3_HEX_STRING ("\xfd"s)
    #define ASCII_NEGATIVE_2_HEX_STRING ("\xfe"s)
    #define ASCII_NEGATIVE_1_HEX_STRING ("\xff"s)

    #define ASCII_0_HEX_STRING ("\x00"s)
    #define ASCII_1_HEX_STRING ("\x01"s)
    #define ASCII_2_HEX_STRING ("\x02"s)
    #define ASCII_3_HEX_STRING ("\x03"s)
    #define ASCII_4_HEX_STRING ("\x04"s)
    #define ASCII_5_HEX_STRING ("\x05"s)
    #define ASCII_6_HEX_STRING ("\x06"s)
    #define ASCII_7_HEX_STRING ("\x07"s)
    #define ASCII_8_HEX_STRING ("\x08"s)
    #define ASCII_9_HEX_STRING ("\x09"s)
    #define ASCII_10_HEX_STRING ("\x0a"s)
    #define ASCII_11_HEX_STRING ("\x0b"s)
    #define ASCII_12_HEX_STRING ("\x0c"s)
    #define ASCII_13_HEX_STRING ("\x0d"s)
    #define ASCII_14_HEX_STRING ("\x0e"s)
    #define ASCII_15_HEX_STRING ("\x0f"s)
    #define ASCII_16_HEX_STRING ("\x10"s)
    #define ASCII_17_HEX_STRING ("\x11"s)
    #define ASCII_18_HEX_STRING ("\x12"s)
    #define ASCII_19_HEX_STRING ("\x13"s)
    #define ASCII_20_HEX_STRING ("\x14"s)
    #define ASCII_21_HEX_STRING ("\x15"s)
    #define ASCII_22_HEX_STRING ("\x16"s)
    #define ASCII_23_HEX_STRING ("\x17"s)
    #define ASCII_24_HEX_STRING ("\x18"s)
    #define ASCII_25_HEX_STRING ("\x19"s)
    #define ASCII_26_HEX_STRING ("\x1a"s)
    #define ASCII_27_HEX_STRING ("\x1b"s)
    #define ASCII_28_HEX_STRING ("\x1c"s)
    #define ASCII_29_HEX_STRING ("\x1d"s)
    #define ASCII_30_HEX_STRING ("\x1e"s)
    #define ASCII_31_HEX_STRING ("\x1f"s)
    #define ASCII_32_HEX_STRING ("\x20"s)
    #define ASCII_33_HEX_STRING ("\x21"s)
    #define ASCII_34_HEX_STRING ("\x22"s)
    #define ASCII_35_HEX_STRING ("\x23"s)
    #define ASCII_36_HEX_STRING ("\x24"s)
    #define ASCII_37_HEX_STRING ("\x25"s)
    #define ASCII_38_HEX_STRING ("\x26"s)
    #define ASCII_39_HEX_STRING ("\x27"s)
    #define ASCII_40_HEX_STRING ("\x28"s)
    #define ASCII_41_HEX_STRING ("\x29"s)
    #define ASCII_42_HEX_STRING ("\x2a"s)
    #define ASCII_43_HEX_STRING ("\x2b"s)
    #define ASCII_44_HEX_STRING ("\x2c"s)
    #define ASCII_45_HEX_STRING ("\x2d"s)
    #define ASCII_46_HEX_STRING ("\x2e"s)
    #define ASCII_47_HEX_STRING ("\x2f"s)
    #define ASCII_48_HEX_STRING ("\x30"s)
    #define ASCII_49_HEX_STRING ("\x31"s)
    #define ASCII_50_HEX_STRING ("\x32"s)
    #define ASCII_51_HEX_STRING ("\x33"s)
    #define ASCII_52_HEX_STRING ("\x34"s)
    #define ASCII_53_HEX_STRING ("\x35"s)
    #define ASCII_54_HEX_STRING ("\x36"s)
    #define ASCII_55_HEX_STRING ("\x37"s)
    #define ASCII_56_HEX_STRING ("\x38"s)
    #define ASCII_57_HEX_STRING ("\x39"s)
    #define ASCII_58_HEX_STRING ("\x3a"s)
    #define ASCII_59_HEX_STRING ("\x3b"s)
    #define ASCII_60_HEX_STRING ("\x3c"s)
    #define ASCII_61_HEX_STRING ("\x3d"s)
    #define ASCII_62_HEX_STRING ("\x3e"s)
    #define ASCII_63_HEX_STRING ("\x3f"s)
    #define ASCII_64_HEX_STRING ("\x40"s)
    #define ASCII_65_HEX_STRING ("\x41"s)
    #define ASCII_66_HEX_STRING ("\x42"s)
    #define ASCII_67_HEX_STRING ("\x43"s)
    #define ASCII_68_HEX_STRING ("\x44"s)
    #define ASCII_69_HEX_STRING ("\x45"s)
    #define ASCII_70_HEX_STRING ("\x46"s)
    #define ASCII_71_HEX_STRING ("\x47"s)
    #define ASCII_72_HEX_STRING ("\x48"s)
    #define ASCII_73_HEX_STRING ("\x49"s)
    #define ASCII_74_HEX_STRING ("\x4a"s)
    #define ASCII_75_HEX_STRING ("\x4b"s)
    #define ASCII_76_HEX_STRING ("\x4c"s)
    #define ASCII_77_HEX_STRING ("\x4d"s)
    #define ASCII_78_HEX_STRING ("\x4e"s)
    #define ASCII_79_HEX_STRING ("\x4f"s)
    #define ASCII_80_HEX_STRING ("\x50"s)
    #define ASCII_81_HEX_STRING ("\x51"s)
    #define ASCII_82_HEX_STRING ("\x52"s)
    #define ASCII_83_HEX_STRING ("\x53"s)
    #define ASCII_84_HEX_STRING ("\x54"s)
    #define ASCII_85_HEX_STRING ("\x55"s)
    #define ASCII_86_HEX_STRING ("\x56"s)
    #define ASCII_87_HEX_STRING ("\x57"s)
    #define ASCII_88_HEX_STRING ("\x58"s)
    #define ASCII_89_HEX_STRING ("\x59"s)
    #define ASCII_90_HEX_STRING ("\x5a"s)
    #define ASCII_91_HEX_STRING ("\x5b"s)
    #define ASCII_92_HEX_STRING ("\x5c"s)
    #define ASCII_93_HEX_STRING ("\x5d"s)
    #define ASCII_94_HEX_STRING ("\x5e"s)
    #define ASCII_95_HEX_STRING ("\x5f"s)
    #define ASCII_96_HEX_STRING ("\x60"s)
    #define ASCII_97_HEX_STRING ("\x61"s)
    #define ASCII_98_HEX_STRING ("\x62"s)
    #define ASCII_99_HEX_STRING ("\x63"s)
    #define ASCII_100_HEX_STRING ("\x64"s)
    #define ASCII_101_HEX_STRING ("\x65"s)
    #define ASCII_102_HEX_STRING ("\x66"s)
    #define ASCII_103_HEX_STRING ("\x67"s)
    #define ASCII_104_HEX_STRING ("\x68"s)
    #define ASCII_105_HEX_STRING ("\x69"s)
    #define ASCII_106_HEX_STRING ("\x6a"s)
    #define ASCII_107_HEX_STRING ("\x6b"s)
    #define ASCII_108_HEX_STRING ("\x6c"s)
    #define ASCII_109_HEX_STRING ("\x6d"s)
    #define ASCII_110_HEX_STRING ("\x6e"s)
    #define ASCII_111_HEX_STRING ("\x6f"s)
    #define ASCII_112_HEX_STRING ("\x70"s)
    #define ASCII_113_HEX_STRING ("\x71"s)
    #define ASCII_114_HEX_STRING ("\x72"s)
    #define ASCII_115_HEX_STRING ("\x73"s)
    #define ASCII_116_HEX_STRING ("\x74"s)
    #define ASCII_117_HEX_STRING ("\x75"s)
    #define ASCII_118_HEX_STRING ("\x76"s)
    #define ASCII_119_HEX_STRING ("\x77"s)
    #define ASCII_120_HEX_STRING ("\x78"s)
    #define ASCII_121_HEX_STRING ("\x79"s)
    #define ASCII_122_HEX_STRING ("\x7a"s)
    #define ASCII_123_HEX_STRING ("\x7b"s)
    #define ASCII_124_HEX_STRING ("\x7c"s)
    #define ASCII_125_HEX_STRING ("\x7d"s)
    #define ASCII_126_HEX_STRING ("\x7e"s)
    #define ASCII_127_HEX_STRING ("\x7f"s)
    #define ASCII_128_HEX_STRING ("\x80"s)
    #define ASCII_129_HEX_STRING ("\x81"s)
    #define ASCII_130_HEX_STRING ("\x82"s)
    #define ASCII_131_HEX_STRING ("\x83"s)
    #define ASCII_132_HEX_STRING ("\x84"s)
    #define ASCII_133_HEX_STRING ("\x85"s)
    #define ASCII_134_HEX_STRING ("\x86"s)
    #define ASCII_135_HEX_STRING ("\x87"s)
    #define ASCII_136_HEX_STRING ("\x88"s)
    #define ASCII_137_HEX_STRING ("\x89"s)
    #define ASCII_138_HEX_STRING ("\x8a"s)
    #define ASCII_139_HEX_STRING ("\x8b"s)
    #define ASCII_140_HEX_STRING ("\x8c"s)
    #define ASCII_141_HEX_STRING ("\x8d"s)
    #define ASCII_142_HEX_STRING ("\x8e"s)
    #define ASCII_143_HEX_STRING ("\x8f"s)
    #define ASCII_144_HEX_STRING ("\x90"s)
    #define ASCII_145_HEX_STRING ("\x91"s)
    #define ASCII_146_HEX_STRING ("\x92"s)
    #define ASCII_147_HEX_STRING ("\x93"s)
    #define ASCII_148_HEX_STRING ("\x94"s)
    #define ASCII_149_HEX_STRING ("\x95"s)
    #define ASCII_150_HEX_STRING ("\x96"s)
    #define ASCII_151_HEX_STRING ("\x97"s)
    #define ASCII_152_HEX_STRING ("\x98"s)
    #define ASCII_153_HEX_STRING ("\x99"s)
    #define ASCII_154_HEX_STRING ("\x9a"s)
    #define ASCII_155_HEX_STRING ("\x9b"s)
    #define ASCII_156_HEX_STRING ("\x9c"s)
    #define ASCII_157_HEX_STRING ("\x9d"s)
    #define ASCII_158_HEX_STRING ("\x9e"s)
    #define ASCII_159_HEX_STRING ("\x9f"s)
    #define ASCII_160_HEX_STRING ("\xa0"s)
    #define ASCII_161_HEX_STRING ("\xa1"s)
    #define ASCII_162_HEX_STRING ("\xa2"s)
    #define ASCII_163_HEX_STRING ("\xa3"s)
    #define ASCII_164_HEX_STRING ("\xa4"s)
    #define ASCII_165_HEX_STRING ("\xa5"s)
    #define ASCII_166_HEX_STRING ("\xa6"s)
    #define ASCII_167_HEX_STRING ("\xa7"s)
    #define ASCII_168_HEX_STRING ("\xa8"s)
    #define ASCII_169_HEX_STRING ("\xa9"s)
    #define ASCII_170_HEX_STRING ("\xaa"s)
    #define ASCII_171_HEX_STRING ("\xab"s)
    #define ASCII_172_HEX_STRING ("\xac"s)
    #define ASCII_173_HEX_STRING ("\xad"s)
    #define ASCII_174_HEX_STRING ("\xae"s)
    #define ASCII_175_HEX_STRING ("\xaf"s)
    #define ASCII_176_HEX_STRING ("\xb0"s)
    #define ASCII_177_HEX_STRING ("\xb1"s)
    #define ASCII_178_HEX_STRING ("\xb2"s)
    #define ASCII_179_HEX_STRING ("\xb3"s)
    #define ASCII_180_HEX_STRING ("\xb4"s)
    #define ASCII_181_HEX_STRING ("\xb5"s)
    #define ASCII_182_HEX_STRING ("\xb6"s)
    #define ASCII_183_HEX_STRING ("\xb7"s)
    #define ASCII_184_HEX_STRING ("\xb8"s)
    #define ASCII_185_HEX_STRING ("\xb9"s)
    #define ASCII_186_HEX_STRING ("\xba"s)
    #define ASCII_187_HEX_STRING ("\xbb"s)
    #define ASCII_188_HEX_STRING ("\xbc"s)
    #define ASCII_189_HEX_STRING ("\xbd"s)
    #define ASCII_190_HEX_STRING ("\xbe"s)
    #define ASCII_191_HEX_STRING ("\xbf"s)
    #define ASCII_192_HEX_STRING ("\xc0"s)
    #define ASCII_193_HEX_STRING ("\xc1"s)
    #define ASCII_194_HEX_STRING ("\xc2"s)
    #define ASCII_195_HEX_STRING ("\xc3"s)
    #define ASCII_196_HEX_STRING ("\xc4"s)
    #define ASCII_197_HEX_STRING ("\xc5"s)
    #define ASCII_198_HEX_STRING ("\xc6"s)
    #define ASCII_199_HEX_STRING ("\xc7"s)
    #define ASCII_200_HEX_STRING ("\xc8"s)
    #define ASCII_201_HEX_STRING ("\xc9"s)
    #define ASCII_202_HEX_STRING ("\xca"s)
    #define ASCII_203_HEX_STRING ("\xcb"s)
    #define ASCII_204_HEX_STRING ("\xcc"s)
    #define ASCII_205_HEX_STRING ("\xcd"s)
    #define ASCII_206_HEX_STRING ("\xce"s)
    #define ASCII_207_HEX_STRING ("\xcf"s)
    #define ASCII_208_HEX_STRING ("\xd0"s)
    #define ASCII_209_HEX_STRING ("\xd1"s)
    #define ASCII_210_HEX_STRING ("\xd2"s)
    #define ASCII_211_HEX_STRING ("\xd3"s)
    #define ASCII_212_HEX_STRING ("\xd4"s)
    #define ASCII_213_HEX_STRING ("\xd5"s)
    #define ASCII_214_HEX_STRING ("\xd6"s)
    #define ASCII_215_HEX_STRING ("\xd7"s)
    #define ASCII_216_HEX_STRING ("\xd8"s)
    #define ASCII_217_HEX_STRING ("\xd9"s)
    #define ASCII_218_HEX_STRING ("\xda"s)
    #define ASCII_219_HEX_STRING ("\xdb"s)
    #define ASCII_220_HEX_STRING ("\xdc"s)
    #define ASCII_221_HEX_STRING ("\xdd"s)
    #define ASCII_222_HEX_STRING ("\xde"s)
    #define ASCII_223_HEX_STRING ("\xdf"s)
    #define ASCII_224_HEX_STRING ("\xe0"s)
    #define ASCII_225_HEX_STRING ("\xe1"s)
    #define ASCII_226_HEX_STRING ("\xe2"s)
    #define ASCII_227_HEX_STRING ("\xe3"s)
    #define ASCII_228_HEX_STRING ("\xe4"s)
    #define ASCII_229_HEX_STRING ("\xe5"s)
    #define ASCII_230_HEX_STRING ("\xe6"s)
    #define ASCII_231_HEX_STRING ("\xe7"s)
    #define ASCII_232_HEX_STRING ("\xe8"s)
    #define ASCII_233_HEX_STRING ("\xe9"s)
    #define ASCII_234_HEX_STRING ("\xea"s)
    #define ASCII_235_HEX_STRING ("\xeb"s)
    #define ASCII_236_HEX_STRING ("\xec"s)
    #define ASCII_237_HEX_STRING ("\xed"s)
    #define ASCII_238_HEX_STRING ("\xee"s)
    #define ASCII_239_HEX_STRING ("\xef"s)
    #define ASCII_240_HEX_STRING ("\xf0"s)
    #define ASCII_241_HEX_STRING ("\xf1"s)
    #define ASCII_242_HEX_STRING ("\xf2"s)
    #define ASCII_243_HEX_STRING ("\xf3"s)
    #define ASCII_244_HEX_STRING ("\xf4"s)
    #define ASCII_245_HEX_STRING ("\xf5"s)
    #define ASCII_246_HEX_STRING ("\xf6"s)
    #define ASCII_247_HEX_STRING ("\xf7"s)
    #define ASCII_248_HEX_STRING ("\xf8"s)
    #define ASCII_249_HEX_STRING ("\xf9"s)
    #define ASCII_250_HEX_STRING ("\xfa"s)
    #define ASCII_251_HEX_STRING ("\xfb"s)
    #define ASCII_252_HEX_STRING ("\xfc"s)
    #define ASCII_253_HEX_STRING ("\xfd"s)
    #define ASCII_254_HEX_STRING ("\xfe"s)
    #define ASCII_255_HEX_STRING ("\xff"s)

    #define ASCII_NEGATIVE_128_HEX_CHAR ('\x80')
    #define ASCII_NEGATIVE_127_HEX_CHAR ('\x81')
    #define ASCII_NEGATIVE_126_HEX_CHAR ('\x82')
    #define ASCII_NEGATIVE_125_HEX_CHAR ('\x83')
    #define ASCII_NEGATIVE_124_HEX_CHAR ('\x84')
    #define ASCII_NEGATIVE_123_HEX_CHAR ('\x85')
    #define ASCII_NEGATIVE_122_HEX_CHAR ('\x86')
    #define ASCII_NEGATIVE_121_HEX_CHAR ('\x87')
    #define ASCII_NEGATIVE_120_HEX_CHAR ('\x88')
    #define ASCII_NEGATIVE_119_HEX_CHAR ('\x89')
    #define ASCII_NEGATIVE_118_HEX_CHAR ('\x8a')
    #define ASCII_NEGATIVE_117_HEX_CHAR ('\x8b')
    #define ASCII_NEGATIVE_116_HEX_CHAR ('\x8c')
    #define ASCII_NEGATIVE_115_HEX_CHAR ('\x8d')
    #define ASCII_NEGATIVE_114_HEX_CHAR ('\x8e')
    #define ASCII_NEGATIVE_113_HEX_CHAR ('\x8f')
    #define ASCII_NEGATIVE_112_HEX_CHAR ('\x90')
    #define ASCII_NEGATIVE_111_HEX_CHAR ('\x91')
    #define ASCII_NEGATIVE_110_HEX_CHAR ('\x92')
    #define ASCII_NEGATIVE_109_HEX_CHAR ('\x93')
    #define ASCII_NEGATIVE_108_HEX_CHAR ('\x94')
    #define ASCII_NEGATIVE_107_HEX_CHAR ('\x95')
    #define ASCII_NEGATIVE_106_HEX_CHAR ('\x96')
    #define ASCII_NEGATIVE_105_HEX_CHAR ('\x97')
    #define ASCII_NEGATIVE_104_HEX_CHAR ('\x98')
    #define ASCII_NEGATIVE_103_HEX_CHAR ('\x99')
    #define ASCII_NEGATIVE_102_HEX_CHAR ('\x9a')
    #define ASCII_NEGATIVE_101_HEX_CHAR ('\x9b')
    #define ASCII_NEGATIVE_100_HEX_CHAR ('\x9c')
    #define ASCII_NEGATIVE_99_HEX_CHAR ('\x9d')
    #define ASCII_NEGATIVE_98_HEX_CHAR ('\x9e')
    #define ASCII_NEGATIVE_97_HEX_CHAR ('\x9f')
    #define ASCII_NEGATIVE_96_HEX_CHAR ('\xa0')
    #define ASCII_NEGATIVE_95_HEX_CHAR ('\xa1')
    #define ASCII_NEGATIVE_94_HEX_CHAR ('\xa2')
    #define ASCII_NEGATIVE_93_HEX_CHAR ('\xa3')
    #define ASCII_NEGATIVE_92_HEX_CHAR ('\xa4')
    #define ASCII_NEGATIVE_91_HEX_CHAR ('\xa5')
    #define ASCII_NEGATIVE_90_HEX_CHAR ('\xa6')
    #define ASCII_NEGATIVE_89_HEX_CHAR ('\xa7')
    #define ASCII_NEGATIVE_88_HEX_CHAR ('\xa8')
    #define ASCII_NEGATIVE_87_HEX_CHAR ('\xa9')
    #define ASCII_NEGATIVE_86_HEX_CHAR ('\xaa')
    #define ASCII_NEGATIVE_85_HEX_CHAR ('\xab')
    #define ASCII_NEGATIVE_84_HEX_CHAR ('\xac')
    #define ASCII_NEGATIVE_83_HEX_CHAR ('\xad')
    #define ASCII_NEGATIVE_82_HEX_CHAR ('\xae')
    #define ASCII_NEGATIVE_81_HEX_CHAR ('\xaf')
    #define ASCII_NEGATIVE_80_HEX_CHAR ('\xb0')
    #define ASCII_NEGATIVE_79_HEX_CHAR ('\xb1')
    #define ASCII_NEGATIVE_78_HEX_CHAR ('\xb2')
    #define ASCII_NEGATIVE_77_HEX_CHAR ('\xb3')
    #define ASCII_NEGATIVE_76_HEX_CHAR ('\xb4')
    #define ASCII_NEGATIVE_75_HEX_CHAR ('\xb5')
    #define ASCII_NEGATIVE_74_HEX_CHAR ('\xb6')
    #define ASCII_NEGATIVE_73_HEX_CHAR ('\xb7')
    #define ASCII_NEGATIVE_72_HEX_CHAR ('\xb8')
    #define ASCII_NEGATIVE_71_HEX_CHAR ('\xb9')
    #define ASCII_NEGATIVE_70_HEX_CHAR ('\xba')
    #define ASCII_NEGATIVE_69_HEX_CHAR ('\xbb')
    #define ASCII_NEGATIVE_68_HEX_CHAR ('\xbc')
    #define ASCII_NEGATIVE_67_HEX_CHAR ('\xbd')
    #define ASCII_NEGATIVE_66_HEX_CHAR ('\xbe')
    #define ASCII_NEGATIVE_65_HEX_CHAR ('\xbf')
    #define ASCII_NEGATIVE_64_HEX_CHAR ('\xc0')
    #define ASCII_NEGATIVE_63_HEX_CHAR ('\xc1')
    #define ASCII_NEGATIVE_62_HEX_CHAR ('\xc2')
    #define ASCII_NEGATIVE_61_HEX_CHAR ('\xc3')
    #define ASCII_NEGATIVE_60_HEX_CHAR ('\xc4')
    #define ASCII_NEGATIVE_59_HEX_CHAR ('\xc5')
    #define ASCII_NEGATIVE_58_HEX_CHAR ('\xc6')
    #define ASCII_NEGATIVE_57_HEX_CHAR ('\xc7')
    #define ASCII_NEGATIVE_56_HEX_CHAR ('\xc8')
    #define ASCII_NEGATIVE_55_HEX_CHAR ('\xc9')
    #define ASCII_NEGATIVE_54_HEX_CHAR ('\xca')
    #define ASCII_NEGATIVE_53_HEX_CHAR ('\xcb')
    #define ASCII_NEGATIVE_52_HEX_CHAR ('\xcc')
    #define ASCII_NEGATIVE_51_HEX_CHAR ('\xcd')
    #define ASCII_NEGATIVE_50_HEX_CHAR ('\xce')
    #define ASCII_NEGATIVE_49_HEX_CHAR ('\xcf')
    #define ASCII_NEGATIVE_48_HEX_CHAR ('\xd0')
    #define ASCII_NEGATIVE_47_HEX_CHAR ('\xd1')
    #define ASCII_NEGATIVE_46_HEX_CHAR ('\xd2')
    #define ASCII_NEGATIVE_45_HEX_CHAR ('\xd3')
    #define ASCII_NEGATIVE_44_HEX_CHAR ('\xd4')
    #define ASCII_NEGATIVE_43_HEX_CHAR ('\xd5')
    #define ASCII_NEGATIVE_42_HEX_CHAR ('\xd6')
    #define ASCII_NEGATIVE_41_HEX_CHAR ('\xd7')
    #define ASCII_NEGATIVE_40_HEX_CHAR ('\xd8')
    #define ASCII_NEGATIVE_39_HEX_CHAR ('\xd9')
    #define ASCII_NEGATIVE_38_HEX_CHAR ('\xda')
    #define ASCII_NEGATIVE_37_HEX_CHAR ('\xdb')
    #define ASCII_NEGATIVE_36_HEX_CHAR ('\xdc')
    #define ASCII_NEGATIVE_35_HEX_CHAR ('\xdd')
    #define ASCII_NEGATIVE_34_HEX_CHAR ('\xde')
    #define ASCII_NEGATIVE_33_HEX_CHAR ('\xdf')
    #define ASCII_NEGATIVE_32_HEX_CHAR ('\xe0')
    #define ASCII_NEGATIVE_31_HEX_CHAR ('\xe1')
    #define ASCII_NEGATIVE_30_HEX_CHAR ('\xe2')
    #define ASCII_NEGATIVE_29_HEX_CHAR ('\xe3')
    #define ASCII_NEGATIVE_28_HEX_CHAR ('\xe4')
    #define ASCII_NEGATIVE_27_HEX_CHAR ('\xe5')
    #define ASCII_NEGATIVE_26_HEX_CHAR ('\xe6')
    #define ASCII_NEGATIVE_25_HEX_CHAR ('\xe7')
    #define ASCII_NEGATIVE_24_HEX_CHAR ('\xe8')
    #define ASCII_NEGATIVE_23_HEX_CHAR ('\xe9')
    #define ASCII_NEGATIVE_22_HEX_CHAR ('\xea')
    #define ASCII_NEGATIVE_21_HEX_CHAR ('\xeb')
    #define ASCII_NEGATIVE_20_HEX_CHAR ('\xec')
    #define ASCII_NEGATIVE_19_HEX_CHAR ('\xed')
    #define ASCII_NEGATIVE_18_HEX_CHAR ('\xee')
    #define ASCII_NEGATIVE_17_HEX_CHAR ('\xef')
    #define ASCII_NEGATIVE_16_HEX_CHAR ('\xf0')
    #define ASCII_NEGATIVE_15_HEX_CHAR ('\xf1')
    #define ASCII_NEGATIVE_14_HEX_CHAR ('\xf2')
    #define ASCII_NEGATIVE_13_HEX_CHAR ('\xf3')
    #define ASCII_NEGATIVE_12_HEX_CHAR ('\xf4')
    #define ASCII_NEGATIVE_11_HEX_CHAR ('\xf5')
    #define ASCII_NEGATIVE_10_HEX_CHAR ('\xf6')
    #define ASCII_NEGATIVE_9_HEX_CHAR ('\xf7')
    #define ASCII_NEGATIVE_8_HEX_CHAR ('\xf8')
    #define ASCII_NEGATIVE_7_HEX_CHAR ('\xf9')
    #define ASCII_NEGATIVE_6_HEX_CHAR ('\xfa')
    #define ASCII_NEGATIVE_5_HEX_CHAR ('\xfb')
    #define ASCII_NEGATIVE_4_HEX_CHAR ('\xfc')
    #define ASCII_NEGATIVE_3_HEX_CHAR ('\xfd')
    #define ASCII_NEGATIVE_2_HEX_CHAR ('\xfe')
    #define ASCII_NEGATIVE_1_HEX_CHAR ('\xff')

    #define ASCII_0_HEX_CHAR ('\x00')
    #define ASCII_1_HEX_CHAR ('\x01')
    #define ASCII_2_HEX_CHAR ('\x02')
    #define ASCII_3_HEX_CHAR ('\x03')
    #define ASCII_4_HEX_CHAR ('\x04')
    #define ASCII_5_HEX_CHAR ('\x05')
    #define ASCII_6_HEX_CHAR ('\x06')
    #define ASCII_7_HEX_CHAR ('\x07')
    #define ASCII_8_HEX_CHAR ('\x08')
    #define ASCII_9_HEX_CHAR ('\x09')
    #define ASCII_10_HEX_CHAR ('\x0a')
    #define ASCII_11_HEX_CHAR ('\x0b')
    #define ASCII_12_HEX_CHAR ('\x0c')
    #define ASCII_13_HEX_CHAR ('\x0d')
    #define ASCII_14_HEX_CHAR ('\x0e')
    #define ASCII_15_HEX_CHAR ('\x0f')
    #define ASCII_16_HEX_CHAR ('\x10')
    #define ASCII_17_HEX_CHAR ('\x11')
    #define ASCII_18_HEX_CHAR ('\x12')
    #define ASCII_19_HEX_CHAR ('\x13')
    #define ASCII_20_HEX_CHAR ('\x14')
    #define ASCII_21_HEX_CHAR ('\x15')
    #define ASCII_22_HEX_CHAR ('\x16')
    #define ASCII_23_HEX_CHAR ('\x17')
    #define ASCII_24_HEX_CHAR ('\x18')
    #define ASCII_25_HEX_CHAR ('\x19')
    #define ASCII_26_HEX_CHAR ('\x1a')
    #define ASCII_27_HEX_CHAR ('\x1b')
    #define ASCII_28_HEX_CHAR ('\x1c')
    #define ASCII_29_HEX_CHAR ('\x1d')
    #define ASCII_30_HEX_CHAR ('\x1e')
    #define ASCII_31_HEX_CHAR ('\x1f')
    #define ASCII_32_HEX_CHAR ('\x20')
    #define ASCII_33_HEX_CHAR ('\x21')
    #define ASCII_34_HEX_CHAR ('\x22')
    #define ASCII_35_HEX_CHAR ('\x23')
    #define ASCII_36_HEX_CHAR ('\x24')
    #define ASCII_37_HEX_CHAR ('\x25')
    #define ASCII_38_HEX_CHAR ('\x26')
    #define ASCII_39_HEX_CHAR ('\x27')
    #define ASCII_40_HEX_CHAR ('\x28')
    #define ASCII_41_HEX_CHAR ('\x29')
    #define ASCII_42_HEX_CHAR ('\x2a')
    #define ASCII_43_HEX_CHAR ('\x2b')
    #define ASCII_44_HEX_CHAR ('\x2c')
    #define ASCII_45_HEX_CHAR ('\x2d')
    #define ASCII_46_HEX_CHAR ('\x2e')
    #define ASCII_47_HEX_CHAR ('\x2f')
    #define ASCII_48_HEX_CHAR ('\x30')
    #define ASCII_49_HEX_CHAR ('\x31')
    #define ASCII_50_HEX_CHAR ('\x32')
    #define ASCII_51_HEX_CHAR ('\x33')
    #define ASCII_52_HEX_CHAR ('\x34')
    #define ASCII_53_HEX_CHAR ('\x35')
    #define ASCII_54_HEX_CHAR ('\x36')
    #define ASCII_55_HEX_CHAR ('\x37')
    #define ASCII_56_HEX_CHAR ('\x38')
    #define ASCII_57_HEX_CHAR ('\x39')
    #define ASCII_58_HEX_CHAR ('\x3a')
    #define ASCII_59_HEX_CHAR ('\x3b')
    #define ASCII_60_HEX_CHAR ('\x3c')
    #define ASCII_61_HEX_CHAR ('\x3d')
    #define ASCII_62_HEX_CHAR ('\x3e')
    #define ASCII_63_HEX_CHAR ('\x3f')
    #define ASCII_64_HEX_CHAR ('\x40')
    #define ASCII_65_HEX_CHAR ('\x41')
    #define ASCII_66_HEX_CHAR ('\x42')
    #define ASCII_67_HEX_CHAR ('\x43')
    #define ASCII_68_HEX_CHAR ('\x44')
    #define ASCII_69_HEX_CHAR ('\x45')
    #define ASCII_70_HEX_CHAR ('\x46')
    #define ASCII_71_HEX_CHAR ('\x47')
    #define ASCII_72_HEX_CHAR ('\x48')
    #define ASCII_73_HEX_CHAR ('\x49')
    #define ASCII_74_HEX_CHAR ('\x4a')
    #define ASCII_75_HEX_CHAR ('\x4b')
    #define ASCII_76_HEX_CHAR ('\x4c')
    #define ASCII_77_HEX_CHAR ('\x4d')
    #define ASCII_78_HEX_CHAR ('\x4e')
    #define ASCII_79_HEX_CHAR ('\x4f')
    #define ASCII_80_HEX_CHAR ('\x50')
    #define ASCII_81_HEX_CHAR ('\x51')
    #define ASCII_82_HEX_CHAR ('\x52')
    #define ASCII_83_HEX_CHAR ('\x53')
    #define ASCII_84_HEX_CHAR ('\x54')
    #define ASCII_85_HEX_CHAR ('\x55')
    #define ASCII_86_HEX_CHAR ('\x56')
    #define ASCII_87_HEX_CHAR ('\x57')
    #define ASCII_88_HEX_CHAR ('\x58')
    #define ASCII_89_HEX_CHAR ('\x59')
    #define ASCII_90_HEX_CHAR ('\x5a')
    #define ASCII_91_HEX_CHAR ('\x5b')
    #define ASCII_92_HEX_CHAR ('\x5c')
    #define ASCII_93_HEX_CHAR ('\x5d')
    #define ASCII_94_HEX_CHAR ('\x5e')
    #define ASCII_95_HEX_CHAR ('\x5f')
    #define ASCII_96_HEX_CHAR ('\x60')
    #define ASCII_97_HEX_CHAR ('\x61')
    #define ASCII_98_HEX_CHAR ('\x62')
    #define ASCII_99_HEX_CHAR ('\x63')
    #define ASCII_100_HEX_CHAR ('\x64')
    #define ASCII_101_HEX_CHAR ('\x65')
    #define ASCII_102_HEX_CHAR ('\x66')
    #define ASCII_103_HEX_CHAR ('\x67')
    #define ASCII_104_HEX_CHAR ('\x68')
    #define ASCII_105_HEX_CHAR ('\x69')
    #define ASCII_106_HEX_CHAR ('\x6a')
    #define ASCII_107_HEX_CHAR ('\x6b')
    #define ASCII_108_HEX_CHAR ('\x6c')
    #define ASCII_109_HEX_CHAR ('\x6d')
    #define ASCII_110_HEX_CHAR ('\x6e')
    #define ASCII_111_HEX_CHAR ('\x6f')
    #define ASCII_112_HEX_CHAR ('\x70')
    #define ASCII_113_HEX_CHAR ('\x71')
    #define ASCII_114_HEX_CHAR ('\x72')
    #define ASCII_115_HEX_CHAR ('\x73')
    #define ASCII_116_HEX_CHAR ('\x74')
    #define ASCII_117_HEX_CHAR ('\x75')
    #define ASCII_118_HEX_CHAR ('\x76')
    #define ASCII_119_HEX_CHAR ('\x77')
    #define ASCII_120_HEX_CHAR ('\x78')
    #define ASCII_121_HEX_CHAR ('\x79')
    #define ASCII_122_HEX_CHAR ('\x7a')
    #define ASCII_123_HEX_CHAR ('\x7b')
    #define ASCII_124_HEX_CHAR ('\x7c')
    #define ASCII_125_HEX_CHAR ('\x7d')
    #define ASCII_126_HEX_CHAR ('\x7e')
    #define ASCII_127_HEX_CHAR ('\x7f')
    #define ASCII_128_HEX_CHAR ('\x80')
    #define ASCII_129_HEX_CHAR ('\x81')
    #define ASCII_130_HEX_CHAR ('\x82')
    #define ASCII_131_HEX_CHAR ('\x83')
    #define ASCII_132_HEX_CHAR ('\x84')
    #define ASCII_133_HEX_CHAR ('\x85')
    #define ASCII_134_HEX_CHAR ('\x86')
    #define ASCII_135_HEX_CHAR ('\x87')
    #define ASCII_136_HEX_CHAR ('\x88')
    #define ASCII_137_HEX_CHAR ('\x89')
    #define ASCII_138_HEX_CHAR ('\x8a')
    #define ASCII_139_HEX_CHAR ('\x8b')
    #define ASCII_140_HEX_CHAR ('\x8c')
    #define ASCII_141_HEX_CHAR ('\x8d')
    #define ASCII_142_HEX_CHAR ('\x8e')
    #define ASCII_143_HEX_CHAR ('\x8f')
    #define ASCII_144_HEX_CHAR ('\x90')
    #define ASCII_145_HEX_CHAR ('\x91')
    #define ASCII_146_HEX_CHAR ('\x92')
    #define ASCII_147_HEX_CHAR ('\x93')
    #define ASCII_148_HEX_CHAR ('\x94')
    #define ASCII_149_HEX_CHAR ('\x95')
    #define ASCII_150_HEX_CHAR ('\x96')
    #define ASCII_151_HEX_CHAR ('\x97')
    #define ASCII_152_HEX_CHAR ('\x98')
    #define ASCII_153_HEX_CHAR ('\x99')
    #define ASCII_154_HEX_CHAR ('\x9a')
    #define ASCII_155_HEX_CHAR ('\x9b')
    #define ASCII_156_HEX_CHAR ('\x9c')
    #define ASCII_157_HEX_CHAR ('\x9d')
    #define ASCII_158_HEX_CHAR ('\x9e')
    #define ASCII_159_HEX_CHAR ('\x9f')
    #define ASCII_160_HEX_CHAR ('\xa0')
    #define ASCII_161_HEX_CHAR ('\xa1')
    #define ASCII_162_HEX_CHAR ('\xa2')
    #define ASCII_163_HEX_CHAR ('\xa3')
    #define ASCII_164_HEX_CHAR ('\xa4')
    #define ASCII_165_HEX_CHAR ('\xa5')
    #define ASCII_166_HEX_CHAR ('\xa6')
    #define ASCII_167_HEX_CHAR ('\xa7')
    #define ASCII_168_HEX_CHAR ('\xa8')
    #define ASCII_169_HEX_CHAR ('\xa9')
    #define ASCII_170_HEX_CHAR ('\xaa')
    #define ASCII_171_HEX_CHAR ('\xab')
    #define ASCII_172_HEX_CHAR ('\xac')
    #define ASCII_173_HEX_CHAR ('\xad')
    #define ASCII_174_HEX_CHAR ('\xae')
    #define ASCII_175_HEX_CHAR ('\xaf')
    #define ASCII_176_HEX_CHAR ('\xb0')
    #define ASCII_177_HEX_CHAR ('\xb1')
    #define ASCII_178_HEX_CHAR ('\xb2')
    #define ASCII_179_HEX_CHAR ('\xb3')
    #define ASCII_180_HEX_CHAR ('\xb4')
    #define ASCII_181_HEX_CHAR ('\xb5')
    #define ASCII_182_HEX_CHAR ('\xb6')
    #define ASCII_183_HEX_CHAR ('\xb7')
    #define ASCII_184_HEX_CHAR ('\xb8')
    #define ASCII_185_HEX_CHAR ('\xb9')
    #define ASCII_186_HEX_CHAR ('\xba')
    #define ASCII_187_HEX_CHAR ('\xbb')
    #define ASCII_188_HEX_CHAR ('\xbc')
    #define ASCII_189_HEX_CHAR ('\xbd')
    #define ASCII_190_HEX_CHAR ('\xbe')
    #define ASCII_191_HEX_CHAR ('\xbf')
    #define ASCII_192_HEX_CHAR ('\xc0')
    #define ASCII_193_HEX_CHAR ('\xc1')
    #define ASCII_194_HEX_CHAR ('\xc2')
    #define ASCII_195_HEX_CHAR ('\xc3')
    #define ASCII_196_HEX_CHAR ('\xc4')
    #define ASCII_197_HEX_CHAR ('\xc5')
    #define ASCII_198_HEX_CHAR ('\xc6')
    #define ASCII_199_HEX_CHAR ('\xc7')
    #define ASCII_200_HEX_CHAR ('\xc8')
    #define ASCII_201_HEX_CHAR ('\xc9')
    #define ASCII_202_HEX_CHAR ('\xca')
    #define ASCII_203_HEX_CHAR ('\xcb')
    #define ASCII_204_HEX_CHAR ('\xcc')
    #define ASCII_205_HEX_CHAR ('\xcd')
    #define ASCII_206_HEX_CHAR ('\xce')
    #define ASCII_207_HEX_CHAR ('\xcf')
    #define ASCII_208_HEX_CHAR ('\xd0')
    #define ASCII_209_HEX_CHAR ('\xd1')
    #define ASCII_210_HEX_CHAR ('\xd2')
    #define ASCII_211_HEX_CHAR ('\xd3')
    #define ASCII_212_HEX_CHAR ('\xd4')
    #define ASCII_213_HEX_CHAR ('\xd5')
    #define ASCII_214_HEX_CHAR ('\xd6')
    #define ASCII_215_HEX_CHAR ('\xd7')
    #define ASCII_216_HEX_CHAR ('\xd8')
    #define ASCII_217_HEX_CHAR ('\xd9')
    #define ASCII_218_HEX_CHAR ('\xda')
    #define ASCII_219_HEX_CHAR ('\xdb')
    #define ASCII_220_HEX_CHAR ('\xdc')
    #define ASCII_221_HEX_CHAR ('\xdd')
    #define ASCII_222_HEX_CHAR ('\xde')
    #define ASCII_223_HEX_CHAR ('\xdf')
    #define ASCII_224_HEX_CHAR ('\xe0')
    #define ASCII_225_HEX_CHAR ('\xe1')
    #define ASCII_226_HEX_CHAR ('\xe2')
    #define ASCII_227_HEX_CHAR ('\xe3')
    #define ASCII_228_HEX_CHAR ('\xe4')
    #define ASCII_229_HEX_CHAR ('\xe5')
    #define ASCII_230_HEX_CHAR ('\xe6')
    #define ASCII_231_HEX_CHAR ('\xe7')
    #define ASCII_232_HEX_CHAR ('\xe8')
    #define ASCII_233_HEX_CHAR ('\xe9')
    #define ASCII_234_HEX_CHAR ('\xea')
    #define ASCII_235_HEX_CHAR ('\xeb')
    #define ASCII_236_HEX_CHAR ('\xec')
    #define ASCII_237_HEX_CHAR ('\xed')
    #define ASCII_238_HEX_CHAR ('\xee')
    #define ASCII_239_HEX_CHAR ('\xef')
    #define ASCII_240_HEX_CHAR ('\xf0')
    #define ASCII_241_HEX_CHAR ('\xf1')
    #define ASCII_242_HEX_CHAR ('\xf2')
    #define ASCII_243_HEX_CHAR ('\xf3')
    #define ASCII_244_HEX_CHAR ('\xf4')
    #define ASCII_245_HEX_CHAR ('\xf5')
    #define ASCII_246_HEX_CHAR ('\xf6')
    #define ASCII_247_HEX_CHAR ('\xf7')
    #define ASCII_248_HEX_CHAR ('\xf8')
    #define ASCII_249_HEX_CHAR ('\xf9')
    #define ASCII_250_HEX_CHAR ('\xfa')
    #define ASCII_251_HEX_CHAR ('\xfb')
    #define ASCII_252_HEX_CHAR ('\xfc')
    #define ASCII_253_HEX_CHAR ('\xfd')
    #define ASCII_254_HEX_CHAR ('\xfe')
    #define ASCII_255_HEX_CHAR ('\xff')

    #define INSANE_NEXT_0() 1
    #define INSANE_NEXT_1() 2
    #define INSANE_NEXT_2() 3
    #define INSANE_NEXT_3() 4
    #define INSANE_NEXT_4() 5
    #define INSANE_NEXT_5() 6
    #define INSANE_NEXT_6() 7
    #define INSANE_NEXT_7() 8
    #define INSANE_NEXT_8() 9
    #define INSANE_NEXT_9() 10
    #define INSANE_NEXT_10() 11
    #define INSANE_NEXT_11() 12
    #define INSANE_NEXT_12() 13
    #define INSANE_NEXT_13() 14
    #define INSANE_NEXT_14() 15
    #define INSANE_NEXT_15() 16
    #define INSANE_NEXT_16() 17
    #define INSANE_NEXT_17() 18
    #define INSANE_NEXT_18() 19
    #define INSANE_NEXT_19() 20
    #define INSANE_NEXT_20() 21
    #define INSANE_NEXT_21() 22
    #define INSANE_NEXT_22() 23
    #define INSANE_NEXT_23() 24
    #define INSANE_NEXT_24() 25
    #define INSANE_NEXT_25() 26
    #define INSANE_NEXT_26() 27
    #define INSANE_NEXT_27() 28
    #define INSANE_NEXT_28() 29
    #define INSANE_NEXT_29() 30
    #define INSANE_NEXT_30() 31
    #define INSANE_NEXT_31() 32
    #define INSANE_NEXT_32() 33
    #define INSANE_NEXT_33() 34
    #define INSANE_NEXT_34() 35
    #define INSANE_NEXT_35() 36
    #define INSANE_NEXT_36() 37
    #define INSANE_NEXT_37() 38
    #define INSANE_NEXT_38() 39
    #define INSANE_NEXT_39() 40
    #define INSANE_NEXT_40() 41
    #define INSANE_NEXT_41() 42
    #define INSANE_NEXT_42() 43
    #define INSANE_NEXT_43() 44
    #define INSANE_NEXT_44() 45
    #define INSANE_NEXT_45() 46
    #define INSANE_NEXT_46() 47
    #define INSANE_NEXT_47() 48
    #define INSANE_NEXT_48() 49
    #define INSANE_NEXT_49() 50
    #define INSANE_NEXT_50() 51
    #define INSANE_NEXT_51() 52
    #define INSANE_NEXT_52() 53
    #define INSANE_NEXT_53() 54
    #define INSANE_NEXT_54() 55
    #define INSANE_NEXT_55() 56
    #define INSANE_NEXT_56() 57
    #define INSANE_NEXT_57() 58
    #define INSANE_NEXT_58() 59
    #define INSANE_NEXT_59() 60
    #define INSANE_NEXT_60() 61
    #define INSANE_NEXT_61() 62
    #define INSANE_NEXT_62() 63
    #define INSANE_NEXT_63() 64
    #define INSANE_NEXT_64() 65
    #define INSANE_NEXT_65() 66
    #define INSANE_NEXT_66() 67
    #define INSANE_NEXT_67() 68
    #define INSANE_NEXT_68() 69
    #define INSANE_NEXT_69() 70
    #define INSANE_NEXT_70() 71
    #define INSANE_NEXT_71() 72
    #define INSANE_NEXT_72() 73
    #define INSANE_NEXT_73() 74
    #define INSANE_NEXT_74() 75
    #define INSANE_NEXT_75() 76
    #define INSANE_NEXT_76() 77
    #define INSANE_NEXT_77() 78
    #define INSANE_NEXT_78() 79
    #define INSANE_NEXT_79() 80
    #define INSANE_NEXT_80() 81
    #define INSANE_NEXT_81() 82
    #define INSANE_NEXT_82() 83
    #define INSANE_NEXT_83() 84
    #define INSANE_NEXT_84() 85
    #define INSANE_NEXT_85() 86
    #define INSANE_NEXT_86() 87
    #define INSANE_NEXT_87() 88
    #define INSANE_NEXT_88() 89
    #define INSANE_NEXT_89() 90
    #define INSANE_NEXT_90() 91
    #define INSANE_NEXT_91() 92
    #define INSANE_NEXT_92() 93
    #define INSANE_NEXT_93() 94
    #define INSANE_NEXT_94() 95
    #define INSANE_NEXT_95() 96
    #define INSANE_NEXT_96() 97
    #define INSANE_NEXT_97() 98
    #define INSANE_NEXT_98() 99
    #define INSANE_NEXT_99() 100
    #define INSANE_NEXT_100() 101
    #define INSANE_NEXT_101() 102
    #define INSANE_NEXT_102() 103
    #define INSANE_NEXT_103() 104
    #define INSANE_NEXT_104() 105
    #define INSANE_NEXT_105() 106
    #define INSANE_NEXT_106() 107
    #define INSANE_NEXT_107() 108
    #define INSANE_NEXT_108() 109
    #define INSANE_NEXT_109() 110
    #define INSANE_NEXT_110() 111
    #define INSANE_NEXT_111() 112
    #define INSANE_NEXT_112() 113
    #define INSANE_NEXT_113() 114
    #define INSANE_NEXT_114() 115
    #define INSANE_NEXT_115() 116
    #define INSANE_NEXT_116() 117
    #define INSANE_NEXT_117() 118
    #define INSANE_NEXT_118() 119
    #define INSANE_NEXT_119() 120
    #define INSANE_NEXT_120() 121
    #define INSANE_NEXT_121() 122
    #define INSANE_NEXT_122() 123
    #define INSANE_NEXT_123() 124
    #define INSANE_NEXT_124() 125
    #define INSANE_NEXT_125() 126
    #define INSANE_NEXT_126() 127
    #define INSANE_NEXT_127() 128
    #define INSANE_NEXT_128() 129
    #define INSANE_NEXT_129() 130
    #define INSANE_NEXT_130() 131
    #define INSANE_NEXT_131() 132
    #define INSANE_NEXT_132() 133
    #define INSANE_NEXT_133() 134
    #define INSANE_NEXT_134() 135
    #define INSANE_NEXT_135() 136
    #define INSANE_NEXT_136() 137
    #define INSANE_NEXT_137() 138
    #define INSANE_NEXT_138() 139
    #define INSANE_NEXT_139() 140
    #define INSANE_NEXT_140() 141
    #define INSANE_NEXT_141() 142
    #define INSANE_NEXT_142() 143
    #define INSANE_NEXT_143() 144
    #define INSANE_NEXT_144() 145
    #define INSANE_NEXT_145() 146
    #define INSANE_NEXT_146() 147
    #define INSANE_NEXT_147() 148
    #define INSANE_NEXT_148() 149
    #define INSANE_NEXT_149() 150
    #define INSANE_NEXT_150() 151
    #define INSANE_NEXT_151() 152
    #define INSANE_NEXT_152() 153
    #define INSANE_NEXT_153() 154
    #define INSANE_NEXT_154() 155
    #define INSANE_NEXT_155() 156
    #define INSANE_NEXT_156() 157
    #define INSANE_NEXT_157() 158
    #define INSANE_NEXT_158() 159
    #define INSANE_NEXT_159() 160
    #define INSANE_NEXT_160() 161
    #define INSANE_NEXT_161() 162
    #define INSANE_NEXT_162() 163
    #define INSANE_NEXT_163() 164
    #define INSANE_NEXT_164() 165
    #define INSANE_NEXT_165() 166
    #define INSANE_NEXT_166() 167
    #define INSANE_NEXT_167() 168
    #define INSANE_NEXT_168() 169
    #define INSANE_NEXT_169() 170
    #define INSANE_NEXT_170() 171
    #define INSANE_NEXT_171() 172
    #define INSANE_NEXT_172() 173
    #define INSANE_NEXT_173() 174
    #define INSANE_NEXT_174() 175
    #define INSANE_NEXT_175() 176
    #define INSANE_NEXT_176() 177
    #define INSANE_NEXT_177() 178
    #define INSANE_NEXT_178() 179
    #define INSANE_NEXT_179() 180
    #define INSANE_NEXT_180() 181
    #define INSANE_NEXT_181() 182
    #define INSANE_NEXT_182() 183
    #define INSANE_NEXT_183() 184
    #define INSANE_NEXT_184() 185
    #define INSANE_NEXT_185() 186
    #define INSANE_NEXT_186() 187
    #define INSANE_NEXT_187() 188
    #define INSANE_NEXT_188() 189
    #define INSANE_NEXT_189() 190
    #define INSANE_NEXT_190() 191
    #define INSANE_NEXT_191() 192
    #define INSANE_NEXT_192() 193
    #define INSANE_NEXT_193() 194
    #define INSANE_NEXT_194() 195
    #define INSANE_NEXT_195() 196
    #define INSANE_NEXT_196() 197
    #define INSANE_NEXT_197() 198
    #define INSANE_NEXT_198() 199
    #define INSANE_NEXT_199() 200
    #define INSANE_NEXT_200() 201
    #define INSANE_NEXT_201() 202
    #define INSANE_NEXT_202() 203
    #define INSANE_NEXT_203() 204
    #define INSANE_NEXT_204() 205
    #define INSANE_NEXT_205() 206
    #define INSANE_NEXT_206() 207
    #define INSANE_NEXT_207() 208
    #define INSANE_NEXT_208() 209
    #define INSANE_NEXT_209() 210
    #define INSANE_NEXT_210() 211
    #define INSANE_NEXT_211() 212
    #define INSANE_NEXT_212() 213
    #define INSANE_NEXT_213() 214
    #define INSANE_NEXT_214() 215
    #define INSANE_NEXT_215() 216
    #define INSANE_NEXT_216() 217
    #define INSANE_NEXT_217() 218
    #define INSANE_NEXT_218() 219
    #define INSANE_NEXT_219() 220
    #define INSANE_NEXT_220() 221
    #define INSANE_NEXT_221() 222
    #define INSANE_NEXT_222() 223
    #define INSANE_NEXT_223() 224
    #define INSANE_NEXT_224() 225
    #define INSANE_NEXT_225() 226
    #define INSANE_NEXT_226() 227
    #define INSANE_NEXT_227() 228
    #define INSANE_NEXT_228() 229
    #define INSANE_NEXT_229() 230
    #define INSANE_NEXT_230() 231
    #define INSANE_NEXT_231() 232
    #define INSANE_NEXT_232() 233
    #define INSANE_NEXT_233() 234
    #define INSANE_NEXT_234() 235
    #define INSANE_NEXT_235() 236
    #define INSANE_NEXT_236() 237
    #define INSANE_NEXT_237() 238
    #define INSANE_NEXT_238() 239
    #define INSANE_NEXT_239() 240
    #define INSANE_NEXT_240() 241
    #define INSANE_NEXT_241() 242
    #define INSANE_NEXT_242() 243
    #define INSANE_NEXT_243() 244
    #define INSANE_NEXT_244() 245
    #define INSANE_NEXT_245() 246
    #define INSANE_NEXT_246() 247
    #define INSANE_NEXT_247() 248
    #define INSANE_NEXT_248() 249
    #define INSANE_NEXT_249() 250
    #define INSANE_NEXT_250() 251
    #define INSANE_NEXT_251() 252
    #define INSANE_NEXT_252() 253
    #define INSANE_NEXT_253() 254
    #define INSANE_NEXT_254() 255
    #define INSANE_NEXT_255() 256
    #define INSANE_NEXT_256() 257
    #define INSANE_NEXT_257() 258
    #define INSANE_NEXT_258() 259
    #define INSANE_NEXT_259() 260
    #define INSANE_NEXT_260() 261
    #define INSANE_NEXT_261() 262
    #define INSANE_NEXT_262() 263
    #define INSANE_NEXT_263() 264
    #define INSANE_NEXT_264() 265
    #define INSANE_NEXT_265() 266
    #define INSANE_NEXT_266() 267
    #define INSANE_NEXT_267() 268
    #define INSANE_NEXT_268() 269
    #define INSANE_NEXT_269() 270
    #define INSANE_NEXT_270() 271
    #define INSANE_NEXT_271() 272
    #define INSANE_NEXT_272() 273
    #define INSANE_NEXT_273() 274
    #define INSANE_NEXT_274() 275
    #define INSANE_NEXT_275() 276
    #define INSANE_NEXT_276() 277
    #define INSANE_NEXT_277() 278
    #define INSANE_NEXT_278() 279
    #define INSANE_NEXT_279() 280
    #define INSANE_NEXT_280() 281
    #define INSANE_NEXT_281() 282
    #define INSANE_NEXT_282() 283
    #define INSANE_NEXT_283() 284
    #define INSANE_NEXT_284() 285
    #define INSANE_NEXT_285() 286
    #define INSANE_NEXT_286() 287
    #define INSANE_NEXT_287() 288
    #define INSANE_NEXT_288() 289
    #define INSANE_NEXT_289() 290
    #define INSANE_NEXT_290() 291
    #define INSANE_NEXT_291() 292
    #define INSANE_NEXT_292() 293
    #define INSANE_NEXT_293() 294
    #define INSANE_NEXT_294() 295
    #define INSANE_NEXT_295() 296
    #define INSANE_NEXT_296() 297
    #define INSANE_NEXT_297() 298
    #define INSANE_NEXT_298() 299
    #define INSANE_NEXT_299() 300
    #define INSANE_NEXT_300() 301
    #define INSANE_NEXT_301() 302
    #define INSANE_NEXT_302() 303
    #define INSANE_NEXT_303() 304
    #define INSANE_NEXT_304() 305
    #define INSANE_NEXT_305() 306
    #define INSANE_NEXT_306() 307
    #define INSANE_NEXT_307() 308
    #define INSANE_NEXT_308() 309
    #define INSANE_NEXT_309() 310
    #define INSANE_NEXT_310() 311
    #define INSANE_NEXT_311() 312
    #define INSANE_NEXT_312() 313
    #define INSANE_NEXT_313() 314
    #define INSANE_NEXT_314() 315
    #define INSANE_NEXT_315() 316
    #define INSANE_NEXT_316() 317
    #define INSANE_NEXT_317() 318
    #define INSANE_NEXT_318() 319
    #define INSANE_NEXT_319() 320
    #define INSANE_NEXT_320() 321
    #define INSANE_NEXT_321() 322
    #define INSANE_NEXT_322() 323
    #define INSANE_NEXT_323() 324
    #define INSANE_NEXT_324() 325
    #define INSANE_NEXT_325() 326
    #define INSANE_NEXT_326() 327
    #define INSANE_NEXT_327() 328
    #define INSANE_NEXT_328() 329
    #define INSANE_NEXT_329() 330
    #define INSANE_NEXT_330() 331
    #define INSANE_NEXT_331() 332
    #define INSANE_NEXT_332() 333
    #define INSANE_NEXT_333() 334
    #define INSANE_NEXT_334() 335
    #define INSANE_NEXT_335() 336
    #define INSANE_NEXT_336() 337
    #define INSANE_NEXT_337() 338
    #define INSANE_NEXT_338() 339
    #define INSANE_NEXT_339() 340
    #define INSANE_NEXT_340() 341
    #define INSANE_NEXT_341() 342
    #define INSANE_NEXT_342() 343
    #define INSANE_NEXT_343() 344
    #define INSANE_NEXT_344() 345
    #define INSANE_NEXT_345() 346
    #define INSANE_NEXT_346() 347
    #define INSANE_NEXT_347() 348
    #define INSANE_NEXT_348() 349
    #define INSANE_NEXT_349() 350
    #define INSANE_NEXT_350() 351
    #define INSANE_NEXT_351() 352
    #define INSANE_NEXT_352() 353
    #define INSANE_NEXT_353() 354
    #define INSANE_NEXT_354() 355
    #define INSANE_NEXT_355() 356
    #define INSANE_NEXT_356() 357
    #define INSANE_NEXT_357() 358
    #define INSANE_NEXT_358() 359
    #define INSANE_NEXT_359() 360
    #define INSANE_NEXT_360() 361
    #define INSANE_NEXT_361() 362
    #define INSANE_NEXT_362() 363
    #define INSANE_NEXT_363() 364
    #define INSANE_NEXT_364() 365
    #define INSANE_NEXT_365() 366
    #define INSANE_NEXT_366() 367
    #define INSANE_NEXT_367() 368
    #define INSANE_NEXT_368() 369
    #define INSANE_NEXT_369() 370
    #define INSANE_NEXT_370() 371
    #define INSANE_NEXT_371() 372
    #define INSANE_NEXT_372() 373
    #define INSANE_NEXT_373() 374
    #define INSANE_NEXT_374() 375
    #define INSANE_NEXT_375() 376
    #define INSANE_NEXT_376() 377
    #define INSANE_NEXT_377() 378
    #define INSANE_NEXT_378() 379
    #define INSANE_NEXT_379() 380
    #define INSANE_NEXT_380() 381
    #define INSANE_NEXT_381() 382
    #define INSANE_NEXT_382() 383
    #define INSANE_NEXT_383() 384
    #define INSANE_NEXT_384() 385
    #define INSANE_NEXT_385() 386
    #define INSANE_NEXT_386() 387
    #define INSANE_NEXT_387() 388
    #define INSANE_NEXT_388() 389
    #define INSANE_NEXT_389() 390
    #define INSANE_NEXT_390() 391
    #define INSANE_NEXT_391() 392
    #define INSANE_NEXT_392() 393
    #define INSANE_NEXT_393() 394
    #define INSANE_NEXT_394() 395
    #define INSANE_NEXT_395() 396
    #define INSANE_NEXT_396() 397
    #define INSANE_NEXT_397() 398
    #define INSANE_NEXT_398() 399
    #define INSANE_NEXT_399() 400
    #define INSANE_NEXT_400() 401
    #define INSANE_NEXT_401() 402
    #define INSANE_NEXT_402() 403
    #define INSANE_NEXT_403() 404
    #define INSANE_NEXT_404() 405
    #define INSANE_NEXT_405() 406
    #define INSANE_NEXT_406() 407
    #define INSANE_NEXT_407() 408
    #define INSANE_NEXT_408() 409
    #define INSANE_NEXT_409() 410
    #define INSANE_NEXT_410() 411
    #define INSANE_NEXT_411() 412
    #define INSANE_NEXT_412() 413
    #define INSANE_NEXT_413() 414
    #define INSANE_NEXT_414() 415
    #define INSANE_NEXT_415() 416
    #define INSANE_NEXT_416() 417
    #define INSANE_NEXT_417() 418
    #define INSANE_NEXT_418() 419
    #define INSANE_NEXT_419() 420
    #define INSANE_NEXT_420() 421
    #define INSANE_NEXT_421() 422
    #define INSANE_NEXT_422() 423
    #define INSANE_NEXT_423() 424
    #define INSANE_NEXT_424() 425
    #define INSANE_NEXT_425() 426
    #define INSANE_NEXT_426() 427
    #define INSANE_NEXT_427() 428
    #define INSANE_NEXT_428() 429
    #define INSANE_NEXT_429() 430
    #define INSANE_NEXT_430() 431
    #define INSANE_NEXT_431() 432
    #define INSANE_NEXT_432() 433
    #define INSANE_NEXT_433() 434
    #define INSANE_NEXT_434() 435
    #define INSANE_NEXT_435() 436
    #define INSANE_NEXT_436() 437
    #define INSANE_NEXT_437() 438
    #define INSANE_NEXT_438() 439
    #define INSANE_NEXT_439() 440
    #define INSANE_NEXT_440() 441
    #define INSANE_NEXT_441() 442
    #define INSANE_NEXT_442() 443
    #define INSANE_NEXT_443() 444
    #define INSANE_NEXT_444() 445
    #define INSANE_NEXT_445() 446
    #define INSANE_NEXT_446() 447
    #define INSANE_NEXT_447() 448
    #define INSANE_NEXT_448() 449
    #define INSANE_NEXT_449() 450
    #define INSANE_NEXT_450() 451
    #define INSANE_NEXT_451() 452
    #define INSANE_NEXT_452() 453
    #define INSANE_NEXT_453() 454
    #define INSANE_NEXT_454() 455
    #define INSANE_NEXT_455() 456
    #define INSANE_NEXT_456() 457
    #define INSANE_NEXT_457() 458
    #define INSANE_NEXT_458() 459
    #define INSANE_NEXT_459() 460
    #define INSANE_NEXT_460() 461
    #define INSANE_NEXT_461() 462
    #define INSANE_NEXT_462() 463
    #define INSANE_NEXT_463() 464
    #define INSANE_NEXT_464() 465
    #define INSANE_NEXT_465() 466
    #define INSANE_NEXT_466() 467
    #define INSANE_NEXT_467() 468
    #define INSANE_NEXT_468() 469
    #define INSANE_NEXT_469() 470
    #define INSANE_NEXT_470() 471
    #define INSANE_NEXT_471() 472
    #define INSANE_NEXT_472() 473
    #define INSANE_NEXT_473() 474
    #define INSANE_NEXT_474() 475
    #define INSANE_NEXT_475() 476
    #define INSANE_NEXT_476() 477
    #define INSANE_NEXT_477() 478
    #define INSANE_NEXT_478() 479
    #define INSANE_NEXT_479() 480
    #define INSANE_NEXT_480() 481
    #define INSANE_NEXT_481() 482
    #define INSANE_NEXT_482() 483
    #define INSANE_NEXT_483() 484
    #define INSANE_NEXT_484() 485
    #define INSANE_NEXT_485() 486
    #define INSANE_NEXT_486() 487
    #define INSANE_NEXT_487() 488
    #define INSANE_NEXT_488() 489
    #define INSANE_NEXT_489() 490
    #define INSANE_NEXT_490() 491
    #define INSANE_NEXT_491() 492
    #define INSANE_NEXT_492() 493
    #define INSANE_NEXT_493() 494
    #define INSANE_NEXT_494() 495
    #define INSANE_NEXT_495() 496
    #define INSANE_NEXT_496() 497
    #define INSANE_NEXT_497() 498
    #define INSANE_NEXT_498() 499
    #define INSANE_NEXT_499() 500
    #define INSANE_NEXT_500() 501
    #define INSANE_NEXT_501() 502
    #define INSANE_NEXT_502() 503
    #define INSANE_NEXT_503() 504
    #define INSANE_NEXT_504() 505
    #define INSANE_NEXT_505() 506
    #define INSANE_NEXT_506() 507
    #define INSANE_NEXT_507() 508
    #define INSANE_NEXT_508() 509
    #define INSANE_NEXT_509() 510
    #define INSANE_NEXT_510() 511
    #define INSANE_NEXT_511() 512

    #define INSANE_PREV_512() 511
    #define INSANE_PREV_511() 510
    #define INSANE_PREV_510() 509
    #define INSANE_PREV_509() 508
    #define INSANE_PREV_508() 507
    #define INSANE_PREV_507() 506
    #define INSANE_PREV_506() 505
    #define INSANE_PREV_505() 504
    #define INSANE_PREV_504() 503
    #define INSANE_PREV_503() 502
    #define INSANE_PREV_502() 501
    #define INSANE_PREV_501() 500
    #define INSANE_PREV_500() 499
    #define INSANE_PREV_499() 498
    #define INSANE_PREV_498() 497
    #define INSANE_PREV_497() 496
    #define INSANE_PREV_496() 495
    #define INSANE_PREV_495() 494
    #define INSANE_PREV_494() 493
    #define INSANE_PREV_493() 492
    #define INSANE_PREV_492() 491
    #define INSANE_PREV_491() 490
    #define INSANE_PREV_490() 489
    #define INSANE_PREV_489() 488
    #define INSANE_PREV_488() 487
    #define INSANE_PREV_487() 486
    #define INSANE_PREV_486() 485
    #define INSANE_PREV_485() 484
    #define INSANE_PREV_484() 483
    #define INSANE_PREV_483() 482
    #define INSANE_PREV_482() 481
    #define INSANE_PREV_481() 480
    #define INSANE_PREV_480() 479
    #define INSANE_PREV_479() 478
    #define INSANE_PREV_478() 477
    #define INSANE_PREV_477() 476
    #define INSANE_PREV_476() 475
    #define INSANE_PREV_475() 474
    #define INSANE_PREV_474() 473
    #define INSANE_PREV_473() 472
    #define INSANE_PREV_472() 471
    #define INSANE_PREV_471() 470
    #define INSANE_PREV_470() 469
    #define INSANE_PREV_469() 468
    #define INSANE_PREV_468() 467
    #define INSANE_PREV_467() 466
    #define INSANE_PREV_466() 465
    #define INSANE_PREV_465() 464
    #define INSANE_PREV_464() 463
    #define INSANE_PREV_463() 462
    #define INSANE_PREV_462() 461
    #define INSANE_PREV_461() 460
    #define INSANE_PREV_460() 459
    #define INSANE_PREV_459() 458
    #define INSANE_PREV_458() 457
    #define INSANE_PREV_457() 456
    #define INSANE_PREV_456() 455
    #define INSANE_PREV_455() 454
    #define INSANE_PREV_454() 453
    #define INSANE_PREV_453() 452
    #define INSANE_PREV_452() 451
    #define INSANE_PREV_451() 450
    #define INSANE_PREV_450() 449
    #define INSANE_PREV_449() 448
    #define INSANE_PREV_448() 447
    #define INSANE_PREV_447() 446
    #define INSANE_PREV_446() 445
    #define INSANE_PREV_445() 444
    #define INSANE_PREV_444() 443
    #define INSANE_PREV_443() 442
    #define INSANE_PREV_442() 441
    #define INSANE_PREV_441() 440
    #define INSANE_PREV_440() 439
    #define INSANE_PREV_439() 438
    #define INSANE_PREV_438() 437
    #define INSANE_PREV_437() 436
    #define INSANE_PREV_436() 435
    #define INSANE_PREV_435() 434
    #define INSANE_PREV_434() 433
    #define INSANE_PREV_433() 432
    #define INSANE_PREV_432() 431
    #define INSANE_PREV_431() 430
    #define INSANE_PREV_430() 429
    #define INSANE_PREV_429() 428
    #define INSANE_PREV_428() 427
    #define INSANE_PREV_427() 426
    #define INSANE_PREV_426() 425
    #define INSANE_PREV_425() 424
    #define INSANE_PREV_424() 423
    #define INSANE_PREV_423() 422
    #define INSANE_PREV_422() 421
    #define INSANE_PREV_421() 420
    #define INSANE_PREV_420() 419
    #define INSANE_PREV_419() 418
    #define INSANE_PREV_418() 417
    #define INSANE_PREV_417() 416
    #define INSANE_PREV_416() 415
    #define INSANE_PREV_415() 414
    #define INSANE_PREV_414() 413
    #define INSANE_PREV_413() 412
    #define INSANE_PREV_412() 411
    #define INSANE_PREV_411() 410
    #define INSANE_PREV_410() 409
    #define INSANE_PREV_409() 408
    #define INSANE_PREV_408() 407
    #define INSANE_PREV_407() 406
    #define INSANE_PREV_406() 405
    #define INSANE_PREV_405() 404
    #define INSANE_PREV_404() 403
    #define INSANE_PREV_403() 402
    #define INSANE_PREV_402() 401
    #define INSANE_PREV_401() 400
    #define INSANE_PREV_400() 399
    #define INSANE_PREV_399() 398
    #define INSANE_PREV_398() 397
    #define INSANE_PREV_397() 396
    #define INSANE_PREV_396() 395
    #define INSANE_PREV_395() 394
    #define INSANE_PREV_394() 393
    #define INSANE_PREV_393() 392
    #define INSANE_PREV_392() 391
    #define INSANE_PREV_391() 390
    #define INSANE_PREV_390() 389
    #define INSANE_PREV_389() 388
    #define INSANE_PREV_388() 387
    #define INSANE_PREV_387() 386
    #define INSANE_PREV_386() 385
    #define INSANE_PREV_385() 384
    #define INSANE_PREV_384() 383
    #define INSANE_PREV_383() 382
    #define INSANE_PREV_382() 381
    #define INSANE_PREV_381() 380
    #define INSANE_PREV_380() 379
    #define INSANE_PREV_379() 378
    #define INSANE_PREV_378() 377
    #define INSANE_PREV_377() 376
    #define INSANE_PREV_376() 375
    #define INSANE_PREV_375() 374
    #define INSANE_PREV_374() 373
    #define INSANE_PREV_373() 372
    #define INSANE_PREV_372() 371
    #define INSANE_PREV_371() 370
    #define INSANE_PREV_370() 369
    #define INSANE_PREV_369() 368
    #define INSANE_PREV_368() 367
    #define INSANE_PREV_367() 366
    #define INSANE_PREV_366() 365
    #define INSANE_PREV_365() 364
    #define INSANE_PREV_364() 363
    #define INSANE_PREV_363() 362
    #define INSANE_PREV_362() 361
    #define INSANE_PREV_361() 360
    #define INSANE_PREV_360() 359
    #define INSANE_PREV_359() 358
    #define INSANE_PREV_358() 357
    #define INSANE_PREV_357() 356
    #define INSANE_PREV_356() 355
    #define INSANE_PREV_355() 354
    #define INSANE_PREV_354() 353
    #define INSANE_PREV_353() 352
    #define INSANE_PREV_352() 351
    #define INSANE_PREV_351() 350
    #define INSANE_PREV_350() 349
    #define INSANE_PREV_349() 348
    #define INSANE_PREV_348() 347
    #define INSANE_PREV_347() 346
    #define INSANE_PREV_346() 345
    #define INSANE_PREV_345() 344
    #define INSANE_PREV_344() 343
    #define INSANE_PREV_343() 342
    #define INSANE_PREV_342() 341
    #define INSANE_PREV_341() 340
    #define INSANE_PREV_340() 339
    #define INSANE_PREV_339() 338
    #define INSANE_PREV_338() 337
    #define INSANE_PREV_337() 336
    #define INSANE_PREV_336() 335
    #define INSANE_PREV_335() 334
    #define INSANE_PREV_334() 333
    #define INSANE_PREV_333() 332
    #define INSANE_PREV_332() 331
    #define INSANE_PREV_331() 330
    #define INSANE_PREV_330() 329
    #define INSANE_PREV_329() 328
    #define INSANE_PREV_328() 327
    #define INSANE_PREV_327() 326
    #define INSANE_PREV_326() 325
    #define INSANE_PREV_325() 324
    #define INSANE_PREV_324() 323
    #define INSANE_PREV_323() 322
    #define INSANE_PREV_322() 321
    #define INSANE_PREV_321() 320
    #define INSANE_PREV_320() 319
    #define INSANE_PREV_319() 318
    #define INSANE_PREV_318() 317
    #define INSANE_PREV_317() 316
    #define INSANE_PREV_316() 315
    #define INSANE_PREV_315() 314
    #define INSANE_PREV_314() 313
    #define INSANE_PREV_313() 312
    #define INSANE_PREV_312() 311
    #define INSANE_PREV_311() 310
    #define INSANE_PREV_310() 309
    #define INSANE_PREV_309() 308
    #define INSANE_PREV_308() 307
    #define INSANE_PREV_307() 306
    #define INSANE_PREV_306() 305
    #define INSANE_PREV_305() 304
    #define INSANE_PREV_304() 303
    #define INSANE_PREV_303() 302
    #define INSANE_PREV_302() 301
    #define INSANE_PREV_301() 300
    #define INSANE_PREV_300() 299
    #define INSANE_PREV_299() 298
    #define INSANE_PREV_298() 297
    #define INSANE_PREV_297() 296
    #define INSANE_PREV_296() 295
    #define INSANE_PREV_295() 294
    #define INSANE_PREV_294() 293
    #define INSANE_PREV_293() 292
    #define INSANE_PREV_292() 291
    #define INSANE_PREV_291() 290
    #define INSANE_PREV_290() 289
    #define INSANE_PREV_289() 288
    #define INSANE_PREV_288() 287
    #define INSANE_PREV_287() 286
    #define INSANE_PREV_286() 285
    #define INSANE_PREV_285() 284
    #define INSANE_PREV_284() 283
    #define INSANE_PREV_283() 282
    #define INSANE_PREV_282() 281
    #define INSANE_PREV_281() 280
    #define INSANE_PREV_280() 279
    #define INSANE_PREV_279() 278
    #define INSANE_PREV_278() 277
    #define INSANE_PREV_277() 276
    #define INSANE_PREV_276() 275
    #define INSANE_PREV_275() 274
    #define INSANE_PREV_274() 273
    #define INSANE_PREV_273() 272
    #define INSANE_PREV_272() 271
    #define INSANE_PREV_271() 270
    #define INSANE_PREV_270() 269
    #define INSANE_PREV_269() 268
    #define INSANE_PREV_268() 267
    #define INSANE_PREV_267() 266
    #define INSANE_PREV_266() 265
    #define INSANE_PREV_265() 264
    #define INSANE_PREV_264() 263
    #define INSANE_PREV_263() 262
    #define INSANE_PREV_262() 261
    #define INSANE_PREV_261() 260
    #define INSANE_PREV_260() 259
    #define INSANE_PREV_259() 258
    #define INSANE_PREV_258() 257
    #define INSANE_PREV_257() 256
    #define INSANE_PREV_256() 255
    #define INSANE_PREV_255() 254
    #define INSANE_PREV_254() 253
    #define INSANE_PREV_253() 252
    #define INSANE_PREV_252() 251
    #define INSANE_PREV_251() 250
    #define INSANE_PREV_250() 249
    #define INSANE_PREV_249() 248
    #define INSANE_PREV_248() 247
    #define INSANE_PREV_247() 246
    #define INSANE_PREV_246() 245
    #define INSANE_PREV_245() 244
    #define INSANE_PREV_244() 243
    #define INSANE_PREV_243() 242
    #define INSANE_PREV_242() 241
    #define INSANE_PREV_241() 240
    #define INSANE_PREV_240() 239
    #define INSANE_PREV_239() 238
    #define INSANE_PREV_238() 237
    #define INSANE_PREV_237() 236
    #define INSANE_PREV_236() 235
    #define INSANE_PREV_235() 234
    #define INSANE_PREV_234() 233
    #define INSANE_PREV_233() 232
    #define INSANE_PREV_232() 231
    #define INSANE_PREV_231() 230
    #define INSANE_PREV_230() 229
    #define INSANE_PREV_229() 228
    #define INSANE_PREV_228() 227
    #define INSANE_PREV_227() 226
    #define INSANE_PREV_226() 225
    #define INSANE_PREV_225() 224
    #define INSANE_PREV_224() 223
    #define INSANE_PREV_223() 222
    #define INSANE_PREV_222() 221
    #define INSANE_PREV_221() 220
    #define INSANE_PREV_220() 219
    #define INSANE_PREV_219() 218
    #define INSANE_PREV_218() 217
    #define INSANE_PREV_217() 216
    #define INSANE_PREV_216() 215
    #define INSANE_PREV_215() 214
    #define INSANE_PREV_214() 213
    #define INSANE_PREV_213() 212
    #define INSANE_PREV_212() 211
    #define INSANE_PREV_211() 210
    #define INSANE_PREV_210() 209
    #define INSANE_PREV_209() 208
    #define INSANE_PREV_208() 207
    #define INSANE_PREV_207() 206
    #define INSANE_PREV_206() 205
    #define INSANE_PREV_205() 204
    #define INSANE_PREV_204() 203
    #define INSANE_PREV_203() 202
    #define INSANE_PREV_202() 201
    #define INSANE_PREV_201() 200
    #define INSANE_PREV_200() 199
    #define INSANE_PREV_199() 198
    #define INSANE_PREV_198() 197
    #define INSANE_PREV_197() 196
    #define INSANE_PREV_196() 195
    #define INSANE_PREV_195() 194
    #define INSANE_PREV_194() 193
    #define INSANE_PREV_193() 192
    #define INSANE_PREV_192() 191
    #define INSANE_PREV_191() 190
    #define INSANE_PREV_190() 189
    #define INSANE_PREV_189() 188
    #define INSANE_PREV_188() 187
    #define INSANE_PREV_187() 186
    #define INSANE_PREV_186() 185
    #define INSANE_PREV_185() 184
    #define INSANE_PREV_184() 183
    #define INSANE_PREV_183() 182
    #define INSANE_PREV_182() 181
    #define INSANE_PREV_181() 180
    #define INSANE_PREV_180() 179
    #define INSANE_PREV_179() 178
    #define INSANE_PREV_178() 177
    #define INSANE_PREV_177() 176
    #define INSANE_PREV_176() 175
    #define INSANE_PREV_175() 174
    #define INSANE_PREV_174() 173
    #define INSANE_PREV_173() 172
    #define INSANE_PREV_172() 171
    #define INSANE_PREV_171() 170
    #define INSANE_PREV_170() 169
    #define INSANE_PREV_169() 168
    #define INSANE_PREV_168() 167
    #define INSANE_PREV_167() 166
    #define INSANE_PREV_166() 165
    #define INSANE_PREV_165() 164
    #define INSANE_PREV_164() 163
    #define INSANE_PREV_163() 162
    #define INSANE_PREV_162() 161
    #define INSANE_PREV_161() 160
    #define INSANE_PREV_160() 159
    #define INSANE_PREV_159() 158
    #define INSANE_PREV_158() 157
    #define INSANE_PREV_157() 156
    #define INSANE_PREV_156() 155
    #define INSANE_PREV_155() 154
    #define INSANE_PREV_154() 153
    #define INSANE_PREV_153() 152
    #define INSANE_PREV_152() 151
    #define INSANE_PREV_151() 150
    #define INSANE_PREV_150() 149
    #define INSANE_PREV_149() 148
    #define INSANE_PREV_148() 147
    #define INSANE_PREV_147() 146
    #define INSANE_PREV_146() 145
    #define INSANE_PREV_145() 144
    #define INSANE_PREV_144() 143
    #define INSANE_PREV_143() 142
    #define INSANE_PREV_142() 141
    #define INSANE_PREV_141() 140
    #define INSANE_PREV_140() 139
    #define INSANE_PREV_139() 138
    #define INSANE_PREV_138() 137
    #define INSANE_PREV_137() 136
    #define INSANE_PREV_136() 135
    #define INSANE_PREV_135() 134
    #define INSANE_PREV_134() 133
    #define INSANE_PREV_133() 132
    #define INSANE_PREV_132() 131
    #define INSANE_PREV_131() 130
    #define INSANE_PREV_130() 129
    #define INSANE_PREV_129() 128
    #define INSANE_PREV_128() 127
    #define INSANE_PREV_127() 126
    #define INSANE_PREV_126() 125
    #define INSANE_PREV_125() 124
    #define INSANE_PREV_124() 123
    #define INSANE_PREV_123() 122
    #define INSANE_PREV_122() 121
    #define INSANE_PREV_121() 120
    #define INSANE_PREV_120() 119
    #define INSANE_PREV_119() 118
    #define INSANE_PREV_118() 117
    #define INSANE_PREV_117() 116
    #define INSANE_PREV_116() 115
    #define INSANE_PREV_115() 114
    #define INSANE_PREV_114() 113
    #define INSANE_PREV_113() 112
    #define INSANE_PREV_112() 111
    #define INSANE_PREV_111() 110
    #define INSANE_PREV_110() 109
    #define INSANE_PREV_109() 108
    #define INSANE_PREV_108() 107
    #define INSANE_PREV_107() 106
    #define INSANE_PREV_106() 105
    #define INSANE_PREV_105() 104
    #define INSANE_PREV_104() 103
    #define INSANE_PREV_103() 102
    #define INSANE_PREV_102() 101
    #define INSANE_PREV_101() 100
    #define INSANE_PREV_100() 99
    #define INSANE_PREV_99() 98
    #define INSANE_PREV_98() 97
    #define INSANE_PREV_97() 96
    #define INSANE_PREV_96() 95
    #define INSANE_PREV_95() 94
    #define INSANE_PREV_94() 93
    #define INSANE_PREV_93() 92
    #define INSANE_PREV_92() 91
    #define INSANE_PREV_91() 90
    #define INSANE_PREV_90() 89
    #define INSANE_PREV_89() 88
    #define INSANE_PREV_88() 87
    #define INSANE_PREV_87() 86
    #define INSANE_PREV_86() 85
    #define INSANE_PREV_85() 84
    #define INSANE_PREV_84() 83
    #define INSANE_PREV_83() 82
    #define INSANE_PREV_82() 81
    #define INSANE_PREV_81() 80
    #define INSANE_PREV_80() 79
    #define INSANE_PREV_79() 78
    #define INSANE_PREV_78() 77
    #define INSANE_PREV_77() 76
    #define INSANE_PREV_76() 75
    #define INSANE_PREV_75() 74
    #define INSANE_PREV_74() 73
    #define INSANE_PREV_73() 72
    #define INSANE_PREV_72() 71
    #define INSANE_PREV_71() 70
    #define INSANE_PREV_70() 69
    #define INSANE_PREV_69() 68
    #define INSANE_PREV_68() 67
    #define INSANE_PREV_67() 66
    #define INSANE_PREV_66() 65
    #define INSANE_PREV_65() 64
    #define INSANE_PREV_64() 63
    #define INSANE_PREV_63() 62
    #define INSANE_PREV_62() 61
    #define INSANE_PREV_61() 60
    #define INSANE_PREV_60() 59
    #define INSANE_PREV_59() 58
    #define INSANE_PREV_58() 57
    #define INSANE_PREV_57() 56
    #define INSANE_PREV_56() 55
    #define INSANE_PREV_55() 54
    #define INSANE_PREV_54() 53
    #define INSANE_PREV_53() 52
    #define INSANE_PREV_52() 51
    #define INSANE_PREV_51() 50
    #define INSANE_PREV_50() 49
    #define INSANE_PREV_49() 48
    #define INSANE_PREV_48() 47
    #define INSANE_PREV_47() 46
    #define INSANE_PREV_46() 45
    #define INSANE_PREV_45() 44
    #define INSANE_PREV_44() 43
    #define INSANE_PREV_43() 42
    #define INSANE_PREV_42() 41
    #define INSANE_PREV_41() 40
    #define INSANE_PREV_40() 39
    #define INSANE_PREV_39() 38
    #define INSANE_PREV_38() 37
    #define INSANE_PREV_37() 36
    #define INSANE_PREV_36() 35
    #define INSANE_PREV_35() 34
    #define INSANE_PREV_34() 33
    #define INSANE_PREV_33() 32
    #define INSANE_PREV_32() 31
    #define INSANE_PREV_31() 30
    #define INSANE_PREV_30() 29
    #define INSANE_PREV_29() 28
    #define INSANE_PREV_28() 27
    #define INSANE_PREV_27() 26
    #define INSANE_PREV_26() 25
    #define INSANE_PREV_25() 24
    #define INSANE_PREV_24() 23
    #define INSANE_PREV_23() 22
    #define INSANE_PREV_22() 21
    #define INSANE_PREV_21() 20
    #define INSANE_PREV_20() 19
    #define INSANE_PREV_19() 18
    #define INSANE_PREV_18() 17
    #define INSANE_PREV_17() 16
    #define INSANE_PREV_16() 15
    #define INSANE_PREV_15() 14
    #define INSANE_PREV_14() 13
    #define INSANE_PREV_13() 12
    #define INSANE_PREV_12() 11
    #define INSANE_PREV_11() 10
    #define INSANE_PREV_10() 9
    #define INSANE_PREV_9() 8
    #define INSANE_PREV_8() 7
    #define INSANE_PREV_7() 6
    #define INSANE_PREV_6() 5
    #define INSANE_PREV_5() 4
    #define INSANE_PREV_4() 3
    #define INSANE_PREV_3() 2
    #define INSANE_PREV_2() 1
    #define INSANE_PREV_1() 0

    #define __INSANE_NEXT_EXPAND(exp) exp()
    #define __INSANE_PREV_EXPAND(exp) exp()
    #define INSANE_NEXT(value) __INSANE_NEXT_EXPAND(INSANE_NEXT_##value)
    #define INSANE_PREV(value) __INSANE_PREV_EXPAND(INSANE_PREV_##value)

    #define __INSANE_REPEAT_SEQ_0(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) ADD_EXPRESSION_##sae(se)
    #define __INSANE_REPEAT_SEQ_1(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) __INSANE_REPEAT_SEQ_0(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_2(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_1(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_3(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_2(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_4(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_3(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_5(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_4(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_6(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_5(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_7(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_6(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_8(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_7(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_9(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_8(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_10(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_9(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_11(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_10(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_12(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_11(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_13(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_12(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_14(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_13(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_15(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_14(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_16(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_15(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_17(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_16(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_18(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_17(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_19(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_18(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_20(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_19(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_21(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_20(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_22(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_21(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_23(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_22(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_24(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_23(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_25(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_24(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_26(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_25(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_27(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_26(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_28(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_27(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_29(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_28(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_30(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_29(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_31(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_30(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_32(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_31(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_33(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_32(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_34(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_33(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_35(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_34(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_36(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_35(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_37(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_36(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_38(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_37(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_39(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_38(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_40(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_39(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_41(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_40(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_42(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_41(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_43(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_42(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_44(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_43(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_45(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_44(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_46(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_45(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_47(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_46(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_48(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_47(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_49(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_48(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_50(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_49(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_51(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_50(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_52(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_51(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_53(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_52(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_54(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_53(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_55(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_54(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_56(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_55(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_57(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_56(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_58(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_57(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_59(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_58(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_60(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_59(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_61(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_60(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_62(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_61(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_63(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_62(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_64(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_63(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_65(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_64(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_66(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_65(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_67(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_66(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_68(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_67(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_69(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_68(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_70(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_69(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_71(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_70(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_72(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_71(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_73(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_72(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_74(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_73(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_75(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_74(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_76(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_75(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_77(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_76(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_78(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_77(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_79(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_78(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_80(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_79(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_81(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_80(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_82(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_81(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_83(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_82(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_84(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_83(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_85(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_84(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_86(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_85(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_87(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_86(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_88(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_87(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_89(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_88(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_90(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_89(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_91(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_90(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_92(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_91(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_93(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_92(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_94(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_93(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_95(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_94(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_96(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_95(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_97(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_96(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_98(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_97(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_99(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_98(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_100(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_99(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_101(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_100(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_102(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_101(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_103(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_102(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_104(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_103(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_105(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_104(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_106(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_105(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_107(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_106(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_108(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_107(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_109(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_108(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_110(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_109(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_111(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_110(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_112(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_111(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_113(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_112(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_114(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_113(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_115(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_114(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_116(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_115(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_117(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_116(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_118(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_117(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_119(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_118(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_120(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_119(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_121(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_120(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_122(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_121(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_123(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_122(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_124(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_123(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_125(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_124(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_126(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_125(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_127(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_126(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_128(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_127(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_129(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_128(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_130(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_129(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_131(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_130(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_132(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_131(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_133(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_132(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_134(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_133(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_135(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_134(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_136(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_135(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_137(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_136(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_138(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_137(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_139(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_138(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_140(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_139(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_141(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_140(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_142(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_141(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_143(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_142(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_144(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_143(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_145(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_144(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_146(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_145(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_147(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_146(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_148(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_147(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_149(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_148(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_150(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_149(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_151(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_150(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_152(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_151(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_153(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_152(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_154(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_153(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_155(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_154(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_156(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_155(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_157(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_156(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_158(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_157(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_159(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_158(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_160(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_159(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_161(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_160(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_162(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_161(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_163(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_162(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_164(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_163(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_165(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_164(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_166(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_165(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_167(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_166(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_168(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_167(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_169(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_168(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_170(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_169(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_171(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_170(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_172(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_171(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_173(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_172(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_174(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_173(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_175(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_174(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_176(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_175(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_177(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_176(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_178(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_177(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_179(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_178(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_180(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_179(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_181(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_180(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_182(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_181(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_183(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_182(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_184(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_183(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_185(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_184(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_186(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_185(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_187(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_186(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_188(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_187(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_189(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_188(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_190(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_189(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_191(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_190(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_192(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_191(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_193(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_192(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_194(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_193(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_195(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_194(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_196(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_195(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_197(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_196(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_198(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_197(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_199(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_198(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_200(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_199(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_201(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_200(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_202(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_201(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_203(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_202(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_204(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_203(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_205(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_204(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_206(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_205(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_207(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_206(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_208(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_207(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_209(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_208(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_210(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_209(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_211(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_210(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_212(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_211(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_213(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_212(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_214(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_213(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_215(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_214(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_216(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_215(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_217(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_216(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_218(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_217(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_219(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_218(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_220(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_219(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_221(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_220(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_222(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_221(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_223(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_222(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_224(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_223(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_225(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_224(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_226(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_225(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_227(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_226(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_228(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_227(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_229(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_228(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_230(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_229(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_231(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_230(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_232(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_231(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_233(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_232(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_234(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_233(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_235(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_234(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_236(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_235(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_237(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_236(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_238(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_237(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_239(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_238(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_240(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_239(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_241(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_240(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_242(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_241(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_243(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_242(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_244(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_243(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_245(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_244(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_246(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_245(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_247(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_246(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_248(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_247(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_249(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_248(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_250(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_249(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_251(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_250(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_252(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_251(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_253(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_252(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_254(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_253(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_255(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_254(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_256(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_255(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_257(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_256(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_258(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_257(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_259(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_258(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_260(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_259(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_261(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_260(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_262(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_261(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_263(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_262(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_264(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_263(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_265(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_264(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_266(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_265(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_267(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_266(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_268(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_267(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_269(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_268(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_270(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_269(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_271(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_270(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_272(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_271(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_273(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_272(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_274(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_273(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_275(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_274(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_276(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_275(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_277(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_276(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_278(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_277(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_279(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_278(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_280(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_279(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_281(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_280(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_282(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_281(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_283(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_282(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_284(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_283(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_285(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_284(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_286(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_285(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_287(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_286(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_288(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_287(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_289(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_288(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_290(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_289(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_291(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_290(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_292(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_291(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_293(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_292(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_294(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_293(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_295(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_294(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_296(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_295(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_297(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_296(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_298(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_297(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_299(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_298(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_300(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_299(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_301(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_300(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_302(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_301(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_303(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_302(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_304(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_303(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_305(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_304(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_306(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_305(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_307(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_306(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_308(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_307(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_309(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_308(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_310(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_309(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_311(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_310(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_312(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_311(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_313(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_312(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_314(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_313(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_315(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_314(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_316(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_315(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_317(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_316(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_318(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_317(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_319(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_318(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_320(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_319(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_321(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_320(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_322(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_321(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_323(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_322(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_324(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_323(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_325(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_324(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_326(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_325(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_327(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_326(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_328(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_327(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_329(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_328(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_330(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_329(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_331(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_330(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_332(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_331(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_333(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_332(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_334(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_333(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_335(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_334(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_336(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_335(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_337(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_336(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_338(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_337(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_339(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_338(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_340(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_339(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_341(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_340(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_342(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_341(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_343(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_342(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_344(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_343(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_345(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_344(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_346(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_345(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_347(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_346(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_348(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_347(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_349(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_348(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_350(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_349(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_351(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_350(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_352(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_351(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_353(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_352(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_354(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_353(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_355(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_354(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_356(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_355(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_357(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_356(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_358(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_357(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_359(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_358(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_360(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_359(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_361(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_360(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_362(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_361(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_363(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_362(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_364(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_363(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_365(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_364(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_366(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_365(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_367(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_366(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_368(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_367(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_369(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_368(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_370(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_369(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_371(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_370(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_372(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_371(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_373(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_372(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_374(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_373(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_375(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_374(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_376(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_375(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_377(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_376(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_378(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_377(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_379(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_378(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_380(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_379(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_381(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_380(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_382(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_381(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_383(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_382(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_384(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_383(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_385(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_384(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_386(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_385(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_387(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_386(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_388(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_387(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_389(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_388(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_390(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_389(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_391(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_390(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_392(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_391(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_393(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_392(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_394(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_393(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_395(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_394(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_396(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_395(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_397(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_396(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_398(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_397(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_399(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_398(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_400(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_399(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_401(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_400(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_402(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_401(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_403(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_402(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_404(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_403(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_405(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_404(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_406(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_405(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_407(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_406(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_408(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_407(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_409(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_408(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_410(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_409(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_411(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_410(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_412(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_411(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_413(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_412(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_414(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_413(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_415(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_414(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_416(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_415(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_417(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_416(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_418(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_417(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_419(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_418(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_420(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_419(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_421(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_420(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_422(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_421(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_423(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_422(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_424(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_423(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_425(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_424(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_426(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_425(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_427(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_426(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_428(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_427(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_429(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_428(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_430(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_429(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_431(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_430(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_432(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_431(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_433(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_432(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_434(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_433(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_435(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_434(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_436(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_435(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_437(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_436(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_438(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_437(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_439(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_438(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_440(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_439(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_441(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_440(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_442(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_441(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_443(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_442(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_444(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_443(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_445(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_444(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_446(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_445(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_447(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_446(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_448(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_447(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_449(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_448(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_450(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_449(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_451(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_450(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_452(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_451(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_453(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_452(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_454(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_453(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_455(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_454(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_456(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_455(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_457(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_456(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_458(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_457(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_459(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_458(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_460(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_459(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_461(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_460(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_462(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_461(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_463(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_462(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_464(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_463(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_465(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_464(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_466(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_465(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_467(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_466(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_468(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_467(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_469(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_468(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_470(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_469(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_471(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_470(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_472(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_471(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_473(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_472(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_474(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_473(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_475(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_474(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_476(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_475(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_477(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_476(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_478(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_477(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_479(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_478(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_480(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_479(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_481(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_480(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_482(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_481(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_483(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_482(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_484(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_483(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_485(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_484(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_486(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_485(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_487(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_486(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_488(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_487(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_489(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_488(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_490(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_489(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_491(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_490(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_492(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_491(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_493(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_492(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_494(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_493(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_495(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_494(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_496(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_495(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_497(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_496(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_498(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_497(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_499(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_498(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_500(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_499(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_501(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_500(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_502(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_501(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_503(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_502(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_504(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_503(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_505(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_504(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_506(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_505(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_507(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_506(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_508(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_507(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_509(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_508(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_510(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_509(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_511(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_510(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define __INSANE_REPEAT_SEQ_512(exp, n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) exp(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) se() __INSANE_REPEAT_SEQ_511(exp, nprev, INSANE_PREV(nprev), cnext, INSANE_NEXT(cnext), se, sae, p1, p2, p3, p4, p5)
    #define INSANE_REPEAT_SEQ(exp, n, se, sae, p1, p2, p3, p4, p5) __INSANE_REPEAT_SEQ_##n(exp, n, INSANE_PREV(n), 1, INSANE_NEXT(1), se, sae, p1, p2, p3, p4, p5)
/*
█ INSANE_REPEAT_SEQ - Help

"exp" is a MACRO identifier that will be invoked and receives the following parameters(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5).This macro must meet the following specifications for proper operation :

"n" : is the current number of the iteration, and it is the number in countdown.It goes from n down to 0.
"nprev" : is n - 1, the previous value of n.
"c" : is the inverse of n, the progressive count from 1 up to n.
"cnext" : is c + 1, the next value of c.
"se" : is a macro identifier that does not receive any parameters.When invoked, it only returns a separator such as a comma(, ), period(.), equal sign(= ), etc.
"sae" : indicates whether to add the separator at the end.Its value must be 0 or 1. 0 means no separator at the end, and 1 means add the separator at the end.
p1, p2, p3, p4, p5 : are additional parameters passed to "exp".

For example :

1. If you want to generate a sequence of numbers from 1 to 10, you should enter the following :
#define COMMA_VALUE() ,
#define MYEXP(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) c
INSANE_REPEAT_SEQ(MYEXP, 10, COMMA_VALUE, 0, 0, 0, 0, 0, 0)

The result upon expansion is :
1, 2, 3, 4, 5, 6, 7, 8, 9, 10.

2. If you want to generate a sequence of numbers from 10 to 1, you should enter the following :
#define COMMA_VALUE() ,
#define MYEXP(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) n
INSANE_REPEAT_SEQ(MYEXP, 10, COMMA_VALUE, 0, 0, 0, 0, 0, 0)

The result upon expansion is :
10, 9, 8, 7, 6, 5, 4, 3, 2, 1.

3. If you want to generate a list of 10 integer parameters where the variable name starts with "param" and has the suffix of the number in incremental form (a1, a2, ..., a10), you should enter the following:

#define MyExpresion(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5) int p1##c
INSANE_REPEAT_SEQ(MyExpresion,10,COMMA_VALUE,0,param,_,_,_,_)

The result upon expansion is:
int param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9, int param10

4. Generate the declaration of type alias of the std::function type that receives N values of any type as arguments, a value of int type, a value of size_t type, and returns an std::vector of another type, you should enter the following:
#define MyExpresion(n, nprev, c, cnext, se, sae, p1, p2, p3, p4, p5)  p1
#define FUNCTION_TYPE_DECLARE(name,paramType,returnType,n) using name = std::function<std::vector<returnType>>(INSANE_REPEAT_SEQ(MyExpresion,n,COMMA_VALUE,1,paramType,_,_,_,_)int,size_t);
FUNCTION_TYPE_DECLARE(CharFunction,char, std::string,5)

The result upon expansion is :
using CharFunction = std::function<std::vector<std::string>>(char, char, char, char, char, int, size_t);
*/

    #define __INSANE_REPEAT_0()
    #define __INSANE_REPEAT_1(value, se, sae) EXPAND_VALUE(value) ADD_EXPRESSION_##sae(se) __INSANE_REPEAT_0()
    #define __INSANE_REPEAT_2(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_1(value, se, sae)
    #define __INSANE_REPEAT_3(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_2(value, se, sae)
    #define __INSANE_REPEAT_4(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_3(value, se, sae)
    #define __INSANE_REPEAT_5(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_4(value, se, sae)
    #define __INSANE_REPEAT_6(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_5(value, se, sae)
    #define __INSANE_REPEAT_7(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_6(value, se, sae)
    #define __INSANE_REPEAT_8(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_7(value, se, sae)
    #define __INSANE_REPEAT_9(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_8(value, se, sae)
    #define __INSANE_REPEAT_10(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_9(value, se, sae)
    #define __INSANE_REPEAT_11(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_10(value, se, sae)
    #define __INSANE_REPEAT_12(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_11(value, se, sae)
    #define __INSANE_REPEAT_13(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_12(value, se, sae)
    #define __INSANE_REPEAT_14(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_13(value, se, sae)
    #define __INSANE_REPEAT_15(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_14(value, se, sae)
    #define __INSANE_REPEAT_16(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_15(value, se, sae)
    #define __INSANE_REPEAT_17(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_16(value, se, sae)
    #define __INSANE_REPEAT_18(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_17(value, se, sae)
    #define __INSANE_REPEAT_19(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_18(value, se, sae)
    #define __INSANE_REPEAT_20(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_19(value, se, sae)
    #define __INSANE_REPEAT_21(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_20(value, se, sae)
    #define __INSANE_REPEAT_22(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_21(value, se, sae)
    #define __INSANE_REPEAT_23(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_22(value, se, sae)
    #define __INSANE_REPEAT_24(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_23(value, se, sae)
    #define __INSANE_REPEAT_25(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_24(value, se, sae)
    #define __INSANE_REPEAT_26(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_25(value, se, sae)
    #define __INSANE_REPEAT_27(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_26(value, se, sae)
    #define __INSANE_REPEAT_28(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_27(value, se, sae)
    #define __INSANE_REPEAT_29(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_28(value, se, sae)
    #define __INSANE_REPEAT_30(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_29(value, se, sae)
    #define __INSANE_REPEAT_31(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_30(value, se, sae)
    #define __INSANE_REPEAT_32(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_31(value, se, sae)
    #define __INSANE_REPEAT_33(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_32(value, se, sae)
    #define __INSANE_REPEAT_34(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_33(value, se, sae)
    #define __INSANE_REPEAT_35(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_34(value, se, sae)
    #define __INSANE_REPEAT_36(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_35(value, se, sae)
    #define __INSANE_REPEAT_37(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_36(value, se, sae)
    #define __INSANE_REPEAT_38(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_37(value, se, sae)
    #define __INSANE_REPEAT_39(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_38(value, se, sae)
    #define __INSANE_REPEAT_40(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_39(value, se, sae)
    #define __INSANE_REPEAT_41(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_40(value, se, sae)
    #define __INSANE_REPEAT_42(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_41(value, se, sae)
    #define __INSANE_REPEAT_43(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_42(value, se, sae)
    #define __INSANE_REPEAT_44(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_43(value, se, sae)
    #define __INSANE_REPEAT_45(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_44(value, se, sae)
    #define __INSANE_REPEAT_46(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_45(value, se, sae)
    #define __INSANE_REPEAT_47(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_46(value, se, sae)
    #define __INSANE_REPEAT_48(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_47(value, se, sae)
    #define __INSANE_REPEAT_49(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_48(value, se, sae)
    #define __INSANE_REPEAT_50(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_49(value, se, sae)
    #define __INSANE_REPEAT_51(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_50(value, se, sae)
    #define __INSANE_REPEAT_52(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_51(value, se, sae)
    #define __INSANE_REPEAT_53(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_52(value, se, sae)
    #define __INSANE_REPEAT_54(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_53(value, se, sae)
    #define __INSANE_REPEAT_55(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_54(value, se, sae)
    #define __INSANE_REPEAT_56(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_55(value, se, sae)
    #define __INSANE_REPEAT_57(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_56(value, se, sae)
    #define __INSANE_REPEAT_58(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_57(value, se, sae)
    #define __INSANE_REPEAT_59(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_58(value, se, sae)
    #define __INSANE_REPEAT_60(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_59(value, se, sae)
    #define __INSANE_REPEAT_61(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_60(value, se, sae)
    #define __INSANE_REPEAT_62(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_61(value, se, sae)
    #define __INSANE_REPEAT_63(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_62(value, se, sae)
    #define __INSANE_REPEAT_64(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_63(value, se, sae)
    #define __INSANE_REPEAT_65(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_64(value, se, sae)
    #define __INSANE_REPEAT_66(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_65(value, se, sae)
    #define __INSANE_REPEAT_67(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_66(value, se, sae)
    #define __INSANE_REPEAT_68(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_67(value, se, sae)
    #define __INSANE_REPEAT_69(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_68(value, se, sae)
    #define __INSANE_REPEAT_70(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_69(value, se, sae)
    #define __INSANE_REPEAT_71(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_70(value, se, sae)
    #define __INSANE_REPEAT_72(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_71(value, se, sae)
    #define __INSANE_REPEAT_73(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_72(value, se, sae)
    #define __INSANE_REPEAT_74(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_73(value, se, sae)
    #define __INSANE_REPEAT_75(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_74(value, se, sae)
    #define __INSANE_REPEAT_76(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_75(value, se, sae)
    #define __INSANE_REPEAT_77(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_76(value, se, sae)
    #define __INSANE_REPEAT_78(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_77(value, se, sae)
    #define __INSANE_REPEAT_79(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_78(value, se, sae)
    #define __INSANE_REPEAT_80(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_79(value, se, sae)
    #define __INSANE_REPEAT_81(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_80(value, se, sae)
    #define __INSANE_REPEAT_82(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_81(value, se, sae)
    #define __INSANE_REPEAT_83(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_82(value, se, sae)
    #define __INSANE_REPEAT_84(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_83(value, se, sae)
    #define __INSANE_REPEAT_85(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_84(value, se, sae)
    #define __INSANE_REPEAT_86(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_85(value, se, sae)
    #define __INSANE_REPEAT_87(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_86(value, se, sae)
    #define __INSANE_REPEAT_88(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_87(value, se, sae)
    #define __INSANE_REPEAT_89(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_88(value, se, sae)
    #define __INSANE_REPEAT_90(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_89(value, se, sae)
    #define __INSANE_REPEAT_91(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_90(value, se, sae)
    #define __INSANE_REPEAT_92(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_91(value, se, sae)
    #define __INSANE_REPEAT_93(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_92(value, se, sae)
    #define __INSANE_REPEAT_94(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_93(value, se, sae)
    #define __INSANE_REPEAT_95(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_94(value, se, sae)
    #define __INSANE_REPEAT_96(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_95(value, se, sae)
    #define __INSANE_REPEAT_97(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_96(value, se, sae)
    #define __INSANE_REPEAT_98(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_97(value, se, sae)
    #define __INSANE_REPEAT_99(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_98(value, se, sae)
    #define __INSANE_REPEAT_100(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_99(value, se, sae)
    #define __INSANE_REPEAT_101(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_100(value, se, sae)
    #define __INSANE_REPEAT_102(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_101(value, se, sae)
    #define __INSANE_REPEAT_103(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_102(value, se, sae)
    #define __INSANE_REPEAT_104(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_103(value, se, sae)
    #define __INSANE_REPEAT_105(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_104(value, se, sae)
    #define __INSANE_REPEAT_106(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_105(value, se, sae)
    #define __INSANE_REPEAT_107(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_106(value, se, sae)
    #define __INSANE_REPEAT_108(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_107(value, se, sae)
    #define __INSANE_REPEAT_109(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_108(value, se, sae)
    #define __INSANE_REPEAT_110(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_109(value, se, sae)
    #define __INSANE_REPEAT_111(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_110(value, se, sae)
    #define __INSANE_REPEAT_112(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_111(value, se, sae)
    #define __INSANE_REPEAT_113(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_112(value, se, sae)
    #define __INSANE_REPEAT_114(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_113(value, se, sae)
    #define __INSANE_REPEAT_115(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_114(value, se, sae)
    #define __INSANE_REPEAT_116(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_115(value, se, sae)
    #define __INSANE_REPEAT_117(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_116(value, se, sae)
    #define __INSANE_REPEAT_118(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_117(value, se, sae)
    #define __INSANE_REPEAT_119(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_118(value, se, sae)
    #define __INSANE_REPEAT_120(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_119(value, se, sae)
    #define __INSANE_REPEAT_121(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_120(value, se, sae)
    #define __INSANE_REPEAT_122(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_121(value, se, sae)
    #define __INSANE_REPEAT_123(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_122(value, se, sae)
    #define __INSANE_REPEAT_124(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_123(value, se, sae)
    #define __INSANE_REPEAT_125(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_124(value, se, sae)
    #define __INSANE_REPEAT_126(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_125(value, se, sae)
    #define __INSANE_REPEAT_127(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_126(value, se, sae)
    #define __INSANE_REPEAT_128(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_127(value, se, sae)
    #define __INSANE_REPEAT_129(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_128(value, se, sae)
    #define __INSANE_REPEAT_130(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_129(value, se, sae)
    #define __INSANE_REPEAT_131(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_130(value, se, sae)
    #define __INSANE_REPEAT_132(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_131(value, se, sae)
    #define __INSANE_REPEAT_133(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_132(value, se, sae)
    #define __INSANE_REPEAT_134(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_133(value, se, sae)
    #define __INSANE_REPEAT_135(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_134(value, se, sae)
    #define __INSANE_REPEAT_136(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_135(value, se, sae)
    #define __INSANE_REPEAT_137(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_136(value, se, sae)
    #define __INSANE_REPEAT_138(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_137(value, se, sae)
    #define __INSANE_REPEAT_139(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_138(value, se, sae)
    #define __INSANE_REPEAT_140(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_139(value, se, sae)
    #define __INSANE_REPEAT_141(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_140(value, se, sae)
    #define __INSANE_REPEAT_142(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_141(value, se, sae)
    #define __INSANE_REPEAT_143(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_142(value, se, sae)
    #define __INSANE_REPEAT_144(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_143(value, se, sae)
    #define __INSANE_REPEAT_145(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_144(value, se, sae)
    #define __INSANE_REPEAT_146(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_145(value, se, sae)
    #define __INSANE_REPEAT_147(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_146(value, se, sae)
    #define __INSANE_REPEAT_148(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_147(value, se, sae)
    #define __INSANE_REPEAT_149(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_148(value, se, sae)
    #define __INSANE_REPEAT_150(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_149(value, se, sae)
    #define __INSANE_REPEAT_151(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_150(value, se, sae)
    #define __INSANE_REPEAT_152(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_151(value, se, sae)
    #define __INSANE_REPEAT_153(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_152(value, se, sae)
    #define __INSANE_REPEAT_154(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_153(value, se, sae)
    #define __INSANE_REPEAT_155(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_154(value, se, sae)
    #define __INSANE_REPEAT_156(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_155(value, se, sae)
    #define __INSANE_REPEAT_157(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_156(value, se, sae)
    #define __INSANE_REPEAT_158(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_157(value, se, sae)
    #define __INSANE_REPEAT_159(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_158(value, se, sae)
    #define __INSANE_REPEAT_160(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_159(value, se, sae)
    #define __INSANE_REPEAT_161(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_160(value, se, sae)
    #define __INSANE_REPEAT_162(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_161(value, se, sae)
    #define __INSANE_REPEAT_163(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_162(value, se, sae)
    #define __INSANE_REPEAT_164(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_163(value, se, sae)
    #define __INSANE_REPEAT_165(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_164(value, se, sae)
    #define __INSANE_REPEAT_166(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_165(value, se, sae)
    #define __INSANE_REPEAT_167(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_166(value, se, sae)
    #define __INSANE_REPEAT_168(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_167(value, se, sae)
    #define __INSANE_REPEAT_169(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_168(value, se, sae)
    #define __INSANE_REPEAT_170(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_169(value, se, sae)
    #define __INSANE_REPEAT_171(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_170(value, se, sae)
    #define __INSANE_REPEAT_172(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_171(value, se, sae)
    #define __INSANE_REPEAT_173(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_172(value, se, sae)
    #define __INSANE_REPEAT_174(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_173(value, se, sae)
    #define __INSANE_REPEAT_175(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_174(value, se, sae)
    #define __INSANE_REPEAT_176(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_175(value, se, sae)
    #define __INSANE_REPEAT_177(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_176(value, se, sae)
    #define __INSANE_REPEAT_178(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_177(value, se, sae)
    #define __INSANE_REPEAT_179(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_178(value, se, sae)
    #define __INSANE_REPEAT_180(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_179(value, se, sae)
    #define __INSANE_REPEAT_181(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_180(value, se, sae)
    #define __INSANE_REPEAT_182(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_181(value, se, sae)
    #define __INSANE_REPEAT_183(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_182(value, se, sae)
    #define __INSANE_REPEAT_184(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_183(value, se, sae)
    #define __INSANE_REPEAT_185(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_184(value, se, sae)
    #define __INSANE_REPEAT_186(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_185(value, se, sae)
    #define __INSANE_REPEAT_187(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_186(value, se, sae)
    #define __INSANE_REPEAT_188(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_187(value, se, sae)
    #define __INSANE_REPEAT_189(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_188(value, se, sae)
    #define __INSANE_REPEAT_190(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_189(value, se, sae)
    #define __INSANE_REPEAT_191(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_190(value, se, sae)
    #define __INSANE_REPEAT_192(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_191(value, se, sae)
    #define __INSANE_REPEAT_193(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_192(value, se, sae)
    #define __INSANE_REPEAT_194(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_193(value, se, sae)
    #define __INSANE_REPEAT_195(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_194(value, se, sae)
    #define __INSANE_REPEAT_196(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_195(value, se, sae)
    #define __INSANE_REPEAT_197(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_196(value, se, sae)
    #define __INSANE_REPEAT_198(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_197(value, se, sae)
    #define __INSANE_REPEAT_199(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_198(value, se, sae)
    #define __INSANE_REPEAT_200(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_199(value, se, sae)
    #define __INSANE_REPEAT_201(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_200(value, se, sae)
    #define __INSANE_REPEAT_202(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_201(value, se, sae)
    #define __INSANE_REPEAT_203(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_202(value, se, sae)
    #define __INSANE_REPEAT_204(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_203(value, se, sae)
    #define __INSANE_REPEAT_205(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_204(value, se, sae)
    #define __INSANE_REPEAT_206(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_205(value, se, sae)
    #define __INSANE_REPEAT_207(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_206(value, se, sae)
    #define __INSANE_REPEAT_208(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_207(value, se, sae)
    #define __INSANE_REPEAT_209(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_208(value, se, sae)
    #define __INSANE_REPEAT_210(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_209(value, se, sae)
    #define __INSANE_REPEAT_211(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_210(value, se, sae)
    #define __INSANE_REPEAT_212(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_211(value, se, sae)
    #define __INSANE_REPEAT_213(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_212(value, se, sae)
    #define __INSANE_REPEAT_214(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_213(value, se, sae)
    #define __INSANE_REPEAT_215(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_214(value, se, sae)
    #define __INSANE_REPEAT_216(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_215(value, se, sae)
    #define __INSANE_REPEAT_217(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_216(value, se, sae)
    #define __INSANE_REPEAT_218(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_217(value, se, sae)
    #define __INSANE_REPEAT_219(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_218(value, se, sae)
    #define __INSANE_REPEAT_220(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_219(value, se, sae)
    #define __INSANE_REPEAT_221(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_220(value, se, sae)
    #define __INSANE_REPEAT_222(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_221(value, se, sae)
    #define __INSANE_REPEAT_223(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_222(value, se, sae)
    #define __INSANE_REPEAT_224(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_223(value, se, sae)
    #define __INSANE_REPEAT_225(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_224(value, se, sae)
    #define __INSANE_REPEAT_226(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_225(value, se, sae)
    #define __INSANE_REPEAT_227(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_226(value, se, sae)
    #define __INSANE_REPEAT_228(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_227(value, se, sae)
    #define __INSANE_REPEAT_229(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_228(value, se, sae)
    #define __INSANE_REPEAT_230(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_229(value, se, sae)
    #define __INSANE_REPEAT_231(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_230(value, se, sae)
    #define __INSANE_REPEAT_232(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_231(value, se, sae)
    #define __INSANE_REPEAT_233(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_232(value, se, sae)
    #define __INSANE_REPEAT_234(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_233(value, se, sae)
    #define __INSANE_REPEAT_235(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_234(value, se, sae)
    #define __INSANE_REPEAT_236(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_235(value, se, sae)
    #define __INSANE_REPEAT_237(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_236(value, se, sae)
    #define __INSANE_REPEAT_238(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_237(value, se, sae)
    #define __INSANE_REPEAT_239(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_238(value, se, sae)
    #define __INSANE_REPEAT_240(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_239(value, se, sae)
    #define __INSANE_REPEAT_241(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_240(value, se, sae)
    #define __INSANE_REPEAT_242(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_241(value, se, sae)
    #define __INSANE_REPEAT_243(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_242(value, se, sae)
    #define __INSANE_REPEAT_244(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_243(value, se, sae)
    #define __INSANE_REPEAT_245(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_244(value, se, sae)
    #define __INSANE_REPEAT_246(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_245(value, se, sae)
    #define __INSANE_REPEAT_247(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_246(value, se, sae)
    #define __INSANE_REPEAT_248(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_247(value, se, sae)
    #define __INSANE_REPEAT_249(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_248(value, se, sae)
    #define __INSANE_REPEAT_250(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_249(value, se, sae)
    #define __INSANE_REPEAT_251(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_250(value, se, sae)
    #define __INSANE_REPEAT_252(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_251(value, se, sae)
    #define __INSANE_REPEAT_253(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_252(value, se, sae)
    #define __INSANE_REPEAT_254(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_253(value, se, sae)
    #define __INSANE_REPEAT_255(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_254(value, se, sae)
    #define __INSANE_REPEAT_256(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_255(value, se, sae)
    #define __INSANE_REPEAT_257(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_256(value, se, sae)
    #define __INSANE_REPEAT_258(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_257(value, se, sae)
    #define __INSANE_REPEAT_259(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_258(value, se, sae)
    #define __INSANE_REPEAT_260(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_259(value, se, sae)
    #define __INSANE_REPEAT_261(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_260(value, se, sae)
    #define __INSANE_REPEAT_262(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_261(value, se, sae)
    #define __INSANE_REPEAT_263(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_262(value, se, sae)
    #define __INSANE_REPEAT_264(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_263(value, se, sae)
    #define __INSANE_REPEAT_265(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_264(value, se, sae)
    #define __INSANE_REPEAT_266(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_265(value, se, sae)
    #define __INSANE_REPEAT_267(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_266(value, se, sae)
    #define __INSANE_REPEAT_268(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_267(value, se, sae)
    #define __INSANE_REPEAT_269(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_268(value, se, sae)
    #define __INSANE_REPEAT_270(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_269(value, se, sae)
    #define __INSANE_REPEAT_271(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_270(value, se, sae)
    #define __INSANE_REPEAT_272(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_271(value, se, sae)
    #define __INSANE_REPEAT_273(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_272(value, se, sae)
    #define __INSANE_REPEAT_274(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_273(value, se, sae)
    #define __INSANE_REPEAT_275(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_274(value, se, sae)
    #define __INSANE_REPEAT_276(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_275(value, se, sae)
    #define __INSANE_REPEAT_277(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_276(value, se, sae)
    #define __INSANE_REPEAT_278(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_277(value, se, sae)
    #define __INSANE_REPEAT_279(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_278(value, se, sae)
    #define __INSANE_REPEAT_280(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_279(value, se, sae)
    #define __INSANE_REPEAT_281(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_280(value, se, sae)
    #define __INSANE_REPEAT_282(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_281(value, se, sae)
    #define __INSANE_REPEAT_283(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_282(value, se, sae)
    #define __INSANE_REPEAT_284(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_283(value, se, sae)
    #define __INSANE_REPEAT_285(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_284(value, se, sae)
    #define __INSANE_REPEAT_286(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_285(value, se, sae)
    #define __INSANE_REPEAT_287(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_286(value, se, sae)
    #define __INSANE_REPEAT_288(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_287(value, se, sae)
    #define __INSANE_REPEAT_289(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_288(value, se, sae)
    #define __INSANE_REPEAT_290(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_289(value, se, sae)
    #define __INSANE_REPEAT_291(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_290(value, se, sae)
    #define __INSANE_REPEAT_292(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_291(value, se, sae)
    #define __INSANE_REPEAT_293(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_292(value, se, sae)
    #define __INSANE_REPEAT_294(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_293(value, se, sae)
    #define __INSANE_REPEAT_295(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_294(value, se, sae)
    #define __INSANE_REPEAT_296(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_295(value, se, sae)
    #define __INSANE_REPEAT_297(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_296(value, se, sae)
    #define __INSANE_REPEAT_298(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_297(value, se, sae)
    #define __INSANE_REPEAT_299(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_298(value, se, sae)
    #define __INSANE_REPEAT_300(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_299(value, se, sae)
    #define __INSANE_REPEAT_301(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_300(value, se, sae)
    #define __INSANE_REPEAT_302(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_301(value, se, sae)
    #define __INSANE_REPEAT_303(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_302(value, se, sae)
    #define __INSANE_REPEAT_304(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_303(value, se, sae)
    #define __INSANE_REPEAT_305(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_304(value, se, sae)
    #define __INSANE_REPEAT_306(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_305(value, se, sae)
    #define __INSANE_REPEAT_307(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_306(value, se, sae)
    #define __INSANE_REPEAT_308(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_307(value, se, sae)
    #define __INSANE_REPEAT_309(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_308(value, se, sae)
    #define __INSANE_REPEAT_310(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_309(value, se, sae)
    #define __INSANE_REPEAT_311(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_310(value, se, sae)
    #define __INSANE_REPEAT_312(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_311(value, se, sae)
    #define __INSANE_REPEAT_313(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_312(value, se, sae)
    #define __INSANE_REPEAT_314(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_313(value, se, sae)
    #define __INSANE_REPEAT_315(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_314(value, se, sae)
    #define __INSANE_REPEAT_316(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_315(value, se, sae)
    #define __INSANE_REPEAT_317(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_316(value, se, sae)
    #define __INSANE_REPEAT_318(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_317(value, se, sae)
    #define __INSANE_REPEAT_319(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_318(value, se, sae)
    #define __INSANE_REPEAT_320(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_319(value, se, sae)
    #define __INSANE_REPEAT_321(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_320(value, se, sae)
    #define __INSANE_REPEAT_322(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_321(value, se, sae)
    #define __INSANE_REPEAT_323(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_322(value, se, sae)
    #define __INSANE_REPEAT_324(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_323(value, se, sae)
    #define __INSANE_REPEAT_325(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_324(value, se, sae)
    #define __INSANE_REPEAT_326(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_325(value, se, sae)
    #define __INSANE_REPEAT_327(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_326(value, se, sae)
    #define __INSANE_REPEAT_328(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_327(value, se, sae)
    #define __INSANE_REPEAT_329(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_328(value, se, sae)
    #define __INSANE_REPEAT_330(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_329(value, se, sae)
    #define __INSANE_REPEAT_331(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_330(value, se, sae)
    #define __INSANE_REPEAT_332(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_331(value, se, sae)
    #define __INSANE_REPEAT_333(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_332(value, se, sae)
    #define __INSANE_REPEAT_334(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_333(value, se, sae)
    #define __INSANE_REPEAT_335(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_334(value, se, sae)
    #define __INSANE_REPEAT_336(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_335(value, se, sae)
    #define __INSANE_REPEAT_337(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_336(value, se, sae)
    #define __INSANE_REPEAT_338(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_337(value, se, sae)
    #define __INSANE_REPEAT_339(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_338(value, se, sae)
    #define __INSANE_REPEAT_340(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_339(value, se, sae)
    #define __INSANE_REPEAT_341(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_340(value, se, sae)
    #define __INSANE_REPEAT_342(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_341(value, se, sae)
    #define __INSANE_REPEAT_343(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_342(value, se, sae)
    #define __INSANE_REPEAT_344(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_343(value, se, sae)
    #define __INSANE_REPEAT_345(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_344(value, se, sae)
    #define __INSANE_REPEAT_346(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_345(value, se, sae)
    #define __INSANE_REPEAT_347(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_346(value, se, sae)
    #define __INSANE_REPEAT_348(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_347(value, se, sae)
    #define __INSANE_REPEAT_349(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_348(value, se, sae)
    #define __INSANE_REPEAT_350(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_349(value, se, sae)
    #define __INSANE_REPEAT_351(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_350(value, se, sae)
    #define __INSANE_REPEAT_352(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_351(value, se, sae)
    #define __INSANE_REPEAT_353(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_352(value, se, sae)
    #define __INSANE_REPEAT_354(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_353(value, se, sae)
    #define __INSANE_REPEAT_355(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_354(value, se, sae)
    #define __INSANE_REPEAT_356(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_355(value, se, sae)
    #define __INSANE_REPEAT_357(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_356(value, se, sae)
    #define __INSANE_REPEAT_358(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_357(value, se, sae)
    #define __INSANE_REPEAT_359(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_358(value, se, sae)
    #define __INSANE_REPEAT_360(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_359(value, se, sae)
    #define __INSANE_REPEAT_361(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_360(value, se, sae)
    #define __INSANE_REPEAT_362(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_361(value, se, sae)
    #define __INSANE_REPEAT_363(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_362(value, se, sae)
    #define __INSANE_REPEAT_364(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_363(value, se, sae)
    #define __INSANE_REPEAT_365(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_364(value, se, sae)
    #define __INSANE_REPEAT_366(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_365(value, se, sae)
    #define __INSANE_REPEAT_367(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_366(value, se, sae)
    #define __INSANE_REPEAT_368(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_367(value, se, sae)
    #define __INSANE_REPEAT_369(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_368(value, se, sae)
    #define __INSANE_REPEAT_370(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_369(value, se, sae)
    #define __INSANE_REPEAT_371(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_370(value, se, sae)
    #define __INSANE_REPEAT_372(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_371(value, se, sae)
    #define __INSANE_REPEAT_373(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_372(value, se, sae)
    #define __INSANE_REPEAT_374(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_373(value, se, sae)
    #define __INSANE_REPEAT_375(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_374(value, se, sae)
    #define __INSANE_REPEAT_376(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_375(value, se, sae)
    #define __INSANE_REPEAT_377(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_376(value, se, sae)
    #define __INSANE_REPEAT_378(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_377(value, se, sae)
    #define __INSANE_REPEAT_379(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_378(value, se, sae)
    #define __INSANE_REPEAT_380(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_379(value, se, sae)
    #define __INSANE_REPEAT_381(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_380(value, se, sae)
    #define __INSANE_REPEAT_382(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_381(value, se, sae)
    #define __INSANE_REPEAT_383(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_382(value, se, sae)
    #define __INSANE_REPEAT_384(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_383(value, se, sae)
    #define __INSANE_REPEAT_385(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_384(value, se, sae)
    #define __INSANE_REPEAT_386(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_385(value, se, sae)
    #define __INSANE_REPEAT_387(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_386(value, se, sae)
    #define __INSANE_REPEAT_388(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_387(value, se, sae)
    #define __INSANE_REPEAT_389(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_388(value, se, sae)
    #define __INSANE_REPEAT_390(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_389(value, se, sae)
    #define __INSANE_REPEAT_391(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_390(value, se, sae)
    #define __INSANE_REPEAT_392(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_391(value, se, sae)
    #define __INSANE_REPEAT_393(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_392(value, se, sae)
    #define __INSANE_REPEAT_394(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_393(value, se, sae)
    #define __INSANE_REPEAT_395(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_394(value, se, sae)
    #define __INSANE_REPEAT_396(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_395(value, se, sae)
    #define __INSANE_REPEAT_397(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_396(value, se, sae)
    #define __INSANE_REPEAT_398(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_397(value, se, sae)
    #define __INSANE_REPEAT_399(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_398(value, se, sae)
    #define __INSANE_REPEAT_400(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_399(value, se, sae)
    #define __INSANE_REPEAT_401(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_400(value, se, sae)
    #define __INSANE_REPEAT_402(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_401(value, se, sae)
    #define __INSANE_REPEAT_403(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_402(value, se, sae)
    #define __INSANE_REPEAT_404(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_403(value, se, sae)
    #define __INSANE_REPEAT_405(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_404(value, se, sae)
    #define __INSANE_REPEAT_406(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_405(value, se, sae)
    #define __INSANE_REPEAT_407(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_406(value, se, sae)
    #define __INSANE_REPEAT_408(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_407(value, se, sae)
    #define __INSANE_REPEAT_409(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_408(value, se, sae)
    #define __INSANE_REPEAT_410(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_409(value, se, sae)
    #define __INSANE_REPEAT_411(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_410(value, se, sae)
    #define __INSANE_REPEAT_412(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_411(value, se, sae)
    #define __INSANE_REPEAT_413(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_412(value, se, sae)
    #define __INSANE_REPEAT_414(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_413(value, se, sae)
    #define __INSANE_REPEAT_415(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_414(value, se, sae)
    #define __INSANE_REPEAT_416(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_415(value, se, sae)
    #define __INSANE_REPEAT_417(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_416(value, se, sae)
    #define __INSANE_REPEAT_418(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_417(value, se, sae)
    #define __INSANE_REPEAT_419(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_418(value, se, sae)
    #define __INSANE_REPEAT_420(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_419(value, se, sae)
    #define __INSANE_REPEAT_421(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_420(value, se, sae)
    #define __INSANE_REPEAT_422(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_421(value, se, sae)
    #define __INSANE_REPEAT_423(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_422(value, se, sae)
    #define __INSANE_REPEAT_424(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_423(value, se, sae)
    #define __INSANE_REPEAT_425(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_424(value, se, sae)
    #define __INSANE_REPEAT_426(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_425(value, se, sae)
    #define __INSANE_REPEAT_427(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_426(value, se, sae)
    #define __INSANE_REPEAT_428(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_427(value, se, sae)
    #define __INSANE_REPEAT_429(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_428(value, se, sae)
    #define __INSANE_REPEAT_430(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_429(value, se, sae)
    #define __INSANE_REPEAT_431(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_430(value, se, sae)
    #define __INSANE_REPEAT_432(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_431(value, se, sae)
    #define __INSANE_REPEAT_433(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_432(value, se, sae)
    #define __INSANE_REPEAT_434(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_433(value, se, sae)
    #define __INSANE_REPEAT_435(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_434(value, se, sae)
    #define __INSANE_REPEAT_436(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_435(value, se, sae)
    #define __INSANE_REPEAT_437(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_436(value, se, sae)
    #define __INSANE_REPEAT_438(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_437(value, se, sae)
    #define __INSANE_REPEAT_439(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_438(value, se, sae)
    #define __INSANE_REPEAT_440(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_439(value, se, sae)
    #define __INSANE_REPEAT_441(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_440(value, se, sae)
    #define __INSANE_REPEAT_442(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_441(value, se, sae)
    #define __INSANE_REPEAT_443(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_442(value, se, sae)
    #define __INSANE_REPEAT_444(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_443(value, se, sae)
    #define __INSANE_REPEAT_445(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_444(value, se, sae)
    #define __INSANE_REPEAT_446(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_445(value, se, sae)
    #define __INSANE_REPEAT_447(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_446(value, se, sae)
    #define __INSANE_REPEAT_448(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_447(value, se, sae)
    #define __INSANE_REPEAT_449(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_448(value, se, sae)
    #define __INSANE_REPEAT_450(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_449(value, se, sae)
    #define __INSANE_REPEAT_451(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_450(value, se, sae)
    #define __INSANE_REPEAT_452(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_451(value, se, sae)
    #define __INSANE_REPEAT_453(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_452(value, se, sae)
    #define __INSANE_REPEAT_454(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_453(value, se, sae)
    #define __INSANE_REPEAT_455(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_454(value, se, sae)
    #define __INSANE_REPEAT_456(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_455(value, se, sae)
    #define __INSANE_REPEAT_457(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_456(value, se, sae)
    #define __INSANE_REPEAT_458(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_457(value, se, sae)
    #define __INSANE_REPEAT_459(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_458(value, se, sae)
    #define __INSANE_REPEAT_460(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_459(value, se, sae)
    #define __INSANE_REPEAT_461(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_460(value, se, sae)
    #define __INSANE_REPEAT_462(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_461(value, se, sae)
    #define __INSANE_REPEAT_463(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_462(value, se, sae)
    #define __INSANE_REPEAT_464(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_463(value, se, sae)
    #define __INSANE_REPEAT_465(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_464(value, se, sae)
    #define __INSANE_REPEAT_466(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_465(value, se, sae)
    #define __INSANE_REPEAT_467(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_466(value, se, sae)
    #define __INSANE_REPEAT_468(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_467(value, se, sae)
    #define __INSANE_REPEAT_469(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_468(value, se, sae)
    #define __INSANE_REPEAT_470(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_469(value, se, sae)
    #define __INSANE_REPEAT_471(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_470(value, se, sae)
    #define __INSANE_REPEAT_472(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_471(value, se, sae)
    #define __INSANE_REPEAT_473(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_472(value, se, sae)
    #define __INSANE_REPEAT_474(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_473(value, se, sae)
    #define __INSANE_REPEAT_475(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_474(value, se, sae)
    #define __INSANE_REPEAT_476(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_475(value, se, sae)
    #define __INSANE_REPEAT_477(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_476(value, se, sae)
    #define __INSANE_REPEAT_478(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_477(value, se, sae)
    #define __INSANE_REPEAT_479(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_478(value, se, sae)
    #define __INSANE_REPEAT_480(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_479(value, se, sae)
    #define __INSANE_REPEAT_481(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_480(value, se, sae)
    #define __INSANE_REPEAT_482(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_481(value, se, sae)
    #define __INSANE_REPEAT_483(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_482(value, se, sae)
    #define __INSANE_REPEAT_484(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_483(value, se, sae)
    #define __INSANE_REPEAT_485(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_484(value, se, sae)
    #define __INSANE_REPEAT_486(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_485(value, se, sae)
    #define __INSANE_REPEAT_487(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_486(value, se, sae)
    #define __INSANE_REPEAT_488(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_487(value, se, sae)
    #define __INSANE_REPEAT_489(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_488(value, se, sae)
    #define __INSANE_REPEAT_490(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_489(value, se, sae)
    #define __INSANE_REPEAT_491(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_490(value, se, sae)
    #define __INSANE_REPEAT_492(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_491(value, se, sae)
    #define __INSANE_REPEAT_493(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_492(value, se, sae)
    #define __INSANE_REPEAT_494(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_493(value, se, sae)
    #define __INSANE_REPEAT_495(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_494(value, se, sae)
    #define __INSANE_REPEAT_496(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_495(value, se, sae)
    #define __INSANE_REPEAT_497(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_496(value, se, sae)
    #define __INSANE_REPEAT_498(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_497(value, se, sae)
    #define __INSANE_REPEAT_499(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_498(value, se, sae)
    #define __INSANE_REPEAT_500(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_499(value, se, sae)
    #define __INSANE_REPEAT_501(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_500(value, se, sae)
    #define __INSANE_REPEAT_502(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_501(value, se, sae)
    #define __INSANE_REPEAT_503(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_502(value, se, sae)
    #define __INSANE_REPEAT_504(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_503(value, se, sae)
    #define __INSANE_REPEAT_505(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_504(value, se, sae)
    #define __INSANE_REPEAT_506(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_505(value, se, sae)
    #define __INSANE_REPEAT_507(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_506(value, se, sae)
    #define __INSANE_REPEAT_508(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_507(value, se, sae)
    #define __INSANE_REPEAT_509(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_508(value, se, sae)
    #define __INSANE_REPEAT_510(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_509(value, se, sae)
    #define __INSANE_REPEAT_511(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_510(value, se, sae)
    #define __INSANE_REPEAT_512(value, se, sae) EXPAND_VALUE(value) se() __INSANE_REPEAT_511(value, se, sae)
    #define INSANE_REPEAT(value, n, se, sae) __INSANE_REPEAT_##n(value, se, sae)

    #define INSANE_1536_DIVIDED_BY_3() 512
    #define INSANE_1533_DIVIDED_BY_3() 511
    #define INSANE_1530_DIVIDED_BY_3() 510
    #define INSANE_1527_DIVIDED_BY_3() 509
    #define INSANE_1524_DIVIDED_BY_3() 508
    #define INSANE_1521_DIVIDED_BY_3() 507
    #define INSANE_1518_DIVIDED_BY_3() 506
    #define INSANE_1515_DIVIDED_BY_3() 505
    #define INSANE_1512_DIVIDED_BY_3() 504
    #define INSANE_1509_DIVIDED_BY_3() 503
    #define INSANE_1506_DIVIDED_BY_3() 502
    #define INSANE_1503_DIVIDED_BY_3() 501
    #define INSANE_1500_DIVIDED_BY_3() 500
    #define INSANE_1497_DIVIDED_BY_3() 499
    #define INSANE_1494_DIVIDED_BY_3() 498
    #define INSANE_1491_DIVIDED_BY_3() 497
    #define INSANE_1488_DIVIDED_BY_3() 496
    #define INSANE_1485_DIVIDED_BY_3() 495
    #define INSANE_1482_DIVIDED_BY_3() 494
    #define INSANE_1479_DIVIDED_BY_3() 493
    #define INSANE_1476_DIVIDED_BY_3() 492
    #define INSANE_1473_DIVIDED_BY_3() 491
    #define INSANE_1470_DIVIDED_BY_3() 490
    #define INSANE_1467_DIVIDED_BY_3() 489
    #define INSANE_1464_DIVIDED_BY_3() 488
    #define INSANE_1461_DIVIDED_BY_3() 487
    #define INSANE_1458_DIVIDED_BY_3() 486
    #define INSANE_1455_DIVIDED_BY_3() 485
    #define INSANE_1452_DIVIDED_BY_3() 484
    #define INSANE_1449_DIVIDED_BY_3() 483
    #define INSANE_1446_DIVIDED_BY_3() 482
    #define INSANE_1443_DIVIDED_BY_3() 481
    #define INSANE_1440_DIVIDED_BY_3() 480
    #define INSANE_1437_DIVIDED_BY_3() 479
    #define INSANE_1434_DIVIDED_BY_3() 478
    #define INSANE_1431_DIVIDED_BY_3() 477
    #define INSANE_1428_DIVIDED_BY_3() 476
    #define INSANE_1425_DIVIDED_BY_3() 475
    #define INSANE_1422_DIVIDED_BY_3() 474
    #define INSANE_1419_DIVIDED_BY_3() 473
    #define INSANE_1416_DIVIDED_BY_3() 472
    #define INSANE_1413_DIVIDED_BY_3() 471
    #define INSANE_1410_DIVIDED_BY_3() 470
    #define INSANE_1407_DIVIDED_BY_3() 469
    #define INSANE_1404_DIVIDED_BY_3() 468
    #define INSANE_1401_DIVIDED_BY_3() 467
    #define INSANE_1398_DIVIDED_BY_3() 466
    #define INSANE_1395_DIVIDED_BY_3() 465
    #define INSANE_1392_DIVIDED_BY_3() 464
    #define INSANE_1389_DIVIDED_BY_3() 463
    #define INSANE_1386_DIVIDED_BY_3() 462
    #define INSANE_1383_DIVIDED_BY_3() 461
    #define INSANE_1380_DIVIDED_BY_3() 460
    #define INSANE_1377_DIVIDED_BY_3() 459
    #define INSANE_1374_DIVIDED_BY_3() 458
    #define INSANE_1371_DIVIDED_BY_3() 457
    #define INSANE_1368_DIVIDED_BY_3() 456
    #define INSANE_1365_DIVIDED_BY_3() 455
    #define INSANE_1362_DIVIDED_BY_3() 454
    #define INSANE_1359_DIVIDED_BY_3() 453
    #define INSANE_1356_DIVIDED_BY_3() 452
    #define INSANE_1353_DIVIDED_BY_3() 451
    #define INSANE_1350_DIVIDED_BY_3() 450
    #define INSANE_1347_DIVIDED_BY_3() 449
    #define INSANE_1344_DIVIDED_BY_3() 448
    #define INSANE_1341_DIVIDED_BY_3() 447
    #define INSANE_1338_DIVIDED_BY_3() 446
    #define INSANE_1335_DIVIDED_BY_3() 445
    #define INSANE_1332_DIVIDED_BY_3() 444
    #define INSANE_1329_DIVIDED_BY_3() 443
    #define INSANE_1326_DIVIDED_BY_3() 442
    #define INSANE_1323_DIVIDED_BY_3() 441
    #define INSANE_1320_DIVIDED_BY_3() 440
    #define INSANE_1317_DIVIDED_BY_3() 439
    #define INSANE_1314_DIVIDED_BY_3() 438
    #define INSANE_1311_DIVIDED_BY_3() 437
    #define INSANE_1308_DIVIDED_BY_3() 436
    #define INSANE_1305_DIVIDED_BY_3() 435
    #define INSANE_1302_DIVIDED_BY_3() 434
    #define INSANE_1299_DIVIDED_BY_3() 433
    #define INSANE_1296_DIVIDED_BY_3() 432
    #define INSANE_1293_DIVIDED_BY_3() 431
    #define INSANE_1290_DIVIDED_BY_3() 430
    #define INSANE_1287_DIVIDED_BY_3() 429
    #define INSANE_1284_DIVIDED_BY_3() 428
    #define INSANE_1281_DIVIDED_BY_3() 427
    #define INSANE_1278_DIVIDED_BY_3() 426
    #define INSANE_1275_DIVIDED_BY_3() 425
    #define INSANE_1272_DIVIDED_BY_3() 424
    #define INSANE_1269_DIVIDED_BY_3() 423
    #define INSANE_1266_DIVIDED_BY_3() 422
    #define INSANE_1263_DIVIDED_BY_3() 421
    #define INSANE_1260_DIVIDED_BY_3() 420
    #define INSANE_1257_DIVIDED_BY_3() 419
    #define INSANE_1254_DIVIDED_BY_3() 418
    #define INSANE_1251_DIVIDED_BY_3() 417
    #define INSANE_1248_DIVIDED_BY_3() 416
    #define INSANE_1245_DIVIDED_BY_3() 415
    #define INSANE_1242_DIVIDED_BY_3() 414
    #define INSANE_1239_DIVIDED_BY_3() 413
    #define INSANE_1236_DIVIDED_BY_3() 412
    #define INSANE_1233_DIVIDED_BY_3() 411
    #define INSANE_1230_DIVIDED_BY_3() 410
    #define INSANE_1227_DIVIDED_BY_3() 409
    #define INSANE_1224_DIVIDED_BY_3() 408
    #define INSANE_1221_DIVIDED_BY_3() 407
    #define INSANE_1218_DIVIDED_BY_3() 406
    #define INSANE_1215_DIVIDED_BY_3() 405
    #define INSANE_1212_DIVIDED_BY_3() 404
    #define INSANE_1209_DIVIDED_BY_3() 403
    #define INSANE_1206_DIVIDED_BY_3() 402
    #define INSANE_1203_DIVIDED_BY_3() 401
    #define INSANE_1200_DIVIDED_BY_3() 400
    #define INSANE_1197_DIVIDED_BY_3() 399
    #define INSANE_1194_DIVIDED_BY_3() 398
    #define INSANE_1191_DIVIDED_BY_3() 397
    #define INSANE_1188_DIVIDED_BY_3() 396
    #define INSANE_1185_DIVIDED_BY_3() 395
    #define INSANE_1182_DIVIDED_BY_3() 394
    #define INSANE_1179_DIVIDED_BY_3() 393
    #define INSANE_1176_DIVIDED_BY_3() 392
    #define INSANE_1173_DIVIDED_BY_3() 391
    #define INSANE_1170_DIVIDED_BY_3() 390
    #define INSANE_1167_DIVIDED_BY_3() 389
    #define INSANE_1164_DIVIDED_BY_3() 388
    #define INSANE_1161_DIVIDED_BY_3() 387
    #define INSANE_1158_DIVIDED_BY_3() 386
    #define INSANE_1155_DIVIDED_BY_3() 385
    #define INSANE_1152_DIVIDED_BY_3() 384
    #define INSANE_1149_DIVIDED_BY_3() 383
    #define INSANE_1146_DIVIDED_BY_3() 382
    #define INSANE_1143_DIVIDED_BY_3() 381
    #define INSANE_1140_DIVIDED_BY_3() 380
    #define INSANE_1137_DIVIDED_BY_3() 379
    #define INSANE_1134_DIVIDED_BY_3() 378
    #define INSANE_1131_DIVIDED_BY_3() 377
    #define INSANE_1128_DIVIDED_BY_3() 376
    #define INSANE_1125_DIVIDED_BY_3() 375
    #define INSANE_1122_DIVIDED_BY_3() 374
    #define INSANE_1119_DIVIDED_BY_3() 373
    #define INSANE_1116_DIVIDED_BY_3() 372
    #define INSANE_1113_DIVIDED_BY_3() 371
    #define INSANE_1110_DIVIDED_BY_3() 370
    #define INSANE_1107_DIVIDED_BY_3() 369
    #define INSANE_1104_DIVIDED_BY_3() 368
    #define INSANE_1101_DIVIDED_BY_3() 367
    #define INSANE_1098_DIVIDED_BY_3() 366
    #define INSANE_1095_DIVIDED_BY_3() 365
    #define INSANE_1092_DIVIDED_BY_3() 364
    #define INSANE_1089_DIVIDED_BY_3() 363
    #define INSANE_1086_DIVIDED_BY_3() 362
    #define INSANE_1083_DIVIDED_BY_3() 361
    #define INSANE_1080_DIVIDED_BY_3() 360
    #define INSANE_1077_DIVIDED_BY_3() 359
    #define INSANE_1074_DIVIDED_BY_3() 358
    #define INSANE_1071_DIVIDED_BY_3() 357
    #define INSANE_1068_DIVIDED_BY_3() 356
    #define INSANE_1065_DIVIDED_BY_3() 355
    #define INSANE_1062_DIVIDED_BY_3() 354
    #define INSANE_1059_DIVIDED_BY_3() 353
    #define INSANE_1056_DIVIDED_BY_3() 352
    #define INSANE_1053_DIVIDED_BY_3() 351
    #define INSANE_1050_DIVIDED_BY_3() 350
    #define INSANE_1047_DIVIDED_BY_3() 349
    #define INSANE_1044_DIVIDED_BY_3() 348
    #define INSANE_1041_DIVIDED_BY_3() 347
    #define INSANE_1038_DIVIDED_BY_3() 346
    #define INSANE_1035_DIVIDED_BY_3() 345
    #define INSANE_1032_DIVIDED_BY_3() 344
    #define INSANE_1029_DIVIDED_BY_3() 343
    #define INSANE_1026_DIVIDED_BY_3() 342
    #define INSANE_1023_DIVIDED_BY_3() 341
    #define INSANE_1020_DIVIDED_BY_3() 340
    #define INSANE_1017_DIVIDED_BY_3() 339
    #define INSANE_1014_DIVIDED_BY_3() 338
    #define INSANE_1011_DIVIDED_BY_3() 337
    #define INSANE_1008_DIVIDED_BY_3() 336
    #define INSANE_1005_DIVIDED_BY_3() 335
    #define INSANE_1002_DIVIDED_BY_3() 334
    #define INSANE_999_DIVIDED_BY_3() 333
    #define INSANE_996_DIVIDED_BY_3() 332
    #define INSANE_993_DIVIDED_BY_3() 331
    #define INSANE_990_DIVIDED_BY_3() 330
    #define INSANE_987_DIVIDED_BY_3() 329
    #define INSANE_984_DIVIDED_BY_3() 328
    #define INSANE_981_DIVIDED_BY_3() 327
    #define INSANE_978_DIVIDED_BY_3() 326
    #define INSANE_975_DIVIDED_BY_3() 325
    #define INSANE_972_DIVIDED_BY_3() 324
    #define INSANE_969_DIVIDED_BY_3() 323
    #define INSANE_966_DIVIDED_BY_3() 322
    #define INSANE_963_DIVIDED_BY_3() 321
    #define INSANE_960_DIVIDED_BY_3() 320
    #define INSANE_957_DIVIDED_BY_3() 319
    #define INSANE_954_DIVIDED_BY_3() 318
    #define INSANE_951_DIVIDED_BY_3() 317
    #define INSANE_948_DIVIDED_BY_3() 316
    #define INSANE_945_DIVIDED_BY_3() 315
    #define INSANE_942_DIVIDED_BY_3() 314
    #define INSANE_939_DIVIDED_BY_3() 313
    #define INSANE_936_DIVIDED_BY_3() 312
    #define INSANE_933_DIVIDED_BY_3() 311
    #define INSANE_930_DIVIDED_BY_3() 310
    #define INSANE_927_DIVIDED_BY_3() 309
    #define INSANE_924_DIVIDED_BY_3() 308
    #define INSANE_921_DIVIDED_BY_3() 307
    #define INSANE_918_DIVIDED_BY_3() 306
    #define INSANE_915_DIVIDED_BY_3() 305
    #define INSANE_912_DIVIDED_BY_3() 304
    #define INSANE_909_DIVIDED_BY_3() 303
    #define INSANE_906_DIVIDED_BY_3() 302
    #define INSANE_903_DIVIDED_BY_3() 301
    #define INSANE_900_DIVIDED_BY_3() 300
    #define INSANE_897_DIVIDED_BY_3() 299
    #define INSANE_894_DIVIDED_BY_3() 298
    #define INSANE_891_DIVIDED_BY_3() 297
    #define INSANE_888_DIVIDED_BY_3() 296
    #define INSANE_885_DIVIDED_BY_3() 295
    #define INSANE_882_DIVIDED_BY_3() 294
    #define INSANE_879_DIVIDED_BY_3() 293
    #define INSANE_876_DIVIDED_BY_3() 292
    #define INSANE_873_DIVIDED_BY_3() 291
    #define INSANE_870_DIVIDED_BY_3() 290
    #define INSANE_867_DIVIDED_BY_3() 289
    #define INSANE_864_DIVIDED_BY_3() 288
    #define INSANE_861_DIVIDED_BY_3() 287
    #define INSANE_858_DIVIDED_BY_3() 286
    #define INSANE_855_DIVIDED_BY_3() 285
    #define INSANE_852_DIVIDED_BY_3() 284
    #define INSANE_849_DIVIDED_BY_3() 283
    #define INSANE_846_DIVIDED_BY_3() 282
    #define INSANE_843_DIVIDED_BY_3() 281
    #define INSANE_840_DIVIDED_BY_3() 280
    #define INSANE_837_DIVIDED_BY_3() 279
    #define INSANE_834_DIVIDED_BY_3() 278
    #define INSANE_831_DIVIDED_BY_3() 277
    #define INSANE_828_DIVIDED_BY_3() 276
    #define INSANE_825_DIVIDED_BY_3() 275
    #define INSANE_822_DIVIDED_BY_3() 274
    #define INSANE_819_DIVIDED_BY_3() 273
    #define INSANE_816_DIVIDED_BY_3() 272
    #define INSANE_813_DIVIDED_BY_3() 271
    #define INSANE_810_DIVIDED_BY_3() 270
    #define INSANE_807_DIVIDED_BY_3() 269
    #define INSANE_804_DIVIDED_BY_3() 268
    #define INSANE_801_DIVIDED_BY_3() 267
    #define INSANE_798_DIVIDED_BY_3() 266
    #define INSANE_795_DIVIDED_BY_3() 265
    #define INSANE_792_DIVIDED_BY_3() 264
    #define INSANE_789_DIVIDED_BY_3() 263
    #define INSANE_786_DIVIDED_BY_3() 262
    #define INSANE_783_DIVIDED_BY_3() 261
    #define INSANE_780_DIVIDED_BY_3() 260
    #define INSANE_777_DIVIDED_BY_3() 259
    #define INSANE_774_DIVIDED_BY_3() 258
    #define INSANE_771_DIVIDED_BY_3() 257
    #define INSANE_768_DIVIDED_BY_3() 256
    #define INSANE_765_DIVIDED_BY_3() 255
    #define INSANE_762_DIVIDED_BY_3() 254
    #define INSANE_759_DIVIDED_BY_3() 253
    #define INSANE_756_DIVIDED_BY_3() 252
    #define INSANE_753_DIVIDED_BY_3() 251
    #define INSANE_750_DIVIDED_BY_3() 250
    #define INSANE_747_DIVIDED_BY_3() 249
    #define INSANE_744_DIVIDED_BY_3() 248
    #define INSANE_741_DIVIDED_BY_3() 247
    #define INSANE_738_DIVIDED_BY_3() 246
    #define INSANE_735_DIVIDED_BY_3() 245
    #define INSANE_732_DIVIDED_BY_3() 244
    #define INSANE_729_DIVIDED_BY_3() 243
    #define INSANE_726_DIVIDED_BY_3() 242
    #define INSANE_723_DIVIDED_BY_3() 241
    #define INSANE_720_DIVIDED_BY_3() 240
    #define INSANE_717_DIVIDED_BY_3() 239
    #define INSANE_714_DIVIDED_BY_3() 238
    #define INSANE_711_DIVIDED_BY_3() 237
    #define INSANE_708_DIVIDED_BY_3() 236
    #define INSANE_705_DIVIDED_BY_3() 235
    #define INSANE_702_DIVIDED_BY_3() 234
    #define INSANE_699_DIVIDED_BY_3() 233
    #define INSANE_696_DIVIDED_BY_3() 232
    #define INSANE_693_DIVIDED_BY_3() 231
    #define INSANE_690_DIVIDED_BY_3() 230
    #define INSANE_687_DIVIDED_BY_3() 229
    #define INSANE_684_DIVIDED_BY_3() 228
    #define INSANE_681_DIVIDED_BY_3() 227
    #define INSANE_678_DIVIDED_BY_3() 226
    #define INSANE_675_DIVIDED_BY_3() 225
    #define INSANE_672_DIVIDED_BY_3() 224
    #define INSANE_669_DIVIDED_BY_3() 223
    #define INSANE_666_DIVIDED_BY_3() 222
    #define INSANE_663_DIVIDED_BY_3() 221
    #define INSANE_660_DIVIDED_BY_3() 220
    #define INSANE_657_DIVIDED_BY_3() 219
    #define INSANE_654_DIVIDED_BY_3() 218
    #define INSANE_651_DIVIDED_BY_3() 217
    #define INSANE_648_DIVIDED_BY_3() 216
    #define INSANE_645_DIVIDED_BY_3() 215
    #define INSANE_642_DIVIDED_BY_3() 214
    #define INSANE_639_DIVIDED_BY_3() 213
    #define INSANE_636_DIVIDED_BY_3() 212
    #define INSANE_633_DIVIDED_BY_3() 211
    #define INSANE_630_DIVIDED_BY_3() 210
    #define INSANE_627_DIVIDED_BY_3() 209
    #define INSANE_624_DIVIDED_BY_3() 208
    #define INSANE_621_DIVIDED_BY_3() 207
    #define INSANE_618_DIVIDED_BY_3() 206
    #define INSANE_615_DIVIDED_BY_3() 205
    #define INSANE_612_DIVIDED_BY_3() 204
    #define INSANE_609_DIVIDED_BY_3() 203
    #define INSANE_606_DIVIDED_BY_3() 202
    #define INSANE_603_DIVIDED_BY_3() 201
    #define INSANE_600_DIVIDED_BY_3() 200
    #define INSANE_597_DIVIDED_BY_3() 199
    #define INSANE_594_DIVIDED_BY_3() 198
    #define INSANE_591_DIVIDED_BY_3() 197
    #define INSANE_588_DIVIDED_BY_3() 196
    #define INSANE_585_DIVIDED_BY_3() 195
    #define INSANE_582_DIVIDED_BY_3() 194
    #define INSANE_579_DIVIDED_BY_3() 193
    #define INSANE_576_DIVIDED_BY_3() 192
    #define INSANE_573_DIVIDED_BY_3() 191
    #define INSANE_570_DIVIDED_BY_3() 190
    #define INSANE_567_DIVIDED_BY_3() 189
    #define INSANE_564_DIVIDED_BY_3() 188
    #define INSANE_561_DIVIDED_BY_3() 187
    #define INSANE_558_DIVIDED_BY_3() 186
    #define INSANE_555_DIVIDED_BY_3() 185
    #define INSANE_552_DIVIDED_BY_3() 184
    #define INSANE_549_DIVIDED_BY_3() 183
    #define INSANE_546_DIVIDED_BY_3() 182
    #define INSANE_543_DIVIDED_BY_3() 181
    #define INSANE_540_DIVIDED_BY_3() 180
    #define INSANE_537_DIVIDED_BY_3() 179
    #define INSANE_534_DIVIDED_BY_3() 178
    #define INSANE_531_DIVIDED_BY_3() 177
    #define INSANE_528_DIVIDED_BY_3() 176
    #define INSANE_525_DIVIDED_BY_3() 175
    #define INSANE_522_DIVIDED_BY_3() 174
    #define INSANE_519_DIVIDED_BY_3() 173
    #define INSANE_516_DIVIDED_BY_3() 172
    #define INSANE_513_DIVIDED_BY_3() 171
    #define INSANE_510_DIVIDED_BY_3() 170
    #define INSANE_507_DIVIDED_BY_3() 169
    #define INSANE_504_DIVIDED_BY_3() 168
    #define INSANE_501_DIVIDED_BY_3() 167
    #define INSANE_498_DIVIDED_BY_3() 166
    #define INSANE_495_DIVIDED_BY_3() 165
    #define INSANE_492_DIVIDED_BY_3() 164
    #define INSANE_489_DIVIDED_BY_3() 163
    #define INSANE_486_DIVIDED_BY_3() 162
    #define INSANE_483_DIVIDED_BY_3() 161
    #define INSANE_480_DIVIDED_BY_3() 160
    #define INSANE_477_DIVIDED_BY_3() 159
    #define INSANE_474_DIVIDED_BY_3() 158
    #define INSANE_471_DIVIDED_BY_3() 157
    #define INSANE_468_DIVIDED_BY_3() 156
    #define INSANE_465_DIVIDED_BY_3() 155
    #define INSANE_462_DIVIDED_BY_3() 154
    #define INSANE_459_DIVIDED_BY_3() 153
    #define INSANE_456_DIVIDED_BY_3() 152
    #define INSANE_453_DIVIDED_BY_3() 151
    #define INSANE_450_DIVIDED_BY_3() 150
    #define INSANE_447_DIVIDED_BY_3() 149
    #define INSANE_444_DIVIDED_BY_3() 148
    #define INSANE_441_DIVIDED_BY_3() 147
    #define INSANE_438_DIVIDED_BY_3() 146
    #define INSANE_435_DIVIDED_BY_3() 145
    #define INSANE_432_DIVIDED_BY_3() 144
    #define INSANE_429_DIVIDED_BY_3() 143
    #define INSANE_426_DIVIDED_BY_3() 142
    #define INSANE_423_DIVIDED_BY_3() 141
    #define INSANE_420_DIVIDED_BY_3() 140
    #define INSANE_417_DIVIDED_BY_3() 139
    #define INSANE_414_DIVIDED_BY_3() 138
    #define INSANE_411_DIVIDED_BY_3() 137
    #define INSANE_408_DIVIDED_BY_3() 136
    #define INSANE_405_DIVIDED_BY_3() 135
    #define INSANE_402_DIVIDED_BY_3() 134
    #define INSANE_399_DIVIDED_BY_3() 133
    #define INSANE_396_DIVIDED_BY_3() 132
    #define INSANE_393_DIVIDED_BY_3() 131
    #define INSANE_390_DIVIDED_BY_3() 130
    #define INSANE_387_DIVIDED_BY_3() 129
    #define INSANE_384_DIVIDED_BY_3() 128
    #define INSANE_381_DIVIDED_BY_3() 127
    #define INSANE_378_DIVIDED_BY_3() 126
    #define INSANE_375_DIVIDED_BY_3() 125
    #define INSANE_372_DIVIDED_BY_3() 124
    #define INSANE_369_DIVIDED_BY_3() 123
    #define INSANE_366_DIVIDED_BY_3() 122
    #define INSANE_363_DIVIDED_BY_3() 121
    #define INSANE_360_DIVIDED_BY_3() 120
    #define INSANE_357_DIVIDED_BY_3() 119
    #define INSANE_354_DIVIDED_BY_3() 118
    #define INSANE_351_DIVIDED_BY_3() 117
    #define INSANE_348_DIVIDED_BY_3() 116
    #define INSANE_345_DIVIDED_BY_3() 115
    #define INSANE_342_DIVIDED_BY_3() 114
    #define INSANE_339_DIVIDED_BY_3() 113
    #define INSANE_336_DIVIDED_BY_3() 112
    #define INSANE_333_DIVIDED_BY_3() 111
    #define INSANE_330_DIVIDED_BY_3() 110
    #define INSANE_327_DIVIDED_BY_3() 109
    #define INSANE_324_DIVIDED_BY_3() 108
    #define INSANE_321_DIVIDED_BY_3() 107
    #define INSANE_318_DIVIDED_BY_3() 106
    #define INSANE_315_DIVIDED_BY_3() 105
    #define INSANE_312_DIVIDED_BY_3() 104
    #define INSANE_309_DIVIDED_BY_3() 103
    #define INSANE_306_DIVIDED_BY_3() 102
    #define INSANE_303_DIVIDED_BY_3() 101
    #define INSANE_300_DIVIDED_BY_3() 100
    #define INSANE_297_DIVIDED_BY_3() 99
    #define INSANE_294_DIVIDED_BY_3() 98
    #define INSANE_291_DIVIDED_BY_3() 97
    #define INSANE_288_DIVIDED_BY_3() 96
    #define INSANE_285_DIVIDED_BY_3() 95
    #define INSANE_282_DIVIDED_BY_3() 94
    #define INSANE_279_DIVIDED_BY_3() 93
    #define INSANE_276_DIVIDED_BY_3() 92
    #define INSANE_273_DIVIDED_BY_3() 91
    #define INSANE_270_DIVIDED_BY_3() 90
    #define INSANE_267_DIVIDED_BY_3() 89
    #define INSANE_264_DIVIDED_BY_3() 88
    #define INSANE_261_DIVIDED_BY_3() 87
    #define INSANE_258_DIVIDED_BY_3() 86
    #define INSANE_255_DIVIDED_BY_3() 85
    #define INSANE_252_DIVIDED_BY_3() 84
    #define INSANE_249_DIVIDED_BY_3() 83
    #define INSANE_246_DIVIDED_BY_3() 82
    #define INSANE_243_DIVIDED_BY_3() 81
    #define INSANE_240_DIVIDED_BY_3() 80
    #define INSANE_237_DIVIDED_BY_3() 79
    #define INSANE_234_DIVIDED_BY_3() 78
    #define INSANE_231_DIVIDED_BY_3() 77
    #define INSANE_228_DIVIDED_BY_3() 76
    #define INSANE_225_DIVIDED_BY_3() 75
    #define INSANE_222_DIVIDED_BY_3() 74
    #define INSANE_219_DIVIDED_BY_3() 73
    #define INSANE_216_DIVIDED_BY_3() 72
    #define INSANE_213_DIVIDED_BY_3() 71
    #define INSANE_210_DIVIDED_BY_3() 70
    #define INSANE_207_DIVIDED_BY_3() 69
    #define INSANE_204_DIVIDED_BY_3() 68
    #define INSANE_201_DIVIDED_BY_3() 67
    #define INSANE_198_DIVIDED_BY_3() 66
    #define INSANE_195_DIVIDED_BY_3() 65
    #define INSANE_192_DIVIDED_BY_3() 64
    #define INSANE_189_DIVIDED_BY_3() 63
    #define INSANE_186_DIVIDED_BY_3() 62
    #define INSANE_183_DIVIDED_BY_3() 61
    #define INSANE_180_DIVIDED_BY_3() 60
    #define INSANE_177_DIVIDED_BY_3() 59
    #define INSANE_174_DIVIDED_BY_3() 58
    #define INSANE_171_DIVIDED_BY_3() 57
    #define INSANE_168_DIVIDED_BY_3() 56
    #define INSANE_165_DIVIDED_BY_3() 55
    #define INSANE_162_DIVIDED_BY_3() 54
    #define INSANE_159_DIVIDED_BY_3() 53
    #define INSANE_156_DIVIDED_BY_3() 52
    #define INSANE_153_DIVIDED_BY_3() 51
    #define INSANE_150_DIVIDED_BY_3() 50
    #define INSANE_147_DIVIDED_BY_3() 49
    #define INSANE_144_DIVIDED_BY_3() 48
    #define INSANE_141_DIVIDED_BY_3() 47
    #define INSANE_138_DIVIDED_BY_3() 46
    #define INSANE_135_DIVIDED_BY_3() 45
    #define INSANE_132_DIVIDED_BY_3() 44
    #define INSANE_129_DIVIDED_BY_3() 43
    #define INSANE_126_DIVIDED_BY_3() 42
    #define INSANE_123_DIVIDED_BY_3() 41
    #define INSANE_120_DIVIDED_BY_3() 40
    #define INSANE_117_DIVIDED_BY_3() 39
    #define INSANE_114_DIVIDED_BY_3() 38
    #define INSANE_111_DIVIDED_BY_3() 37
    #define INSANE_108_DIVIDED_BY_3() 36
    #define INSANE_105_DIVIDED_BY_3() 35
    #define INSANE_102_DIVIDED_BY_3() 34
    #define INSANE_99_DIVIDED_BY_3() 33
    #define INSANE_96_DIVIDED_BY_3() 32
    #define INSANE_93_DIVIDED_BY_3() 31
    #define INSANE_90_DIVIDED_BY_3() 30
    #define INSANE_87_DIVIDED_BY_3() 29
    #define INSANE_84_DIVIDED_BY_3() 28
    #define INSANE_81_DIVIDED_BY_3() 27
    #define INSANE_78_DIVIDED_BY_3() 26
    #define INSANE_75_DIVIDED_BY_3() 25
    #define INSANE_72_DIVIDED_BY_3() 24
    #define INSANE_69_DIVIDED_BY_3() 23
    #define INSANE_66_DIVIDED_BY_3() 22
    #define INSANE_63_DIVIDED_BY_3() 21
    #define INSANE_60_DIVIDED_BY_3() 20
    #define INSANE_57_DIVIDED_BY_3() 19
    #define INSANE_54_DIVIDED_BY_3() 18
    #define INSANE_51_DIVIDED_BY_3() 17
    #define INSANE_48_DIVIDED_BY_3() 16
    #define INSANE_45_DIVIDED_BY_3() 15
    #define INSANE_42_DIVIDED_BY_3() 14
    #define INSANE_39_DIVIDED_BY_3() 13
    #define INSANE_36_DIVIDED_BY_3() 12
    #define INSANE_33_DIVIDED_BY_3() 11
    #define INSANE_30_DIVIDED_BY_3() 10
    #define INSANE_27_DIVIDED_BY_3() 9
    #define INSANE_24_DIVIDED_BY_3() 8
    #define INSANE_21_DIVIDED_BY_3() 7
    #define INSANE_18_DIVIDED_BY_3() 6
    #define INSANE_15_DIVIDED_BY_3() 5
    #define INSANE_12_DIVIDED_BY_3() 4
    #define INSANE_9_DIVIDED_BY_3() 3
    #define INSANE_6_DIVIDED_BY_3() 2
    #define INSANE_3_DIVIDED_BY_3() 1

    #define __INSANE_GET_MULTIPLE_OF_3_EXPAND_3(count) count()
    #define __INSANE_GET_MULTIPLE_OF_3_EXPAND_2(count) __INSANE_GET_MULTIPLE_OF_3_EXPAND_3(INSANE_##count##_DIVIDED_BY_3)
    #define __INSANE_GET_MULTIPLE_OF_3_EXPAND_1(count) __INSANE_GET_MULTIPLE_OF_3_EXPAND_2(count)
    #define INSANE_GET_MULTIPLE_OF_3(...) __INSANE_GET_MULTIPLE_OF_3_EXPAND_1(COUNT_ARGUMENTS(__VA_ARGS__))

    #define __INSANE_ENUM_REPEAT_1(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value)
    #define __INSANE_ENUM_REPEAT_2(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_1(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_3(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_2(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_4(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_3(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_5(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_4(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_6(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_5(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_7(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_6(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_8(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_7(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_9(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_8(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_10(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_9(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_11(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_10(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_12(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_11(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_13(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_12(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_14(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_13(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_15(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_14(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_16(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_15(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_17(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_16(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_18(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_17(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_19(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_18(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_20(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_19(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_21(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_20(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_22(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_21(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_23(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_22(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_24(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_23(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_25(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_24(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_26(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_25(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_27(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_26(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_28(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_27(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_29(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_28(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_30(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_29(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_31(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_30(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_32(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_31(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_33(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_32(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_34(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_33(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_35(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_34(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_36(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_35(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_37(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_36(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_38(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_37(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_39(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_38(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_40(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_39(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_41(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_40(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_42(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_41(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_43(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_42(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_44(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_43(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_45(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_44(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_46(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_45(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_47(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_46(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_48(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_47(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_49(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_48(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_50(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_49(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_51(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_50(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_52(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_51(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_53(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_52(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_54(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_53(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_55(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_54(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_56(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_55(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_57(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_56(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_58(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_57(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_59(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_58(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_60(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_59(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_61(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_60(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_62(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_61(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_63(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_62(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_64(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_63(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_65(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_64(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_66(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_65(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_67(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_66(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_68(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_67(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_69(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_68(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_70(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_69(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_71(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_70(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_72(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_71(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_73(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_72(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_74(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_73(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_75(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_74(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_76(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_75(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_77(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_76(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_78(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_77(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_79(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_78(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_80(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_79(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_81(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_80(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_82(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_81(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_83(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_82(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_84(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_83(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_85(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_84(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_86(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_85(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_87(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_86(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_88(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_87(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_89(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_88(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_90(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_89(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_91(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_90(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_92(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_91(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_93(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_92(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_94(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_93(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_95(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_94(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_96(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_95(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_97(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_96(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_98(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_97(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_99(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_98(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_100(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_99(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_101(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_100(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_102(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_101(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_103(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_102(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_104(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_103(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_105(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_104(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_106(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_105(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_107(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_106(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_108(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_107(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_109(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_108(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_110(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_109(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_111(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_110(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_112(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_111(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_113(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_112(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_114(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_113(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_115(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_114(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_116(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_115(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_117(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_116(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_118(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_117(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_119(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_118(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_120(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_119(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_121(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_120(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_122(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_121(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_123(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_122(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_124(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_123(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_125(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_124(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_126(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_125(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_127(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_126(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_128(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_127(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_129(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_128(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_130(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_129(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_131(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_130(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_132(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_131(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_133(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_132(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_134(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_133(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_135(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_134(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_136(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_135(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_137(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_136(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_138(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_137(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_139(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_138(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_140(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_139(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_141(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_140(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_142(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_141(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_143(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_142(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_144(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_143(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_145(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_144(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_146(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_145(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_147(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_146(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_148(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_147(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_149(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_148(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_150(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_149(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_151(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_150(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_152(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_151(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_153(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_152(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_154(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_153(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_155(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_154(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_156(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_155(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_157(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_156(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_158(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_157(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_159(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_158(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_160(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_159(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_161(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_160(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_162(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_161(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_163(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_162(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_164(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_163(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_165(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_164(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_166(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_165(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_167(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_166(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_168(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_167(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_169(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_168(Name, UType, count, x, __VA_ARGS__)
    #define __INSANE_ENUM_REPEAT_170(Name, UType, count, x, var, oper, value, ...) x(Name, UType, var, oper, value) __INSANE_ENUM_REPEAT_169(Name, UType, count, x, __VA_ARGS__)

    #define __INSANE_ENUM_MAP_ENUMT_STRINGT_VALUE(Name, UType, var, oper, value) {Name::var, #var},
    #define __INSANE_ENUM_MAP_INTEGRALT_ENUMT_VALUE(Name, UType, var, oper, value) {static_cast<UType>(Name::var), Name::var},
    #define __INSANE_ENUM_MAP_STRINGT_INTEGRALT_VALUE_NO_NAME(Name, UType, var, oper, value) {#var, Name::var},
    #define __INSANE_ENUM_MAP_STRINGT_INTEGRALT_VALUE_WITH_NAME(Name, UType, var, oper, value) {STRINGIFY_DEFER(Name::var), Name::var},
    #define __INSANE_ENUM_MAP_STRINGT_INTEGRALT_VALUE_INTEGRAL(Name, UType, var, oper, value) {IntegralExtensions::ToString(static_cast<UType>(Name::var)), Name::var},

    #define __INSANE_ENUM_GET_EQ(value) = value,
    #define __INSANE_ENUM_GET__(value) ,
    #define __INSANE_ENUM_GET_VALUE_EXPAND(exp, value) exp(value)
    #define __INSANE_ENUM_GET_VALUE(Name, UType, var, oper, value) var __INSANE_ENUM_GET_VALUE_EXPAND(__INSANE_ENUM_GET_##oper, value)
    #define __INSANE_ENUM_GET_ONLY_VALUE(Name, UType, var, oper, value) Name::var,

    #define __INSANE_ENUM_EXPAND_3(Name, UType, count, iexp, exp, ...) exp(Name, UType, count, iexp, __VA_ARGS__)
    #define __INSANE_ENUM_EXPAND_2(Name, UType, count, iexp, exp, ...) __INSANE_ENUM_EXPAND_3(Name, UType, count, iexp, exp, __VA_ARGS__)
    #define __INSANE_ENUM_EXPAND_1(Name, UType, count, iexp, ...) __INSANE_ENUM_EXPAND_2(Name, UType, count, iexp, __INSANE_ENUM_REPEAT_##count, __VA_ARGS__)

    #define __INSANE_ENUM_DECLARATION(Name, UType, count, ...) enum class INSANE_API Name : UType \
    {                                                                                             \
        __INSANE_ENUM_EXPAND_1(Name, UType, count, __INSANE_ENUM_GET_VALUE, __VA_ARGS__)          \
    };

    #define __INSANE_ENUM_EXTENSIONS_DECLARATION(Name, UType, count, ...)                                                                                                                                                                                                                                                                                                                           \
        class INSANE_API Name##EnumExtensions                                                                                                                                                                                                                                                                                                                                                       \
        {                                                                                                                                                                                                                                                                                                                                                                                           \
        public:                                                                                                                                                                                                                                                                                                                                                                                     \
            [[nodiscard]] static String ToIntegralString(const Name &value)                                                                                                                                                                                                                                                                                                                         \
            {                                                                                                                                                                                                                                                                                                                                                                                       \
                USING_NS_INSANE_CORE;                                                                                                                                                                                                                                                                                                                                                               \
                return EnumExtensions::ToIntegralString(value);                                                                                                                                                                                                                                                                                                                                     \
            }                                                                                                                                                                                                                                                                                                                                                                                       \
            [[nodiscard]] static UType ToIntegral(const Name &value)                                                                                                                                                                                                                                                                                                                                \
            {                                                                                                                                                                                                                                                                                                                                                                                       \
                USING_NS_INSANE_CORE;                                                                                                                                                                                                                                                                                                                                                               \
                return EnumExtensions::ToIntegral<Name>(value);                                                                                                                                                                                                                                                                                                                                     \
            }                                                                                                                                                                                                                                                                                                                                                                                       \
            [[nodiscard]] static std::string ToString(const Name &value, bool includeEnumName = false)                                                                                                                                                                                                                                                                                              \
            {                                                                                                                                                                                                                                                                                                                                                                                       \
                static const std::map<Name, String> values = {__INSANE_ENUM_EXPAND_1(Name, UType, count, __INSANE_ENUM_MAP_ENUMT_STRINGT_VALUE, __VA_ARGS__)};                                                                                                                                                                                                                                      \
                return includeEnumName ? STRINGIFY_DEFER(Name::) + values.at(value) : values.at(value);                                                                                                                                                                                                                                                                                             \
            }                                                                                                                                                                                                                                                                                                                                                                                       \
            [[nodiscard]] static Name Parse(const UType &value)                                                                                                                                                                                                                                                                                                                                     \
            {                                                                                                                                                                                                                                                                                                                                                                                       \
                USING_NS_INSANE_EXCEPTION;                                                                                                                                                                                                                                                                                                                                                          \
                static const std::map<UType, Name> values = {__INSANE_ENUM_EXPAND_1(Name, UType, count, __INSANE_ENUM_MAP_INTEGRALT_ENUMT_VALUE, __VA_ARGS__)};                                                                                                                                                                                                                                     \
                try                                                                                                                                                                                                                                                                                                                                                                                 \
                {                                                                                                                                                                                                                                                                                                                                                                                   \
                    return values.at(value);                                                                                                                                                                                                                                                                                                                                                        \
                }                                                                                                                                                                                                                                                                                                                                                                                   \
                catch (...)                                                                                                                                                                                                                                                                                                                                                                         \
                {                                                                                                                                                                                                                                                                                                                                                                                   \
                    throw ParseException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);                                                                                                                                                                                                                                                                                                            \
                }                                                                                                                                                                                                                                                                                                                                                                                   \
            }                                                                                                                                                                                                                                                                                                                                                                                       \
            [[nodiscard]] static Name Parse(const String &value)                                                                                                                                                                                                                                                                                                                                    \
            {                                                                                                                                                                                                                                                                                                                                                                                       \
                USING_NS_INSANE_CORE;                                                                                                                                                                                                                                                                                                                                                               \
                USING_NS_INSANE_EXCEPTION;                                                                                                                                                                                                                                                                                                                                                          \
                static const std::map<String, Name> values = {__INSANE_ENUM_EXPAND_1(Name, UType, count, __INSANE_ENUM_MAP_STRINGT_INTEGRALT_VALUE_NO_NAME, __VA_ARGS__) __INSANE_ENUM_EXPAND_1(Name, UType, count, __INSANE_ENUM_MAP_STRINGT_INTEGRALT_VALUE_WITH_NAME, __VA_ARGS__) __INSANE_ENUM_EXPAND_1(Name, UType, count, __INSANE_ENUM_MAP_STRINGT_INTEGRALT_VALUE_INTEGRAL, __VA_ARGS__)}; \
                try                                                                                                                                                                                                                                                                                                                                                                                 \
                {                                                                                                                                                                                                                                                                                                                                                                                   \
                    return values.at(value);                                                                                                                                                                                                                                                                                                                                                        \
                }                                                                                                                                                                                                                                                                                                                                                                                   \
                catch (...)                                                                                                                                                                                                                                                                                                                                                                         \
                {                                                                                                                                                                                                                                                                                                                                                                                   \
                    throw ParseException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);                                                                                                                                                                                                                                                                                                            \
                }                                                                                                                                                                                                                                                                                                                                                                                   \
            }                                                                                                                                                                                                                                                                                                                                                                                       \
            [[nodiscard]] static std::vector<Name> GetValues()                                                                                                                                                                                                                                                                                                                                      \
            {                                                                                                                                                                                                                                                                                                                                                                                       \
                return {__INSANE_ENUM_EXPAND_1(Name, UType, count, __INSANE_ENUM_GET_ONLY_VALUE, __VA_ARGS__)};                                                                                                                                                                                                                                                                                     \
            }                                                                                                                                                                                                                                                                                                                                                                                       \
        };
    #define __INSANE_ENUM_OUTPUT_OPERATOR_DECLARATION(Name, UType, count, ...)          \
        inline INSANE_API std::ostream &operator<<(std::ostream &os, const Name &value) \
        {                                                                               \
            os << Name##EnumExtensions::ToString(value);                                \
            return os;                                                                  \
        }
    #define __INSANE_ENUM_UT_EXPAND_1(Name, UType, count, ...) __INSANE_ENUM_DECLARATION(Name, UType, count, __VA_ARGS__) __INSANE_ENUM_EXTENSIONS_DECLARATION(Name, UType, count, __VA_ARGS__) __INSANE_ENUM_OUTPUT_OPERATOR_DECLARATION(Name, UType, count, __VA_ARGS_)
    #define INSANE_ENUM_UT(Name, UType, ...) __INSANE_ENUM_UT_EXPAND_1(Name, UType, INSANE_GET_MULTIPLE_OF_3(__VA_ARGS__), __VA_ARGS__)
    #define INSANE_ENUM(Name, ...) INSANE_ENUM_UT(Name, int, __VA_ARGS__)

namespace InsaneIO::Insane::Preprocessor
{

} // namespace InsaneIO::Insane::Preprocessor

#endif // !INSANE_PREPROCESSOR_H
