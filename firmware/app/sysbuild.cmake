if(SB_CONFIG_BOOTLOADER_MCUBOOT)
	set(
		mcuboot_EXTRA_DTC_OVERLAY_FILE
		"${CMAKE_CURRENT_LIST_DIR}/sysbuild/mcuboot.overlay"
		CACHE INTERNAL "MCUboot partition overlay"
		FORCE
	)
endif()
