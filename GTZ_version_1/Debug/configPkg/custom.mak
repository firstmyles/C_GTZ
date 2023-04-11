## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/Sys_BIOs_config_pe66.oe66

linker.cmd: package/cfg/Sys_BIOs_config_pe66.xdl
	$(SED) 's"^\"\(package/cfg/Sys_BIOs_config_pe66cfg.cmd\)\"$""\"C:/Users/Ryan Trueman/Documents/C for Embedded Systems/GTZ_all_freq_2022/Debug/configPkg/\1\""' package/cfg/Sys_BIOs_config_pe66.xdl > $@
