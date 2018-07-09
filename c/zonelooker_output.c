#include "zonelooker.h"

#define PNG_DEBUG 3
#include <png.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void abort_(const char * s, ...) {
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

struct country_color {
	char cc[2];
	uint8_t r,g,b;
};

int color_for_country_cmp(const char *key, const struct country_color *color) {
	return strncmp(key, color->cc, 2);
}

const struct country_color *color_for_country(const char *cc) {
	static const struct country_color ocean_color = {.cc = "XO", .r = 0, .g = 0, .b = 0xff};
	static const struct country_color colors[] = {
		{.cc = "AD", .r = 0x93, .g = 0x54, .b = 0xf2},
		{.cc = "AE", .r = 0x53, .g = 0xee, .b = 0x9e},
		{.cc = "AF", .r = 0xc9, .g = 0x51, .b = 0x2d},
		{.cc = "AG", .r = 0x09, .g = 0xeb, .b = 0x41},
		{.cc = "AI", .r = 0x54, .g = 0x83, .b = 0x5a},
		{.cc = "AL", .r = 0x20, .g = 0x32, .b = 0x88},
		{.cc = "AM", .r = 0xe0, .g = 0x88, .b = 0xe4},
		{.cc = "AO", .r = 0xba, .g = 0x8d, .b = 0x3b},
		{.cc = "AQ", .r = 0x5a, .g = 0x58, .b = 0xd2},
		{.cc = "AR", .r = 0xc0, .g = 0xe7, .b = 0x61},
		{.cc = "AS", .r = 0x00, .g = 0x5d, .b = 0x0d},
		{.cc = "AT", .r = 0x2e, .g = 0xe9, .b = 0x00},
		{.cc = "AU", .r = 0xee, .g = 0x53, .b = 0x6c},
		{.cc = "AW", .r = 0xb4, .g = 0x56, .b = 0xb3},
		{.cc = "AX", .r = 0x29, .g = 0x84, .b = 0xc4},
		{.cc = "AZ", .r = 0x73, .g = 0x81, .b = 0x1b},
		{.cc = "BA", .r = 0x39, .g = 0xc4, .b = 0x01},
		{.cc = "BB", .r = 0xa3, .g = 0x7b, .b = 0xb2},
		{.cc = "BD", .r = 0x4d, .g = 0x75, .b = 0xd3},
		{.cc = "BE", .r = 0x8d, .g = 0xcf, .b = 0xbf},
		{.cc = "BF", .r = 0x17, .g = 0x70, .b = 0x0c},
		{.cc = "BG", .r = 0xd7, .g = 0xca, .b = 0x60},
		{.cc = "BH", .r = 0x4a, .g = 0x18, .b = 0x17},
		{.cc = "BI", .r = 0x8a, .g = 0xa2, .b = 0x7b},
		{.cc = "BJ", .r = 0x10, .g = 0x1d, .b = 0xc8},
		{.cc = "BL", .r = 0xfe, .g = 0x13, .b = 0xa9},
		{.cc = "BM", .r = 0x3e, .g = 0xa9, .b = 0xc5},
		{.cc = "BN", .r = 0xa4, .g = 0x16, .b = 0x76},
		{.cc = "BO", .r = 0x64, .g = 0xac, .b = 0x1a},
		{.cc = "BR", .r = 0x1e, .g = 0xc6, .b = 0x40},
		{.cc = "BS", .r = 0xde, .g = 0x7c, .b = 0x2c},
		{.cc = "BT", .r = 0xf0, .g = 0xc8, .b = 0x21},
		{.cc = "BV", .r = 0xaa, .g = 0xcd, .b = 0xfe},
		{.cc = "BW", .r = 0x6a, .g = 0x77, .b = 0x92},
		{.cc = "BY", .r = 0x37, .g = 0x1f, .b = 0x89},
		{.cc = "BZ", .r = 0xad, .g = 0xa0, .b = 0x3a},
		{.cc = "CA", .r = 0x73, .g = 0xdb, .b = 0x1e},
		{.cc = "CC", .r = 0x29, .g = 0xde, .b = 0xc1},
		{.cc = "CD", .r = 0x07, .g = 0x6a, .b = 0xcc},
		{.cc = "CF", .r = 0x5d, .g = 0x6f, .b = 0x13},
		{.cc = "CG", .r = 0x9d, .g = 0xd5, .b = 0x7f},
		{.cc = "CH", .r = 0x00, .g = 0x07, .b = 0x08},
		{.cc = "CI", .r = 0xc0, .g = 0xbd, .b = 0x64},
		{.cc = "CK", .r = 0x9a, .g = 0xb8, .b = 0xbb},
		{.cc = "CL", .r = 0xb4, .g = 0x0c, .b = 0xb6},
		{.cc = "CM", .r = 0x74, .g = 0xb6, .b = 0xda},
		{.cc = "CN", .r = 0xee, .g = 0x09, .b = 0x69},
		{.cc = "CO", .r = 0x2e, .g = 0xb3, .b = 0x05},
		{.cc = "CR", .r = 0x54, .g = 0xd9, .b = 0x5f},
		{.cc = "CU", .r = 0x7a, .g = 0x6d, .b = 0x52},
		{.cc = "CV", .r = 0xe0, .g = 0xd2, .b = 0xe1},
		{.cc = "CW", .r = 0x20, .g = 0x68, .b = 0x8d},
		{.cc = "CX", .r = 0xbd, .g = 0xba, .b = 0xfa},
		{.cc = "CY", .r = 0x7d, .g = 0x00, .b = 0x96},
		{.cc = "CZ", .r = 0xe7, .g = 0xbf, .b = 0x25},
		{.cc = "DE", .r = 0xea, .g = 0xfc, .b = 0xfb},
		{.cc = "DJ", .r = 0x77, .g = 0x2e, .b = 0x8c},
		{.cc = "DK", .r = 0xb7, .g = 0x94, .b = 0xe0},
		{.cc = "DM", .r = 0x59, .g = 0x9a, .b = 0x81},
		{.cc = "DO", .r = 0x03, .g = 0x9f, .b = 0x5e},
		{.cc = "DZ", .r = 0xca, .g = 0x93, .b = 0x7e},
		{.cc = "EC", .r = 0x4e, .g = 0xed, .b = 0x85},
		{.cc = "EE", .r = 0xa0, .g = 0xe3, .b = 0xe4},
		{.cc = "EG", .r = 0xfa, .g = 0xe6, .b = 0x3b},
		{.cc = "EH", .r = 0x67, .g = 0x34, .b = 0x4c},
		{.cc = "ER", .r = 0x33, .g = 0xea, .b = 0x1b},
		{.cc = "ES", .r = 0xf3, .g = 0x50, .b = 0x77},
		{.cc = "ET", .r = 0xdd, .g = 0xe4, .b = 0x7a},
		{.cc = "FI", .r = 0x79, .g = 0xaf, .b = 0x01},
		{.cc = "FJ", .r = 0xe3, .g = 0x10, .b = 0xb2},
		{.cc = "FK", .r = 0x23, .g = 0xaa, .b = 0xde},
		{.cc = "FM", .r = 0xcd, .g = 0xa4, .b = 0xbf},
		{.cc = "FO", .r = 0x97, .g = 0xa1, .b = 0x60},
		{.cc = "FR", .r = 0xed, .g = 0xcb, .b = 0x3a},
		{.cc = "GA", .r = 0x80, .g = 0xd6, .b = 0x64},
		{.cc = "GB", .r = 0x1a, .g = 0x69, .b = 0xd7},
		{.cc = "GD", .r = 0xf4, .g = 0x67, .b = 0xb6},
		{.cc = "GE", .r = 0x34, .g = 0xdd, .b = 0xda},
		{.cc = "GF", .r = 0xae, .g = 0x62, .b = 0x69},
		{.cc = "GG", .r = 0x6e, .g = 0xd8, .b = 0x05},
		{.cc = "GH", .r = 0xf3, .g = 0x0a, .b = 0x72},
		{.cc = "GI", .r = 0x33, .g = 0xb0, .b = 0x1e},
		{.cc = "GL", .r = 0x47, .g = 0x01, .b = 0xcc},
		{.cc = "GM", .r = 0x87, .g = 0xbb, .b = 0xa0},
		{.cc = "GN", .r = 0x1d, .g = 0x04, .b = 0x13},
		{.cc = "GP", .r = 0xfd, .g = 0xd1, .b = 0xfa},
		{.cc = "GQ", .r = 0x3d, .g = 0x6b, .b = 0x96},
		{.cc = "GR", .r = 0xa7, .g = 0xd4, .b = 0x25},
		{.cc = "GS", .r = 0x67, .g = 0x6e, .b = 0x49},
		{.cc = "GT", .r = 0x49, .g = 0xda, .b = 0x44},
		{.cc = "GU", .r = 0x89, .g = 0x60, .b = 0x28},
		{.cc = "GW", .r = 0xd3, .g = 0x65, .b = 0xf7},
		{.cc = "GY", .r = 0x8e, .g = 0x0d, .b = 0xec},
		{.cc = "HK", .r = 0x79, .g = 0xf2, .b = 0x69},
		{.cc = "HM", .r = 0x97, .g = 0xfc, .b = 0x08},
		{.cc = "HN", .r = 0x0d, .g = 0x43, .b = 0xbb},
		{.cc = "HR", .r = 0xb7, .g = 0x93, .b = 0x8d},
		{.cc = "HT", .r = 0x59, .g = 0x9d, .b = 0xec},
		{.cc = "HU", .r = 0x99, .g = 0x27, .b = 0x80},
		{.cc = "ID", .r = 0xae, .g = 0x3f, .b = 0x01},
		{.cc = "IE", .r = 0x6e, .g = 0x85, .b = 0x6d},
		{.cc = "IL", .r = 0x1d, .g = 0x59, .b = 0x7b},
		{.cc = "IM", .r = 0xdd, .g = 0xe3, .b = 0x17},
		{.cc = "IN", .r = 0x47, .g = 0x5c, .b = 0xa4},
		{.cc = "IO", .r = 0x87, .g = 0xe6, .b = 0xc8},
		{.cc = "IQ", .r = 0x67, .g = 0x33, .b = 0x21},
		{.cc = "IR", .r = 0xfd, .g = 0x8c, .b = 0x92},
		{.cc = "IS", .r = 0x3d, .g = 0x36, .b = 0xfe},
		{.cc = "IT", .r = 0x13, .g = 0x82, .b = 0xf3},
		{.cc = "JE", .r = 0xb0, .g = 0xa4, .b = 0x4c},
		{.cc = "JM", .r = 0x03, .g = 0xc2, .b = 0x36},
		{.cc = "JO", .r = 0x59, .g = 0xc7, .b = 0xe9},
		{.cc = "JP", .r = 0x79, .g = 0xa8, .b = 0x6c},
		{.cc = "KE", .r = 0xfa, .g = 0xbb, .b = 0x53},
		{.cc = "KG", .r = 0xa0, .g = 0xbe, .b = 0x8c},
		{.cc = "KH", .r = 0x3d, .g = 0x6c, .b = 0xfb},
		{.cc = "KI", .r = 0xfd, .g = 0xd6, .b = 0x97},
		{.cc = "KM", .r = 0x49, .g = 0xdd, .b = 0x29},
		{.cc = "KN", .r = 0xd3, .g = 0x62, .b = 0x9a},
		{.cc = "KP", .r = 0x33, .g = 0xb7, .b = 0x73},
		{.cc = "KR", .r = 0x69, .g = 0xb2, .b = 0xac},
		{.cc = "KW", .r = 0x1d, .g = 0x03, .b = 0x7e},
		{.cc = "KY", .r = 0x40, .g = 0x6b, .b = 0x65},
		{.cc = "KZ", .r = 0xda, .g = 0xd4, .b = 0xd6},
		{.cc = "LA", .r = 0x63, .g = 0x9c, .b = 0xb6},
		{.cc = "LB", .r = 0xf9, .g = 0x23, .b = 0x05},
		{.cc = "LC", .r = 0x39, .g = 0x99, .b = 0x69},
		{.cc = "LI", .r = 0xd0, .g = 0xfa, .b = 0xcc},
		{.cc = "LK", .r = 0x8a, .g = 0xff, .b = 0x13},
		{.cc = "LR", .r = 0x44, .g = 0x9e, .b = 0xf7},
		{.cc = "LS", .r = 0x84, .g = 0x24, .b = 0x9b},
		{.cc = "LT", .r = 0xaa, .g = 0x90, .b = 0x96},
		{.cc = "LU", .r = 0x6a, .g = 0x2a, .b = 0xfa},
		{.cc = "LV", .r = 0xf0, .g = 0x95, .b = 0x49},
		{.cc = "LY", .r = 0x6d, .g = 0x47, .b = 0x3e},
		{.cc = "MA", .r = 0x29, .g = 0x83, .b = 0xa9},
		{.cc = "MC", .r = 0x73, .g = 0x86, .b = 0x76},
		{.cc = "MD", .r = 0x5d, .g = 0x32, .b = 0x7b},
		{.cc = "ME", .r = 0x9d, .g = 0x88, .b = 0x17},
		{.cc = "MF", .r = 0x07, .g = 0x37, .b = 0xa4},
		{.cc = "MG", .r = 0xc7, .g = 0x8d, .b = 0xc8},
		{.cc = "MH", .r = 0x5a, .g = 0x5f, .b = 0xbf},
		{.cc = "MK", .r = 0xc0, .g = 0xe0, .b = 0x0c},
		{.cc = "ML", .r = 0xee, .g = 0x54, .b = 0x01},
		{.cc = "MM", .r = 0x2e, .g = 0xee, .b = 0x6d},
		{.cc = "MN", .r = 0xb4, .g = 0x51, .b = 0xde},
		{.cc = "MO", .r = 0x74, .g = 0xeb, .b = 0xb2},
		{.cc = "MP", .r = 0x54, .g = 0x84, .b = 0x37},
		{.cc = "MQ", .r = 0x94, .g = 0x3e, .b = 0x5b},
		{.cc = "MR", .r = 0x0e, .g = 0x81, .b = 0xe8},
		{.cc = "MS", .r = 0xce, .g = 0x3b, .b = 0x84},
		{.cc = "MT", .r = 0xe0, .g = 0x8f, .b = 0x89},
		{.cc = "MU", .r = 0x20, .g = 0x35, .b = 0xe5},
		{.cc = "MV", .r = 0xba, .g = 0x8a, .b = 0x56},
		{.cc = "MW", .r = 0x7a, .g = 0x30, .b = 0x3a},
		{.cc = "MX", .r = 0xe7, .g = 0xe2, .b = 0x4d},
		{.cc = "MY", .r = 0x27, .g = 0x58, .b = 0x21},
		{.cc = "MZ", .r = 0xbd, .g = 0xe7, .b = 0x92},
		{.cc = "NA", .r = 0xf7, .g = 0xa2, .b = 0x88},
		{.cc = "NC", .r = 0xad, .g = 0xa7, .b = 0x57},
		{.cc = "NE", .r = 0x43, .g = 0xa9, .b = 0x36},
		{.cc = "NF", .r = 0xd9, .g = 0x16, .b = 0x85},
		{.cc = "NG", .r = 0x19, .g = 0xac, .b = 0xe9},
		{.cc = "NI", .r = 0x44, .g = 0xc4, .b = 0xf2},
		{.cc = "NL", .r = 0x30, .g = 0x75, .b = 0x20},
		{.cc = "NO", .r = 0xaa, .g = 0xca, .b = 0x93},
		{.cc = "NP", .r = 0x8a, .g = 0xa5, .b = 0x16},
		{.cc = "NR", .r = 0xd0, .g = 0xa0, .b = 0xc9},
		{.cc = "NU", .r = 0xfe, .g = 0x14, .b = 0xc4},
		{.cc = "NZ", .r = 0x63, .g = 0xc6, .b = 0xb3},
		{.cc = "OM", .r = 0xba, .g = 0xd0, .b = 0x53},
		{.cc = "PA", .r = 0xd7, .g = 0x2d, .b = 0xd8},
		{.cc = "PE", .r = 0x63, .g = 0x26, .b = 0x66},
		{.cc = "PF", .r = 0xf9, .g = 0x99, .b = 0xd5},
		{.cc = "PG", .r = 0x39, .g = 0x23, .b = 0xb9},
		{.cc = "PH", .r = 0xa4, .g = 0xf1, .b = 0xce},
		{.cc = "PK", .r = 0x3e, .g = 0x4e, .b = 0x7d},
		{.cc = "PL", .r = 0x10, .g = 0xfa, .b = 0x70},
		{.cc = "PM", .r = 0xd0, .g = 0x40, .b = 0x1c},
		{.cc = "PN", .r = 0x4a, .g = 0xff, .b = 0xaf},
		{.cc = "PR", .r = 0xf0, .g = 0x2f, .b = 0x99},
		{.cc = "PS", .r = 0x30, .g = 0x95, .b = 0xf5},
		{.cc = "PT", .r = 0x1e, .g = 0x21, .b = 0xf8},
		{.cc = "PW", .r = 0x84, .g = 0x9e, .b = 0x4b},
		{.cc = "PY", .r = 0xd9, .g = 0xf6, .b = 0x50},
		{.cc = "QA", .r = 0x9d, .g = 0x32, .b = 0xc7},
		{.cc = "RE", .r = 0xf7, .g = 0x18, .b = 0x58},
		{.cc = "RO", .r = 0x1e, .g = 0x7b, .b = 0xfd},
		{.cc = "RS", .r = 0xa4, .g = 0xab, .b = 0xcb},
		{.cc = "RU", .r = 0x4a, .g = 0xa5, .b = 0xaa},
		{.cc = "RW", .r = 0x10, .g = 0xa0, .b = 0x75},
		{.cc = "SA", .r = 0x09, .g = 0x0c, .b = 0xf9},
		{.cc = "SB", .r = 0x93, .g = 0xb3, .b = 0x4a},
		{.cc = "SC", .r = 0x53, .g = 0x09, .b = 0x26},
		{.cc = "SD", .r = 0x7d, .g = 0xbd, .b = 0x2b},
		{.cc = "SE", .r = 0xbd, .g = 0x07, .b = 0x47},
		{.cc = "SG", .r = 0xe7, .g = 0x02, .b = 0x98},
		{.cc = "SH", .r = 0x7a, .g = 0xd0, .b = 0xef},
		{.cc = "SI", .r = 0xba, .g = 0x6a, .b = 0x83},
		{.cc = "SJ", .r = 0x20, .g = 0xd5, .b = 0x30},
		{.cc = "SK", .r = 0xe0, .g = 0x6f, .b = 0x5c},
		{.cc = "SL", .r = 0xce, .g = 0xdb, .b = 0x51},
		{.cc = "SM", .r = 0x0e, .g = 0x61, .b = 0x3d},
		{.cc = "SN", .r = 0x94, .g = 0xde, .b = 0x8e},
		{.cc = "SO", .r = 0x54, .g = 0x64, .b = 0xe2},
		{.cc = "SR", .r = 0x2e, .g = 0x0e, .b = 0xb8},
		{.cc = "SS", .r = 0xee, .g = 0xb4, .b = 0xd4},
		{.cc = "ST", .r = 0xc0, .g = 0x00, .b = 0xd9},
		{.cc = "SV", .r = 0x9a, .g = 0x05, .b = 0x06},
		{.cc = "SX", .r = 0xc7, .g = 0x6d, .b = 0x1d},
		{.cc = "SY", .r = 0x07, .g = 0xd7, .b = 0x71},
		{.cc = "SZ", .r = 0x9d, .g = 0x68, .b = 0xc2},
		{.cc = "TC", .r = 0x7e, .g = 0x25, .b = 0x7d},
		{.cc = "TD", .r = 0x50, .g = 0x91, .b = 0x70},
		{.cc = "TG", .r = 0xca, .g = 0x2e, .b = 0xc3},
		{.cc = "TH", .r = 0x57, .g = 0xfc, .b = 0xb4},
		{.cc = "TJ", .r = 0x0d, .g = 0xf9, .b = 0x6b},
		{.cc = "TK", .r = 0xcd, .g = 0x43, .b = 0x07},
		{.cc = "TL", .r = 0xe3, .g = 0xf7, .b = 0x0a},
		{.cc = "TM", .r = 0x23, .g = 0x4d, .b = 0x66},
		{.cc = "TN", .r = 0xb9, .g = 0xf2, .b = 0xd5},
		{.cc = "TO", .r = 0x79, .g = 0x48, .b = 0xb9},
		{.cc = "TR", .r = 0x03, .g = 0x22, .b = 0xe3},
		{.cc = "TT", .r = 0xed, .g = 0x2c, .b = 0x82},
		{.cc = "TV", .r = 0xb7, .g = 0x29, .b = 0x5d},
		{.cc = "TW", .r = 0x77, .g = 0x93, .b = 0x31},
		{.cc = "TZ", .r = 0xb0, .g = 0x44, .b = 0x99},
		{.cc = "UA", .r = 0x6e, .g = 0x3f, .b = 0xbd},
		{.cc = "UG", .r = 0x80, .g = 0x31, .b = 0xdc},
		{.cc = "UM", .r = 0x69, .g = 0x52, .b = 0x79},
		{.cc = "US", .r = 0x89, .g = 0x87, .b = 0x90},
		{.cc = "UY", .r = 0x60, .g = 0xe4, .b = 0x35},
		{.cc = "UZ", .r = 0xfa, .g = 0x5b, .b = 0x86},
		{.cc = "VA", .r = 0xb0, .g = 0x1e, .b = 0x9c},
		{.cc = "VC", .r = 0xea, .g = 0x1b, .b = 0x43},
		{.cc = "VE", .r = 0x04, .g = 0x15, .b = 0x22},
		{.cc = "VG", .r = 0x5e, .g = 0x10, .b = 0xfd},
		{.cc = "VI", .r = 0x03, .g = 0x78, .b = 0xe6},
		{.cc = "VN", .r = 0x2d, .g = 0xcc, .b = 0xeb},
		{.cc = "VU", .r = 0xb9, .g = 0xa8, .b = 0xd0},
		{.cc = "WF", .r = 0xd4, .g = 0xb5, .b = 0x8e},
		{.cc = "WS", .r = 0x1d, .g = 0xb9, .b = 0xae},
		{.cc = "XK", .r = 0x03, .g = 0x25, .b = 0x8e},
		{.cc = "YE", .r = 0x14, .g = 0x52, .b = 0x8a},
		{.cc = "YT", .r = 0x69, .g = 0x55, .b = 0x14},
		{.cc = "ZA", .r = 0x7e, .g = 0x78, .b = 0x15},
		{.cc = "ZM", .r = 0x79, .g = 0x15, .b = 0xd1},
		{.cc = "ZW", .r = 0x2d, .g = 0xcb, .b = 0x86}
	};
	if (cc == NULL) return &ocean_color;
	void *found = bsearch(cc, colors, sizeof(colors)/sizeof(colors[0]), sizeof(colors[0]), (int(*)(const void *, const void *))(color_for_country_cmp));
	return found?:&ocean_color;
}

