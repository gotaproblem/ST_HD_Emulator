cmake_minimum_required ( VERSION 3.2 )
cmake_policy ( VERSION 3.24 )

include ( $ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake )

project ( ATARI_HDC_EMU )

pico_sdk_init ()

# leave above alone (other than changing project name)
# now define project related stuff

#add_executable (
#	pio_try
#)

#pico_generate_pio_header ( pio_try ${CMAKE_CURRENT_LIST_DIR}/try.pio )

#target_sources (
#	pio_try 
#	PRIVATE
#   	main.c
#	sdcard.c 
#	fileIO.c
#	mountFS.c
#	mountRAW.c
#)

#target_link_libraries (
#	pio_try PRIVATE
#	pico_stdlib 
#	hardware_pio 
#	hardware_spi 
#	FatFs_SPI
#	pico_multicore
#

add_subdirectory ( ../../no-OS-FatFS/FatFs_SPI build )


add_executable(
	${PROJECT_NAME} 
	emusdcard.c
	emu.c 
	fileIO.c
	mountFS.c
	mountRAW.c
	emurtc.c
	emuinit.c
	emuscsi.c
	shell/emushell.c
	shell/uptime.c
	shell/datetime.c
	)

target_link_libraries( 
	${PROJECT_NAME}
	FatFs_SPI
	pico_stdlib
	pico_multicore
	hardware_spi
	hardware_rtc
	hardware_clocks
	hardware_pll
#	pico_printf
#	hardware_pio
	)

#pico_enable_stdio_usb ( pio_try 1 )
pico_enable_stdio_usb ( ${PROJECT_NAME} 1 )

#pico_add_extra_outputs ( pio_try )
pico_add_extra_outputs(${PROJECT_NAME})

# stop the "Out of Memory" PANIC
add_compile_definitions ( PICO_MALLOC_PANIC=0 )
#pico_set_binary_type ( ${PROJECT_NAME} no_flash )
add_compile_options ( PICO_COPY_TO_RAM )
