

proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "myip" "NUM_INSTANCES" "DEVICE_ID"  "C_M00_AXI_BASEADDR" "C_M00_AXI_HIGHADDR" "C_S00_AXI_BASEADDR" "C_S00_AXI_HIGHADDR"
}