const char *zoneLokupPixel(const struct zl_table *table, int x, int y);

void write_png_file(const struct zl_table *table, const char *file_name) {
	int width = 360 * zl_deg_pixels(table);
	int height = 180 * zl_deg_pixels(table);

	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp) abort_("[write_png_file] File %s could not be opened for writing", file_name);

	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) abort_("[write_png_file] png_create_write_struct failed");

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) abort_("[write_png_file] png_create_info_struct failed");
	if (setjmp(png_jmpbuf(png_ptr))) abort_("[write_png_file] Error during init_io");
	png_init_io(png_ptr, fp);

	/* write header */
	if (setjmp(png_jmpbuf(png_ptr))) abort_("[write_png_file] Error during writing header");
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	
	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr))) abort_("[write_png_file] Error during writing bytes");
	
	png_bytep row = malloc(width * 3);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			const char *country = zoneLokupPixel(table, x, y);
			const struct country_color *color = color_for_country(country);
			row[3 * x] = color->r;
			row[3 * x + 1] = color->g;
			row[3 * x + 2] = color->b;
		}
		png_write_row(png_ptr, row);
	}
	free(row);
	
	/* end write */
	if (setjmp(png_jmpbuf(png_ptr))) abort_("[write_png_file] Error during end of write");
	
	png_write_end(png_ptr, NULL);
	
	fclose(fp);
}

int main (int argc, char const *argv[])
{
	if (argc == 1) {
		write_png_file(zl_get_table(NULL), argv[1]);
		return 0;
	} else {
		fprintf(stderr, "usage: %s path\n", argv[0]);
		return 1;
	}
}