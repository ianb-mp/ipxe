/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include "etherboot.h"
#include "dev.h"
#include "console.h"

#include "config/general.h"

/*
 * Build ID string calculations
 *
 */
#undef XSTR
#undef STR
#define XSTR(s) STR(s)
#define STR(s) #s

#ifdef BUILD_SERIAL
#include "config/.buildserial.h"
#define BUILD_SERIAL_STR " #" XSTR(BUILD_SERIAL_NUM)
#else
#define BUILD_SERIAL_STR ""
#endif

#ifdef BUILD_ID
#define BUILD_ID_STR " " BUILD_ID
#else
#define BUILD_ID_STR ""
#endif

#if defined(BUILD_ID) || defined(BUILD_SERIAL)
#define BUILD_STRING " [build" BUILD_ID_STR BUILD_SERIAL_STR "]"
#else
#define BUILD_STRING ""
#endif

/*
 * Print out configuration
 *
 */
void print_config ( void ) {
	printf( "Etherboot " VERSION BUILD_STRING
		" (GPL) http://etherboot.org\n"
		"Drivers: " );
	print_drivers();
	printf( "  Images: " 
#ifdef	TAGGED_IMAGE
		"NBI "
#endif
#ifdef	ELF64_IMAGE
		"ELF64 "
#endif
#ifdef	ELF_IMAGE
		"ELF "
#endif
#ifdef	COFF_IMAGE
		"COFF "
#endif
#ifdef	IMAGE_FREEBSD
		"FreeBSD "
#endif
#ifdef	IMAGE_MULTIBOOT
		"Multiboot "
#endif
#ifdef	AOUT_IMAGE
		"a.out "
#endif
#ifdef	WINCE_IMAGE
		"WINCE "
#endif
#ifdef	PXE_IMAGE
		"PXE "
#endif
#ifdef PXE_EXPORT /* All possible exports */
		"  Exports: "
#ifdef PXE_EXPORT
		"PXE "
#endif
#endif /* All possible exports */
		"  "
		);
#if	(BOOTP_SERVER != 67) || (BOOTP_CLIENT != 68)
	printf( "[DHCP ports %d and %d] ",
		BOOTP_SERVER, BOOTP_CLIENT);
#endif
	putchar('\n');
	printf( "Protocols: "
#ifdef RARP_NOT_BOOTP
		"RARP "
#else
# ifndef NO_DHCP_SUPPORT
		"DHCP "
# else
		"BOOTP "
# endif
#endif
#ifdef DOWNLOAD_PROTO_TFTP
		"TFTP "
#endif
#ifdef  DOWNLOAD_PROTO_NFS
		"NFS "
#endif
#ifdef  DOWNLOAD_PROTO_SLAM
		"SLAM "
#endif
#ifdef  DOWNLOAD_PROTO_TFTM
		"TFTM "
#endif
#ifdef  DOWNLOAD_PROTO_HTTP
		"HTTP "
#endif
#ifdef  PROTO_LACP
		"LACP "
#endif
#ifdef DNS_RESOLVER
		"DNS "
#endif
		"\n");
#ifdef KEEP_IT_REAL
	printf( "Keeping It Real [EXPERIMENTAL]\n" );
#endif
}

/*
 * Drag in all requested console types
 *
 * CONSOLE_DUAL sets both CONSOLE_FIRMWARE and CONSOLE_SERIAL for
 * legacy compatibility.
 *
 */

#ifdef	CONSOLE_DUAL
#undef	CONSOLE_FIRMWARE
#define	CONSOLE_FIRMWARE	1
#undef	CONSOLE_SERIAL
#define	CONSOLE_SERIAL		1
#endif

#ifdef CONSOLE_FIRMWARE
REQUIRE_OBJECT ( bios_console );
#endif

#ifdef CONSOLE_SERIAL
REQUIRE_OBJECT ( serial );
#endif

#ifdef CONSOLE_DIRECT_VGA
REQUIRE_OBJECT ( video_subr );
#endif

#ifdef CONSOLE_BTEXT
REQUIRE_OBJECT ( btext );
#endif

#ifdef CONSOLE_PC_KBD
REQUIRE_OBJECT ( pc_kbd );
#endif

/*
 * Drag in relocate.o if required
 *
 */

#ifdef RELOCATE
REQUIRE_OBJECT ( relocate );
#endif
