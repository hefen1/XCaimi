#pragma  once

#ifdef _HSEXT
	#define HS_EXT_CLASS       AFX_CLASS_EXPORT
	#define HS_EXT_API         AFX_API_EXPORT
	#define HS_EXT_DATA        AFX_DATA_EXPORT
	#define HS_EXT_DATADEF
#else
	#define HS_EXT_CLASS       AFX_CLASS_IMPORT
	#define HS_EXT_API         AFX_API_IMPORT
	#define HS_EXT_DATA        AFX_DATA_IMPORT
	#define HS_EXT_DATADEF
#endif